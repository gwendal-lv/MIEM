/*
  ==============================================================================

    OSCRecorder.cpp
    Created: 7 Feb 2019 9:09:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <thread>
#include <algorithm>
#include <random>

#include "boost/lexical_cast.hpp"
//#include "boost/property_tree/ptree.hpp"

#include "MonitoringServer.h"

#include "OSCRecorder.h"
#include "OSCRecorderComponent.h"

#include "MainComponent.h"

#include "InterprocessControlBlock.h"


// The constructor will init the experiment with random preset values
OSCRecorder::OSCRecorder(MainComponent* _mainComponent)
:
mainComponent(_mainComponent),
recorderComponent(*(_mainComponent->oscRecorderComponent.get()))
{
    recorderComponent.SetRecorderManager(this);
    mainComponent->oscRecorderIntroComponent->SetUserQuestionsManager(this);
    mainComponent->firstUserQuestionsComponent->SetUserQuestionsManager(this);
    mainComponent->finalUserQuestionsComponent->SetUserQuestionsManager(this);
    
    // Setup of the Reaper controller at first (might wait for REAPER responses...)
    reaperController = std::make_shared<ReaperOscController>(1
                                                             + TrialSynthsCount * 2
                                                             + ExperimentPresetsCount); // testés 2 fois
    reaperController->SetTrackSolo_usingMutes(-1);
    
    // Setup of TCP control connection to the server inside MIEM Controller
    tcpConnection = std::make_shared<OSCRecorderConnection>(*this);
    
    // Internal timers
    researchDurationTimer = std::make_unique<OSCRecorderTimer>(this);
    // White noise must begin before the "after finished" pause has ended
    assert(WhiteNoiseStartDelay_ms < delayAfterFinished_ms);
}

void OSCRecorder::BeginExperiment()
{
    // (re)init of the whole experiment (presets, scenes, ...)
    reinitExperiment();
    
    // Actual Setup trials in next callbacks within the Juce Message thread
    // Will launch the next steps, after reconnection !
    juce::MessageManager::callAsync([this] {this->tryConnectSocketAndContinueExpe();});
}

void OSCRecorder::reinitExperiment()
{
    getExperimentParametersFromXmlFile();
    
    // new name will be extracted from the beginning date
    startTimePt = MiemClock::now();
    startSystemTime = Time::getCurrentTime();
    mainComponent->expeLabel->setText(startSystemTime.toString(true, true), NotificationType::sendNotification);
    
    // BEFORE doing anything : we check that we will be able to save the files...
    createNewDataFiles();
    
    // TCP connection reinit
    tcpConnection->disconnect();
    hasConnectionBeenLostDuringLastStep = true; // to force re-connect
    stateBeforeConnectionLost = ExperimentState::IntroDescriptionDisplayed; // forced fake state
    
    // OSC UDP listener (no check for connection stability....)
    oscRealtimeListener = std::make_shared<OSCListenerForwarder>(udpOscPort, startTimePt);
    
    if (presets.size() > 0)
    {
        MonitoringServer::SendLog("REINITIALISATION SANS SAUVEGARDE DES DONNEES !!");
        assert(false);
    }
    
    // - - - - Presets vector - - - -
    // is always the same... re-cleared of its data each time
    presets.clear();
    presets.reserve(ExperimentPresetsCount + TrialPresetsCount);
    // normal presets at the beginning
    // fader/interp is not random anymore...
    /*
    auto longIntSeed = Time::currentTimeMillis() % 1000000;
    int timeSeed = (int) longIntSeed;
    auto generator = std::bind(std::uniform_int_distribution<>(0,1),
                               std::mt19937(timeSeed)); // graine à zéro par défaut !
    for (int i=0 ; i<ExperimentPresetsCount ; i++)
    {
        bool findByInterpolation = generator(); // auto-cast ?
        presets.push_back(std::make_shared<MiemExpePreset>(i, findByInterpolation));
    }
    */
    assert((ExperimentPresetsCount % 2) == 0); // on a besoin de 2 presets par synthé ! pour comparer
    int synthsCount = ExperimentPresetsCount / 2;
    for (int i=0; i<ExperimentPresetsCount ; i++)
        presets.push_back(std::make_shared<MiemExpePreset>(i % synthsCount,
                                                           (i >= synthsCount))); // interp/fader ?
    // trial presets at the end of the vector (for the same synth)
    presets.push_back(std::make_shared<MiemExpePreset>(-2, false)); // fader
    presets.push_back(std::make_shared<MiemExpePreset>(-1, true)); // interpolation
    
    // - - - - Random Index maps - - - -
    presetRandomIdx.clear();
    presetRandomIdx = MiemExpePreset::GeneratePresetIndexToRandomIndexMap((int)ExperimentPresetsCount,
                                                                          (int)TrialPresetsCount,
                                                                          randomizePresets);
    // Random indexes stored also into presets (for saving in files, later)
    for (int i = -TrialPresetsCount; i<ExperimentPresetsCount ; i++)
    {
        int indexInPresetsVector = (int) presetRandomIdx.at(i);
        presets[indexInPresetsVector]->SetAppearanceIndex(i);
    }
    // - - - Display of presets indexes (simple check), and randomization map
    std::stringstream logStream;
    logStream << "Presets SynthId and isInterpo : ";
    for (int i=0 ; i<presets.size() ; i++)
        logStream << presets[i]->GetSynthId() << "/" << presets[i]->GetIsFoundFromInterpolation() << " ";
    MonitoringServer::SendLog(logStream.str());
    std::stringstream logStream2;
    logStream2 << "Presets randomised indexes:     ";
    for (int i=0 ; i<presetRandomIdx.size() ; i++)
        logStream2 << presetRandomIdx.at(-(int)(TrialPresetsCount) + i) << "   ";
    MonitoringServer::SendLog(logStream2.str());
    
    // - - - Init of main counter
    currentPresetStep = -(int)TrialPresetsCount - 1;
}
                                    
void OSCRecorder::tryConnectSocketAndContinueExpe()
{
    // We don't know how much we will wait... Maybe juste a few ms... or maybe minutes...
    changeState(ExperimentState::WaitingForTcpServerConnection);
    
    bool connectionSucceeded = tcpConnection->connectToSocket(String(tcpServerName), tcpServerPort, tcpTimeOut_ms);
    // If connected : we update the display and the state
    if (connectionSucceeded)
    {
        String infoString = "OK, TCP Socket connected to " + String(tcpServerName) + " on port " + String(tcpServerPort);
        this->mainComponent->backLabel->setText(infoString, NotificationType::sendNotification);
        MonitoringServer::SendLog(infoString.toStdString());
        Timer::callAfterDelay(1000,
                              [this] { this->nextExperimentStep(); });
    }
    // Sinon: on refait un tour de cette fonction !!
    else
    {
        // after a delay (connectToSocket is not blocking for long)
        juce::Timer::callAfterDelay(tcpTimeOut_ms,
                                    [this] {this->tryConnectSocketAndContinueExpe();});
    }
}

void OSCRecorder::nextExperimentStep()
{
    // if connection was lost : check for normal preset, or questions
    if (hasConnectionBeenLostDuringLastStep)
    {
        // interactive experimet preset ? if yes, we go to the next preset
        if (ExperimentStateUtils::IsInteractiveExperimentState(stateBeforeConnectionLost))
            nextPreset();
        // else if questions : they MUST be answered, even if we lsot the connection
        // (also true for intro/other displays...)
        else
            changeState(stateBeforeConnectionLost);
    }
    // connection was not not lost : next preset if not the very last one
    else
    {
        nextPreset();
    }
    
    // on continue. S'il y avait eu déconnection, on la suppose maintenant gérée
    hasConnectionBeenLostDuringLastStep = false;
}

void OSCRecorder::nextPreset()
{
    // last preset : final questions
    if (currentPresetStep >= (int)ExperimentPresetsCount - 1)
    {
        changeState(ExperimentState::FinalQuestionsDisplayed);
    }
    // or just the next preset
    else
    {
        currentPresetStep++;
        changeState(ExperimentState::ReadyToListen);
    }
}

void OSCRecorder::startRecording()
{
    // recording always start after the scene is selected
    // because of the TCP connection : we are sure that the scene is selected !
    // but delays might remain...
    oscRealtimeListener->StartRecording();
    
    startTimer(10); /// 100 Hz de fréquence de scrutation du ring buffer
}
void OSCRecorder::stopRecording()
{
    oscRealtimeListener->StopRecording();
    
    stopTimer();
    // on doit ensuite vider les derniers coefficients qui auraient pu arriver....
    timerCallback(); // last callback forced
    
    // tri dès maintenant car on sauvegarde après chaque preset
    presets[presetRandomIdx[currentPresetStep]]->SortSamples();
}


/// ========================================================================
/// ========================================================================
/// =====================  STATE MANAGEMENT FUNCTION  ======================
/// ========================================================================
/// ========================================================================
void OSCRecorder::changeState(ExperimentState newState)
{
    bool stateChanged = (state != newState);
    
    // Pre-change processing
    // OSC control messages are sent here (as soon as possible)
    // OSC component manages its buttons... but the questions component must be (de)actived
    String infoString;
    switch(newState)
    {
        case ExperimentState::NotInitialized:
            break;
        
        case ExperimentState::ConnectionLost:
            reaperController->Stop();
            reaperController->SetTrackSolo_usingMutes(-1);
            if (state == ExperimentState::SearchingPreset)
                stopRecording();
            mainComponent->SetOneGuiComponentVisible(nullptr);
            infoString = "Connection lost! TCP Socket will try to re-connect to " + String(tcpServerName) + " on port " + String(tcpServerPort);
            displayError(infoString);
            break;
            
        case ExperimentState::WaitingForTcpServerConnection:
            if (! stateChanged) // we do not re-display... not to fill std::cout
                break;
            mainComponent->SetOneGuiComponentVisible(nullptr);
            infoString = "TCP Socket trying to connect to " + String(tcpServerName) + " on port " + String(tcpServerPort) + "...";
            displayError(infoString);
            break;
            
        case ExperimentState::IntroDescriptionDisplayed:
            if (GetLanguage().compare("FR") == 0)
                mainComponent->oscRecorderIntroComponent->SetMainText(BinaryData::Recorder_Intro_FR_txt);
            else
                mainComponent->oscRecorderIntroComponent->SetMainText(BinaryData::Recorder_Intro_EN_txt);
            mainComponent->SetOneGuiComponentVisible(mainComponent->oscRecorderIntroComponent.get());
            mainComponent->SetTotalScore(-1); // to hide the score label
            break;
            
        case ExperimentState::PostTrialDescriptionDisplayed:
            if (GetLanguage().compare("FR") == 0)
                mainComponent->oscRecorderIntroComponent->SetMainText(BinaryData::Recorder_AfterTrial_FR_txt);
            else
                mainComponent->oscRecorderIntroComponent->SetMainText(BinaryData::Recorder_AfterTrial_EN_txt);
            mainComponent->SetOneGuiComponentVisible(mainComponent->oscRecorderIntroComponent.get());
            break;
            
        case ExperimentState::InitialQuestionsDisplayed:
            mainComponent->SetOneGuiComponentVisible(mainComponent->firstUserQuestionsComponent.get());
            break;
            
        case ExperimentState::FinalQuestionsDisplayed:
            mainComponent->SetOneGuiComponentVisible(mainComponent->finalUserQuestionsComponent.get());
            break;
            
            // Préparation pour Écoute : on affiche une scène vide, puis
            // on active la bonne track de Reaper
        case ExperimentState::ReadyToListen:
            selectNewMiemScene(true); // empty scene
            // on sélectionne la track EXPERIMENT (pas la référence)
            // pas de délai car le white noise a dû s'arrêter bien avant
            reaperController->SetTrackSolo_usingMutes(presets[presetRandomIdx[currentPresetStep]]->GetReaperTrackNumber(true),
                                                      true ); // full reset on every new cycle
            
            mainComponent->SetOneGuiComponentVisible(nullptr);
            break;
            
            // Listening : tout est prêt, on fait juste play dans Reaper
        case ExperimentState::Listening:
            reaperController->RestartAndPlay(presets[presetRandomIdx[currentPresetStep]]->GetTempo());
            break;
            
            // Au "ready to search" :
            // on STOP REAPER, on sélectionne déjà la bonne track, mais on
            // n'affiche pas encore la scène MIEM
        case ExperimentState::ReadyToSearchPreset:
            reaperController->Stop();
            // on sélectionne la track EXPERIMENT (pas la référence)
            // avec un petit délai pour que Reaper s'en sorte bien....
            Timer::callAfterDelay(SearchAutoTriggerDelay_s / 2, [this] {
            reaperController->SetTrackSolo_usingMutes(presets[presetRandomIdx[currentPresetStep]]->GetReaperTrackNumber(false));
            });
            break;
            
            // Au lancement de la recherche : on lance la scène MIEM d'abord
            // Puis : la track Reaper est déjà
            // sélectionnée, il ne reste qu'à faire PLAY
        case ExperimentState::SearchingPreset:
            reaperController->RestartAndPlay(presets[presetRandomIdx[currentPresetStep]]->GetTempo());
            selectNewMiemScene(); // actual playable scene
            currentResearchStartTimePt = MiemClock::now();
            researchDurationTimer->startTimer(ResearchTimeMax_ms);
            startRecording();
            break;
            
            // Fin de la recherche : on coupe Reaper (mute all)
            // et on replace la scène vide,
            // pour alors jouer du bruit blanc (tentative suppression ASM)
        case ExperimentState::FinishedSearchingPreset:
            reaperController->Stop();
            selectNewMiemScene(true);
            stopRecording();
            // security save
            saveEverythingToFiles(currentPresetStep); // next step pas encore demandée
            // score display update
            recorderComponent.SetPerformance(presets[presetRandomIdx[currentPresetStep]]
                                             -> ComputePerformance());
            // WHITE NOISE starts after a short delay
            Timer::callAfterDelay(WhiteNoiseStartDelay_ms/2,
                                  [this] {
            reaperController->SetTrackSolo_usingMutes(ReaperWhiteNoiseTrackNumber);
                                  });
            Timer::callAfterDelay(WhiteNoiseStartDelay_ms,
                                  [this] {
              reaperController->RestartAndPlay((float)ReaperWhiteNoiseTrackBpm);
                                  });
            Timer::callAfterDelay(delayAfterFinished_ms - WhiteNoisePrematureEnd_ms,
                                  [this] {
              reaperController->Stop(); // to end the white noise
                                  });
            break;
            
        case ExperimentState::Finished:
            mainComponent->finalUserQuestionsComponent->setVisible(false);
            mainComponent->backLabel->setText(TRANS("Thank you for participating!"), NotificationType::sendNotification);
            computeAndDisplayTotalScore();
            break;
            
        case ExperimentState::ExperimentStatesCount:
        case ExperimentState::FirstExperimentState:
            assert(false); // these states are utility state, and must not be used...
            break;
    }
    
    // Actual change of state
    state = newState;
    
    // Display updates (if state actually changed)
    if (stateChanged)
    {
        // Experiment state : full display (with preset #)
        if (ExperimentStateUtils::IsInteractiveExperimentState(state))
        {
            if (currentPresetStep < ExperimentPresetsCount) // si pas encore fini
            {
                std::stringstream logStream;
                logStream << "[Interactive experiment in progress...] " << ExperimentStateUtils::GetName(state) << " #" << currentPresetStep << " (" << presets[presetRandomIdx[currentPresetStep]]->GetName() << ")";
                MonitoringServer::SendLog(logStream.str());
            }
            recorderComponent.DisplayNewState(state, currentPresetStep, ExperimentPresetsCount);
        }
        // other state : simple name display
        else
        {
            std::stringstream logStream;
            logStream << "[Current experiment state: ] " <<  ExperimentStateUtils::GetName(state);
            MonitoringServer::SendLog(logStream.str());
            recorderComponent.DisplayNewState(state, currentPresetStep, ExperimentPresetsCount);
        }
    }
    
    // Post-updates processing
    if (state == ExperimentState::FinishedSearchingPreset)
    {
        // special behavior for special steps
        if (currentPresetStep == -1) // end of trial
        {
            Timer::callAfterDelay(delayAfterFinished_ms, [this]
            { this->changeState(ExperimentState::PostTrialDescriptionDisplayed); } );
        }
        // final (étape différente selon que l'expérience soit une courte/démo ou non)
        else if ( ((!ShortExperiment) && (currentPresetStep >= (ExperimentPresetsCount - 1)))
                 || (ShortExperiment && (currentPresetStep >= (ShortExperimentPresetsCount - 1)) ) )
        {
            reaperController->SetTrackSolo_usingMutes(-1);
            // will trigger an event, but after this one is treated...
            // not very robust... But should be ok for the experiment....
            Timer::callAfterDelay(delayAfterFinished_ms, [this]
                                { currentPresetStep++; // goes to an unvalid value
                                this->changeState(ExperimentState::FinalQuestionsDisplayed); } );
        }
        else // usual case : next step
        {
            Timer::callAfterDelay(delayAfterFinished_ms,
                                  [this] {
                                      this->nextExperimentStep(); // without delay
                                  });
        }
    }
    else if (state == ExperimentState::Listening)
        Timer::callAfterDelay(listeningTime_ms,
                              [this] {
                                  changeState(ExperimentState::ReadyToSearchPreset);
                              });
}

void OSCRecorder::selectNewMiemScene(bool selectEmptyScene)
{
    if (selectEmptyScene)
        selectMiemControllerScene(0);
    else
    {
        auto& currentPreset = presets[presetRandomIdx[currentPresetStep]];
        selectMiemControllerScene(currentPreset->GetMiemSceneIndex());
    }
}







// ================== periodic updates =============================
void OSCRecorder::timerCallback()
{
    // OSC buffer emptying
    presets[presetRandomIdx[currentPresetStep]]->AddSamples(oscRealtimeListener->GetBufferedSamples());
    
    // Graphical forced update
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(MiemClock::now() - currentResearchStartTimePt).count();
    recorderComponent.UpdateRemainingTimeSlider((double)(ResearchTimeMax_ms - duration_ms) / 1000.0,
                                                (double)(ResearchTimeMax_ms) / 1000.0);
}
void OSCRecorder::AttributeTimerCallback(juce::Timer* timer)
{
    if (timer == researchDurationTimer.get())
    {
        recorderComponent.simulateClickOnDisplayedButton();
    }
    else
        assert(false); // no other timer known at the moment...
}






// ================== Events froms View =============================

void OSCRecorder::OnButtonClicked(ExperimentState requestedState)
{
    switch(requestedState)
    {
            // If asks for listening or search : direct OK
        case ExperimentState::Listening:
            changeState(requestedState);
            break;
        
        case ExperimentState::SearchingPreset:
            changeState(requestedState);
            break;
            
            // If finished : changeState will check itself
        case ExperimentState::FinishedSearchingPreset:
            changeState(requestedState);
            break;
            
        default:
            assert(false);
            break;
    }
}


void OSCRecorder::OnIntroFinished(OSCRecorderIntroComponent* /*sender*/)
{
    if (state == ExperimentState::IntroDescriptionDisplayed)
        changeState(ExperimentState::InitialQuestionsDisplayed);
    else if (state == ExperimentState::PostTrialDescriptionDisplayed)
        nextExperimentStep();
    else
        assert(false); // unplanned state...
}
void OSCRecorder::OnFirstQuestionsAnswered(UserQuestions* sender)
{
    // Save before the init screen is being displayed
    // (for data consistency before mode change)
    saveEverythingToFiles(currentPresetStep); // valeur non-valide, trop négative au départ
    
    nextExperimentStep();
}
void OSCRecorder::OnFinalQuestionsAnswered(UserFinalQuestions* sender)
{
    // Save after the finish screen is being displayed
    // // (for data consistency before mode change)
    saveEverythingToFiles(currentPresetStep - 1); // car valeur invalide
    
    changeState(ExperimentState::Finished);
}
bool OSCRecorder::OnQuitRequest()
{
    bool waitingToGetBackToInitalState = hasConnectionBeenLostDuringLastStep
    && (state == ExperimentState::WaitingForTcpServerConnection && stateBeforeConnectionLost == ExperimentState::IntroDescriptionDisplayed);
    
    if (state == ExperimentState::Finished
        || state == ExperimentState::NotInitialized
        || state == ExperimentState::IntroDescriptionDisplayed
        || state == ExperimentState::InitialQuestionsDisplayed
        || waitingToGetBackToInitalState)
        return true;
    else
        return false;
}




// ================== Events to View =============================

void OSCRecorder::displayError(String errorMsg)
{
    mainComponent->backLabel->setText(errorMsg, NotificationType::sendNotification);
    MonitoringServer::SendLog(errorMsg.toStdString());
}
void OSCRecorder::computeAndDisplayTotalScore()
{
    int totalScore = 0;
    for (size_t i=0 ; i < (presets.size() - TrialPresetsCount) ; i++)
    {
        if (presets[i]->GetPerformance())
            totalScore += (int) std::round(presets[i]->GetPerformance() * 100.0);
    }
    mainComponent->SetTotalScore(totalScore);
}





// ================== Events from Network =============================
void OSCRecorder::OnConnectionLost()
{
    hasConnectionBeenLostDuringLastStep = true;
    stateBeforeConnectionLost = state; // current state backup
    
    // Brutal app mode change (will display the error)
    changeState(ExperimentState::ConnectionLost);
    
    // Ensuite, si preset réel (pas trial), sauvegarde de l'état
    // L'expérience en cours sera supprimée... Tant pis !
    // On passe à la suivante
    if (currentPresetStep >= 0) // check for "not trial and not question"
        presets[presetRandomIdx[currentPresetStep]]->SetIsValid(false);
    
    // Enfin préparation pour la re-connection

    // Re-connection (will change the app mode) (après un court délai)
    juce::Timer::callAfterDelay(tcpTimeOut_ms, [this] {this->tryConnectSocketAndContinueExpe();} );
}


// ================== Events to MIEM Controller via network ==================
bool OSCRecorder::selectMiemControllerScene(int sceneIndex)
{
    MonitoringServer::SendLog("[OSC -> MIEM Controller] : affichage scène "
                                  + boost::lexical_cast<std::string>(sceneIndex));
    
    Miam::AsyncParamChange paramChange(Miam::AsyncParamChange::Scene);
    paramChange.Id1 = sceneIndex;
    Miam::InterprocessControlBlock controlBlock(paramChange);
    
    bool msgSent = tcpConnection->sendMessage(controlBlock);
    
    // Message lost !!!! We must try to get the connection back...
    if (!msgSent)
        assert(false);
    
    return true;
}



// ================== XML and DATA save and Read =============================
void OSCRecorder::createNewDataFiles()
{
    std::string asciiStartTime;
    // addition of zeros to get constant file name sizes
    int month = startSystemTime.getMonth() + 1; // !! month index, from 0 to 11
    auto monthStr = boost::lexical_cast<std::string>(month);
    if (month < 10)
        monthStr = "0" + monthStr;
    auto dayStr = boost::lexical_cast<std::string>(startSystemTime.getDayOfMonth());
    if (startSystemTime.getDayOfMonth() < 10)
        dayStr = "0" + dayStr;
    auto hoursStr = boost::lexical_cast<std::string>(startSystemTime.getHours());
    if (startSystemTime.getHours() < 10)
        hoursStr = "0" + hoursStr;
    auto minutesStr = boost::lexical_cast<std::string>(startSystemTime.getMinutes());
    if (startSystemTime.getMinutes() < 10)
        minutesStr = "0" + minutesStr;
    // building of the time string
    asciiStartTime += boost::lexical_cast<std::string>(startSystemTime.getYear()) + "-";
    asciiStartTime += monthStr + "-" + dayStr + "_" + hoursStr + "h" + minutesStr;
    
    // Exp name, 4 digits, in front of the time string
    std::string expName = boost::lexical_cast<std::string>(currentExperimentUid);
    if (currentExperimentUid < 1000)
    {
        expName = "0" + expName;
        if (currentExperimentUid < 100)
        {
            expName = "0" + expName;
            if (currentExperimentUid < 10)
                expName = "0" + expName;
        }
    }
    expName = "Exp" + expName;
    
    // Files' names
    dataFilePath = filesSavingPath + expName + /*"_" + asciiStartTime +*/ "_data.csv";
    infoFilePath = filesSavingPath + expName + /*"_" + asciiStartTime +*/ "_info.xml";
    mainComponent->expeLabel->setText(dataFilePath, NotificationType::dontSendNotification);
    mainComponent->expeLabel2->setText(infoFilePath, NotificationType::dontSendNotification);
    MonitoringServer::SendLog("--------------- FILE NAMES ------------------");
    MonitoringServer::SendLog(dataFilePath);
    MonitoringServer::SendLog(infoFilePath);
    MonitoringServer::SendLog("--------------- FILE NAMES ------------------");
    
    // First save, to check the write rights on this machine
    saveEverythingToFiles(currentPresetStep); // valeur non-valide
    
    // If everything could be saved.... Then we update now the count of experiments already done
    // (in the main "general data file" common for all subjects)
    parametersTree->clear();
    bptree::ptree experimentsInnerTree;
    // configuration, settings, generic counters
    experimentsInnerTree.put("count", currentExperimentUid+1);
    experimentsInnerTree.put("tcp.<xmlattr>.ip", tcpServerName);
    experimentsInnerTree.put("tcp.<xmlattr>.port", tcpServerPort);
    experimentsInnerTree.put("osc.udp.<xmlattr>.port", udpOscPort);
    // We also rewrite parameters of the synths
    bptree::ptree synthsInnerTree;
    assert( (ExperimentPresetsCount%2) == 0); // 2 times more presets than synths for the actual experiment presets
    synthsInnerTree.put("<xmlattr>.total_count", (ExperimentPresetsCount/2) + TrialSynthsCount);
    synthsInnerTree.put("<xmlattr>.trials_count", TrialSynthsCount);
    assert(ExperimentPresetsCount%2 == 0);
    for (int i=(-TrialSynthsCount); i < (ExperimentPresetsCount/2) ; i++)
    {
        auto currentPreset = MiemExpePreset(i, false);
        bptree::ptree synthTree; // no lol
        synthTree.put("<xmlattr>.id", i);
        synthTree.put("<xmlattr>.name", currentPreset.GetName());
        synthTree.add_child("parameters", *(currentPreset.GetTargetValuesInnerTree()));
        synthsInnerTree.add_child("synth", synthTree);
    }
    experimentsInnerTree.add_child("synths", synthsInnerTree);
    parametersTree->add_child("experiments", experimentsInnerTree);
    // write, and error triggering as usual
    bptree::xml_writer_settings<std::string> xmlSettings(' ', 4);
    std::string parametersPath = filesSavingPath;
    parametersPath += experimentGeneralDataFile;
    try {
        bptree::write_xml(parametersPath, *parametersTree, std::locale(), xmlSettings);
    }
    catch (bptree::xml_parser::xml_parser_error& e) {
        String errorStr = "Cannot write " + String(parametersPath) + ". Please check parameters and restart experiment.";
        displayError(errorStr); // won't be graphically displayed... console only
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
        throw std::runtime_error(errorStr.toStdString());
    }
}
void OSCRecorder::saveEverythingToFiles(int lastPresetStepToSave)
{
    // Save of INFO
    bptree::ptree experimentInnerTree; // does not contain the <subject> tag itself
    experimentInnerTree.put("<xmlattr>.UID", currentExperimentUid);
    experimentInnerTree.put("<xmlattr>.is_valid", true); // might be put to false after the experiment...
    // Date et heure de départ
    bptree::ptree startTimeTree;
    startTimeTree.put("<xmlattr>.year", startSystemTime.getYear());
    startTimeTree.put("<xmlattr>.month", startSystemTime.getMonth() + 1);
    startTimeTree.put("<xmlattr>.month_name", startSystemTime.getMonthName(false).toStdString());
    startTimeTree.put("<xmlattr>.day", startSystemTime.getDayOfMonth());
    startTimeTree.put("<xmlattr>.day_name", startSystemTime.getWeekdayName(false));
    startTimeTree.put("<xmlattr>.hours", startSystemTime.getHours());
    startTimeTree.put("<xmlattr>.minutes", startSystemTime.getMinutes());
    experimentInnerTree.add_child("start_date", startTimeTree);
    // Questions (même celles qui ne sont pas encore répondues
    experimentInnerTree.add_child("first_questions",
                                    *(mainComponent->firstUserQuestionsComponent->GetQuestionsBPTree()));
    experimentInnerTree.add_child("final_questions",
                                    *(mainComponent->finalUserQuestionsComponent->GetQuestionsBPTree()));
    // Infos sur les presets
    bptree::ptree testedPresetInnerTree;
    testedPresetInnerTree.put("<xmlattr>.count", lastPresetStepToSave + TrialPresetsCount + 1);
    // Liste de tous les presets jusqu'à maintenant
    // y compris ceux de test
    for (int i=(-TrialPresetsCount) ; i<=lastPresetStepToSave ; i++)
        testedPresetInnerTree.add_child("preset", *(presets[presetRandomIdx[i]]->GetInfoTree()));
    experimentInnerTree.add_child("tested_presets", testedPresetInnerTree);
    // Actual XML/CSV data writing into files
    bptree::xml_writer_settings<std::string> xmlSettings(' ', 4);
    try {
        bptree::ptree experimenTree;
        experimenTree.add_child("experiment", experimentInnerTree);
        bptree::write_xml(infoFilePath, experimenTree, std::locale(), xmlSettings);
    }
    catch (bptree::xml_parser::xml_parser_error& e) {
        String errorStr = "Cannot write " + String(infoFilePath) + ". Please check parameters and restart experiment.";
        displayError(errorStr); // won't be graphically displayed... console only
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
        throw std::runtime_error(errorStr.toStdString());
    }
    
    // Save of DATA into the CSV file (string output preparation)
    std::ofstream streamFile(dataFilePath, std::ios_base::out);
    if (! streamFile.is_open())
    {
        String errorStr = "Cannot write " + String(dataFilePath) + ". Please check parameters and restart experiment.";
        displayError(errorStr); // won't be graphically displayed... console only
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
        throw std::runtime_error(errorStr.toStdString());
    }
    streamFile << MiemExpePreset::GetCSVFileHeader();
    // on écrit chaque preset (y compris les essais)
    for (int i=(-TrialPresetsCount) ; i<=lastPresetStepToSave ; i++)
        streamFile << (*(presets[presetRandomIdx[i]]->GetSortedSamples_CSV()));
}

void OSCRecorder::getExperimentParametersFromXmlFile()
{
    std::string xmlExpeFilePath = filesSavingPath;
    xmlExpeFilePath += experimentGeneralDataFile;
    
    // Chargement du fichier de config. SI impossible : on attent un peu puis on quitte...
    parametersTree = std::make_shared<bptree::ptree>();
    try {
        bptree::read_xml(xmlExpeFilePath, *parametersTree);
    }
    catch(bptree::ptree_error &e) {
        String errorStr = TRANS("Cannot read the file '") + String(xmlExpeFilePath) + TRANS("' for configuration parameters. Experiment aborted.");
        displayError(errorStr);
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10));
        throw std::runtime_error(errorStr.toStdString());
    }
    
    // Chargement des paramètres du fichier
    try {
        // uid of current expe is the current expe count
        currentExperimentUid = parametersTree->get<int>("experiments.count");
        
        tcpServerName = parametersTree->get<std::string>("experiments.tcp.<xmlattr>.ip");
        tcpServerPort = parametersTree->get<int>("experiments.tcp.<xmlattr>.port");
        
        udpOscPort = parametersTree->get<int>("experiments.osc.udp.<xmlattr>.port");
    }
    catch(bptree::ptree_error &e) {
        String errorStr = TRANS("Wrong value or XML parsing error in file '") + String(xmlExpeFilePath) + TRANS("'. Experiment aborted.");
        displayError(errorStr); // won't be graphically displayed... console only
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
        throw std::runtime_error(errorStr.toStdString());
    }
}


