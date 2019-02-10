/*
  ==============================================================================

    OSCRecorder.cpp
    Created: 7 Feb 2019 9:09:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <thread>
#include <algorithm>

#include "boost/lexical_cast.hpp"
//#include "boost/property_tree/ptree.hpp"

#include "OSCRecorder.h"
#include "OSCRecorderComponent.h"

#include "MainComponent.h"

// The constructor will init the experiment with random preset values
OSCRecorder::OSCRecorder(MainComponent* _mainComponent)
:
mainComponent(_mainComponent),
recorderComponent(*(_mainComponent->oscRecorderComponent.get()))
{
    recorderComponent.SetRecorderManager(this);
    mainComponent->firstUserQuestionsComponent->SetUserQuestionsManager(this);
    mainComponent->finalUserQuestionsComponent->SetUserQuestionsManager(this);
    
    // Setup of TCP control connection to the server inside MIEM Controller
    tcpConnection = std::make_shared<OSCRecorderConnection>(*this);
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
    stateBeforeConnectionLost = ExperimentState::InitialQuestionsDisplayed; // forced fake state
    
    if (presetsList.size() > 0)
    {
        std::cout << "REINITIALISATION SANS SAUVEGARDE DES DONNEES !!" << std::endl;
        assert(false);
    }
    
    // Random presets are planned from the beginning
    // We must have all indexes in the vector
    presetsList.clear();
    presetsList.reserve(ExperimentPresetsCount);
    for (int i=0 ; i<ExperimentPresetsCount ; i++)
    {
        // pas d'aléatoire pour l'instant !!
        // UID et index sont les mêmes...
        presetsList.push_back(MiemExpePreset(i, i));
    }
    
    
    currentPresetIndex = -(int)TrialPresetsCount - 1;
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
        DBG(infoString);
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
    if (currentPresetIndex >= (int)ExperimentPresetsCount - 1)
    {
        changeState(ExperimentState::FinalQuestionsDisplayed);
    }
    // or just the next preset
    else
    {
        currentPresetIndex++;
        changeState(ExperimentState::ReadyToListen);
    }
}


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
            mainComponent->firstUserQuestionsComponent->setVisible(false);
            mainComponent->finalUserQuestionsComponent->setVisible(false);
            infoString = "Connection lost! TCP Socket will try to re-connect to " + String(tcpServerName) + " on port " + String(tcpServerPort);
            displayError(infoString);
            break;
            
        case ExperimentState::WaitingForTcpServerConnection:
            if (! stateChanged) // we do not re-display... not to fill std::cout
                break;
            mainComponent->firstUserQuestionsComponent->setVisible(false);
            mainComponent->finalUserQuestionsComponent->setVisible(false);
            infoString = "TCP Socket trying to connect to " + String(tcpServerName) + " on port " + String(tcpServerPort) + "...";
            displayError(infoString);
            break;
            
        case ExperimentState::InitialQuestionsDisplayed:
            mainComponent->firstUserQuestionsComponent->setVisible(true);
            mainComponent->finalUserQuestionsComponent->setVisible(false);
            break;
            
        case ExperimentState::FinalQuestionsDisplayed:
            mainComponent->firstUserQuestionsComponent->setVisible(false);
            mainComponent->finalUserQuestionsComponent->setVisible(true);
            break;
            
        case ExperimentState::ReadyToListen:
            mainComponent->firstUserQuestionsComponent->setVisible(false);
            mainComponent->finalUserQuestionsComponent->setVisible(false);
            break;
            
        case ExperimentState::Listening:
            break;
            
        case ExperimentState::ReadyToSearchPreset:
            break;
            
        case ExperimentState::SearchingPreset:
            selectNewScene();
            break;
            
        case ExperimentState::FinishedSearchingPreset:
            if (currentPresetIndex >= ((int)ExperimentPresetsCount - 1))
            {
                // will trigger an event, but after this one is treated...
                // not very robust... But should be ok for the experiment....
                juce::Timer::callAfterDelay(delayAfterFinished_ms, [this]
                {   currentPresetIndex++; // goes to an unvalid value
                    this->changeState(ExperimentState::FinalQuestionsDisplayed); } );
            }
            break;
            
        case ExperimentState::Finished:
            mainComponent->finalUserQuestionsComponent->setVisible(false);
            mainComponent->backLabel->setText(TRANS("Thank you for participating!"), NotificationType::sendNotification);
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
            std::cout << "[Interactive experiment in progress...] " << ExperimentStateUtils::GetName(state) << " #" << currentPresetIndex << std::endl;
            recorderComponent.DisplayNewState(state, currentPresetIndex, ExperimentPresetsCount);
        }
        // other state : simple name display
        else
        {
            std::cout << "[Current experiment state: ] " <<  ExperimentStateUtils::GetName(state) << std::endl;
            recorderComponent.DisplayNewState(state, currentPresetIndex, ExperimentPresetsCount);
        }
    }
    
    // Post-updates processing
    if (state == ExperimentState::FinishedSearchingPreset)
    {
        Timer::callAfterDelay(delayAfterFinished_ms,
                              [this] {
                                  this->nextExperimentStep(); // without delay
                              });
    }
    else if (state == ExperimentState::Listening)
        Timer::callAfterDelay(listeningTime_ms,
                              [this] {
                                  changeState(ExperimentState::ReadyToSearchPreset);
                              });
}

void OSCRecorder::selectNewScene()
{
    std::cout << "DEMANDE OSC DE LA SCENE INDEX #" << currentPresetIndex << std::endl;
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

void OSCRecorder::OnSpaceBarPushed()
{
    // If it is not a double-tap on the space bar...
    
    // Then we jump to the next state
}

void OSCRecorder::OnFirstQuestionsAnswered(UserQuestions* sender)
{
    // Save after before the finish screen is being displayed
    // (for data consistency before mode change)
    saveEverythingToFiles();
    
    nextExperimentStep();
}
void OSCRecorder::OnFinalQuestionsAnswered(UserFinalQuestions* sender)
{
    // Save after the finish screen is being displayed
    // // (for data consistency before mode change)
    saveEverythingToFiles();
    
    changeState(ExperimentState::Finished);
}
bool OSCRecorder::OnQuitRequest()
{
    bool waitingToGetBackToInitalQuestion = hasConnectionBeenLostDuringLastStep
    && (state == ExperimentState::WaitingForTcpServerConnection && stateBeforeConnectionLost == ExperimentState::InitialQuestionsDisplayed);
    
    if (state == ExperimentState::Finished
        || state == ExperimentState::NotInitialized
        || state == ExperimentState::InitialQuestionsDisplayed
        || waitingToGetBackToInitalQuestion)
        return true;
    else
        return false;
}




// ================== Events to View =============================

void OSCRecorder::displayError(String errorMsg)
{
    mainComponent->backLabel->setText(errorMsg, NotificationType::sendNotification);
    DBG(errorMsg);
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
    if (currentPresetIndex >= 0) // check for "not trial and not question"
        presetsList[currentPresetIndex].SetIsValid(false);
    
    // Enfin préparation pour la re-connection

    // Re-connection (will change the app mode) (après un court délai)
    juce::Timer::callAfterDelay(tcpTimeOut_ms, [this] {this->tryConnectSocketAndContinueExpe();} );
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
    dataFilePath = filesSavingPath + expName + "_" + asciiStartTime + "_data.csv";
    infoFilePath = filesSavingPath + expName + "_" + asciiStartTime + "_info.xml";
    mainComponent->expeLabel->setText(dataFilePath, NotificationType::dontSendNotification);
    mainComponent->expeLabel2->setText(infoFilePath, NotificationType::dontSendNotification);
    DBG("--------------- FILE NAMES ------------------");
    DBG(dataFilePath);
    DBG(infoFilePath);
    DBG("--------------- FILE NAMES ------------------");
    
    // First save, to check the write rights on this machine
    saveEverythingToFiles();
    
    // If everything could be saved.... Then we update now the count of experiments already done
    // (in the main "general data file" common for all subjects)
    parametersTree->erase("experiment.count");
    parametersTree->put("experiment.count", currentExperimentUid+1);
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
void OSCRecorder::saveEverythingToFiles()
{
    // Save of INFO
    bptree::ptree questionsChildrenTree; // does not contain the <subject> tag itself
    questionsChildrenTree.put("tested_presets.<xmlattr>.count", std::max(currentPresetIndex+1, 0));
    questionsChildrenTree.add_child("first_questions",
                                    *(mainComponent->firstUserQuestionsComponent->GetQuestionsBPTree()));
    questionsChildrenTree.add_child("final_questions",
                                    *(mainComponent->finalUserQuestionsComponent->GetQuestionsBPTree()));
    // Actual XML data writing into file
    bptree::xml_writer_settings<std::string> xmlSettings(' ', 4);
    try {
        bptree::write_xml(infoFilePath, questionsChildrenTree, std::locale(), xmlSettings);
    }
    catch (bptree::xml_parser::xml_parser_error& e) {
        String errorStr = "Cannot write " + String(infoFilePath) + " or " + String(dataFilePath) + ". Please check parameters and restart experiment.";
        displayError(errorStr); // won't be graphically displayed... console only
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
        throw std::runtime_error(errorStr.toStdString());
    }
    
    // Save of DATA
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
        currentExperimentUid = parametersTree->get<int>("experiment.count");
        
        tcpServerName = parametersTree->get<std::string>("experiment.tcp.<xmlattr>.ip");
        tcpServerPort = parametersTree->get<int>("experiment.tcp.<xmlattr>.port");
    }
    catch(bptree::ptree_error &e) {
        String errorStr = TRANS("Wrong value or XML parsing error in file '") + String(xmlExpeFilePath) + TRANS("'. Experiment aborted.");
        displayError(errorStr); // won't be graphically displayed... console only
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
        throw std::runtime_error(errorStr.toStdString());
    }
}


