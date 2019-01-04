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
        updateThreadMeasurer.OnNewFrame();
        
        // Infos de performance -> à passer dans un fichier texte pour ne pas perturber la mesure...
        // (écriture en asynchrone dans un thread séparé)
#ifdef __MIAM_DEBUG
        if (updateThreadMeasurer.IsFreshAverageAvailable())
            DBG(updateThreadMeasurer.GetInfo());
#endif
        
        // Récupération de toutes les données les + à jour
        AsyncParamChange lastParamChange;
        while (presenter->TryGetAsyncParamChange(lastParamChange))
        {
            switch (lastParamChange.Type)
            {
                case AsyncParamChange::Excitement :
                    interpolator->OnNewExcitementAmount((size_t) lastParamChange.Id1,
                                                        (uint64_t) lastParamChange.Id2,
                                                        lastParamChange.DoubleValue);
                    break;
                    
                case AsyncParamChange::Play :
                    onPlay();
                    break;
                    
                case AsyncParamChange::Stop :
                    playState = AsyncParamChange::Stop;
                    std::cout << "[Modèle] STOP" << std::endl;
                    interpolator->OnStop();
                    // Après le stop, il faut peut-être envoyer des données
                    if (interpolator->OnDataUpdateFinished()) // vrai si données actualisées
                    {
                        miamOscSender->SendStateModifications(interpolator->GetCurrentInterpolatedState());
                    }
                    break;
                    
                default :
                    break;
            }
        }
        
        // - - - - - SI ON EST EN TRAIN DE JOUER - - - - -
        if ( playState == AsyncParamChange::Play )
        {
            // Envoi de la nouvelle matrice, si nécessaire
            bool somethingWasUpdated = interpolator->OnDataUpdateFinished();
            if (somethingWasUpdated)
            {
                miamOscSender->SendStateModifications(interpolator->GetCurrentInterpolatedState());
            }
            else if (continuousBackgroundBlobMatrixRefresh)
            {
                if ( (refreshFramesCounter++) >= refreshPeriod_frames )
                {
                    miamOscSender->ForceCoeffsBlockRefresh( interpolator->GetCurrentInterpolatedState() );
                    refreshFramesCounter = 0;
                }
            }
            else if (continuousBackgroundSingleMatrixCoeffRefresh)
            {
                if ( (refreshFramesCounter++) >= refreshPeriod_frames )
                {
                    miamOscSender->ForceSend1MatrixCoeff( interpolator->GetCurrentInterpolatedState() );
                    refreshFramesCounter = 0;
                }
            }
        }
        // fin de : - - - - - SI ON EST EN TRAIN DE JOUER - - - - -
        
        
        
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
        child.put("<xmattr>.type", stateSenders[i]->GetTypeAsString());
    }
    configurationTree->add_child("senders", senderChildren);
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
}


