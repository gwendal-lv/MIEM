/*
  ==============================================================================

    ControlModel.cpp
    Created: 28 Nov 2016 11:02:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "JuceHeader.h"

#include "ControlModel.h"
#include "MiamOscSender.hpp"

#include "ControlPresenter.h"


using namespace Miam;



// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
void ControlModel::SetIsMasterGainEnabled(bool isEnabled)
{
    masterGainEnabled = isEnabled;
    if (!masterGainEnabled)
        masterGain = 1.0;
}
InterpolationType ControlModel::GetInterpolatorType_Atomic ()
{
    return interpolator->GetType_Atomic();
}
std::shared_ptr<ControlStateSender<double>> ControlModel::GetStateSender(size_t index)
{
    if ( index == 0 )
        return stateSenders[0];
    else
        throw std::logic_error("State sender #0 is the only one available at the moment.");
}

size_t ControlModel::GetOutputsCount()
{
    return interpolator->GetOutputsCount();
    
}




// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
ControlModel::ControlModel(ControlPresenter* presenter_,
                           bool startUpdateThread,
                           double updateFrequency_Hz)
:
PeriodicUpdateThread("Control Model updater"), // base class

presenter(presenter_),
commonStartTimePt( presenter->GetCommonTimePoint() )
{
    // Choice of interpolation type
    interpolator = std::make_shared<StatesInterpolator<double>>(InterpolationType::None);
    
    // Définition de la fréquence de fonctionnement
    updateThreadF_Hz = updateFrequency_Hz;
    updateThreadT_us = (int)std::round(1000000.0/updateFrequency_Hz);
    continueUpdate = false;
    
    // Communication initialization
    stateSenders.push_back( std::make_shared<MiamOscSender<double>>() );
    
    // Begin of update thread, only if asked (default is false)
    if (startUpdateThread)
    {
        // Using a c++11 lambda function for class member calling
        // Warning : polymorphic functions links are not properly setup yet...
        updateThread = std::thread( [this] {this->update();} );
    }
}


ControlModel::~ControlModel()
{
    if (threadHasBeenLaunched)
    {
        // On attend le join propre du thread avant la fermeture
        continueUpdate = false;
        updateThread.join();
    }
}


// = = = = = = = = = = Periodic updates = = = = = = = = = =

void ControlModel::update()
{
    threadHasBeenLaunched = true;
    
    while(continueUpdate)
    {
        wasSomethingUpdated = false;
        // virtual function, to be overriden by child classes
        onUpdateStarts();
        
        // Boolean to manage the very special case of 2 consecutive
        // STOP then PLAY messages in the lock-free queue
        bool replayDirectlyAfterStop = false;
        
        
        updateThreadMeasurer.OnNewFrame();
        
        // Infos de performance -> à passer dans un fichier texte pour ne pas perturber la mesure...
        // (écriture en asynchrone dans un thread séparé)
#ifdef __MIAM_DEBUG
        if (updateThreadMeasurer.IsFreshAverageAvailable())
            DBG(updateThreadMeasurer.GetInfo());
#endif
        
        // Récupération de toutes les données les + à jour
        AsyncParamChange lastParamChange;
        AsyncParamChange lastParamChange2;
        AsyncParamChange localParamChange;
        while (presenter->TryGetAsyncParamChange(lastParamChange))
        {
            switch (lastParamChange.Type)
            {
                case AsyncParamChange::Excitement :
                    interpolator->OnNewExcitementAmount((size_t) lastParamChange.Id1,
                                                        (uint64_t) lastParamChange.Id2,
                                                        lastParamChange.DoubleValue);
                    break;
                    
                case AsyncParamChange::MasterVolume :
                    masterGain = lastParamChange.DoubleValue;
                    break;
                    
                case AsyncParamChange::Play :
                    onPlay();
                    break;
                    
                case AsyncParamChange::Stop :
                    playState = AsyncParamChange::Stop;
                    std::cout << "[Modèle] STOP" << std::endl;
                    interpolator->OnStop();
                    // Après le stop, il faut peut-être envoyer des données
                    wasSomethingUpdated = interpolator->OnDataUpdateFinished();
                    if (wasSomethingUpdated) // vrai si données actualisées
                    {
                        miamOscSender->SendStateModifications(interpolator->GetCurrentInterpolatedState());
                    }
                    // DOUBLE-SIGNAL Confirmation sent back to the presenter
                    // ultra-fast and lock-free
                    presenter->OnModelStopped();
                    // typical using the lock-free queue
                    localParamChange.Type = AsyncParamChange::ParamType::Stopped;
                    SendParamChange(localParamChange);
                    // After confirmation is sent back to the presenter, we force-empty
                    // the lock-free queue.
                    // BUT we have to check for re-play messages in this lock-free queue...
                    // (in case of very short stop->play sequence)
                    while(presenter->TryGetAsyncParamChange(lastParamChange2))
                    {
                        if (lastParamChange2.Type == AsyncParamChange::Play)
                            replayDirectlyAfterStop = true;
                        else if (lastParamChange2.Type == AsyncParamChange::Stop)
                            replayDirectlyAfterStop = false;
                    }
                    // Remark : no need to wait for another ack from the Presenter.
                    // If it isn't playing, it does not send lock-free data.
                    break;
                    
                default :
                    break;
            }
        }
        
        // - - - - - SI ON EST EN TRAIN DE JOUER - - - - -
        // Alors on peut faire pas mal de choses. Si on ne joue pas par contre... Il
        // faut vraiment éviter de faire des trucs parce que Model et Presenter communiquent
        // en single threaded direct
        if ( playState == AsyncParamChange::Play )
        {
            // Envoi de la nouvelle matrice, si nécessaire
            wasSomethingUpdated = interpolator->OnDataUpdateFinished(masterGainEnabled, masterGain);
            if (wasSomethingUpdated)
            {
                miamOscSender->SendStateModifications(interpolator->GetCurrentInterpolatedState());
            }
            else if (continuousBackgroundBlobMatrixRefresh)
            {
                if ( (refreshFramesCounter++) >= refreshPeriod_frames )
                {
                    miamOscSender->ForceCoeffsBlockRefresh( interpolator->GetCurrentInterpolatedState() );
                    wasSomethingUpdated = true;
                    refreshFramesCounter = 0;
                }
            }
            else if (continuousBackgroundSingleMatrixCoeffRefresh)
            {
                if ( (refreshFramesCounter++) >= refreshPeriod_frames )
                {
                    miamOscSender->ForceSend1MatrixCoeff( interpolator->GetCurrentInterpolatedState(),
                                                         GetSessionPurpose());
                    wasSomethingUpdated = true;
                    refreshFramesCounter = 0;
                }
            }
        }
        // fin de : - - - - - SI ON EST EN TRAIN DE JOUER - - - - -
        
        
        // virtual function, to be overriden by child classes
        onUpdateFinished();
        
        // in case of very quick stop/play messages in the lock-free queue
        if (replayDirectlyAfterStop)
            onPlay();
        
        // Sleep forcé uniquement si on est assez loin de la période souhaitée
        // On prend 1.5 ms de marge pour la réaction de l'OS en sortie de sleep (TOTALEMENT ARBITRAIRE !)
        const int sleepDelayMargin_us = std::max(updateThreadT_us - 500, 1500);
        if (updateThreadMeasurer.GetElapsedTimeSinceLastNewFrame_us()
            < (updateThreadT_us - sleepDelayMargin_us))
            std::this_thread::sleep_for(std::chrono::microseconds(updateThreadT_us
                                                                  - updateThreadMeasurer.GetElapsedTimeSinceLastNewFrame_us() ) );
    }
}


// = = = = = = = = Internal events = = = = = = = =
void ControlModel::onPlay()
{
    playState = AsyncParamChange::Play;
    std::cout << "[Modèle] PLAY (interpolation de type '" << InterpolationTypes::GetInterpolationName(GetInterpolator()->GetType()) << "')" << std::endl;
    interpolator->OnPlay();
    
    // Confirmation sent back to the presenter
    AsyncParamChange paramChange(AsyncParamChange::ParamType::Playing);
    SendParamChange(paramChange);
}

// = = = = = = = = Events from Presenter = = = = = = = =
bool ControlModel::ResetOscConfigurationFromTree(bptree::ptree& tree)
{
    try {
        // true, send exception if missing data
        getMainSpatSender()->SetConfigurationFromTree(tree, true);
    }
    catch( bptree::ptree_error& ) {
        DBG("[Model - Control] Cannot reset OSC configuration... bptree data is not valid");
        return false;
    }
    
    return getMainSpatSender()->TryConnect();
}


// = = = = = = = = Property tree (for XML) import/export = = = = = = = =
std::shared_ptr<bptree::ptree> ControlModel::GetConfigurationTree()
{
    // Interpolator general config
    auto configurationTree = interpolator->GetConfigurationTree();
    // Individual senders
    bptree::ptree senderChildren;
    for (size_t i=0 ; i<stateSenders.size() ; i++)
    {
        auto child = senderChildren.add_child( "sender", *(stateSenders[i]->GetConfigurationTree()) );
        child.put("<xmlattr>.type", stateSenders[i]->GetTypeAsString());
    }
    configurationTree->add_child("senders", senderChildren);
    // Master Gain : enabled or not, with current value
    bptree::ptree masterGainTree;
    masterGainTree.put("<xmlattr>.enabled", masterGainEnabled);
    masterGainTree.put("<xmlattr>.value", masterGain);
    configurationTree->add_child("master_gain", masterGainTree);
    // Final return
    return configurationTree;
}
void ControlModel::SetConfigurationFromTree(bptree::ptree& tree)
{
    try {
        // Interpolator general config
        interpolator->SetConfigurationFromTree(tree);
        // Individual senders
        int sendersCount = 0; // exactly 1 sender allowed at the moment
        for (auto& sender : tree.get_child("senders"))
        {
            if (sendersCount >=1)
                throw XmlReadException("No more than 1 <sender> is allowed");
            
            stateSenders[sendersCount]->SetConfigurationFromTree(sender.second);
        }
    }
    catch (XmlReadException &e) {
        throw XmlReadException("Control settings are not correct: ", e);
    }
    // Master Gain : facultatif, with default values
    // No to break compatibility of mspat files (very recently broken for version 1.0.0 ...)
    masterGain = tree.get<double>("master_gain.<xmlattr>.value", 1.0); // default neutral value
    masterGainEnabled = tree.get<bool>("master_gain.<xmlattr>.enabled", false); // default false value
}


