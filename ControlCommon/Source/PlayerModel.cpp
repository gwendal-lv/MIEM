/*
  ==============================================================================

    PlayerModel.cpp
    Created: 24 Jan 2017 9:09:12am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <map>
#include <string>

#include "PlayerModel.h"

#include "PlayerPresenter.h"

#include "InterpolationTypes.h"

#if defined(JUCE_MAC) || defined(JUCE_IOS)

    // #include <mach/mach_init.h> // on laisse tomber pour l'instantt...
    // #include <mach/thread_policy.h> // beaucoup trop d'investissement nécessaire !
    // #include <mach/sched.h> // -> pas trouvé !!!!!!!

    #include <pthread.h>

#else // - - - - - autre OS.... - - - - -

#endif


using namespace Miam;


PlayerModel::PlayerModel(PlayerPresenter* presenter_)
:
ControlModel(presenter_, 500.0),
presenter(presenter_) // own private downcasted pointer
{
    playState = AsyncParamChange::Stop;
    
    // Pas très propre... Mais pour l'instant c'est la seule option
    miamOscSender = std::dynamic_pointer_cast<MiamOscSender<double>>(stateSenders[0]);
    if (! miamOscSender )
        throw std::runtime_error("First and only StateSender must be a MiamOscSender at the moment");
    
    // Fin d'Initialisation des autres modules
    presenter->CompleteInitialisation(this);
    
    // Launch of thread, at the specified frequency
    continueUpdate = true;
    // Using a c++11 lambda function for class member calling
    updateThread = std::thread( [this] {this->update();} );
}

PlayerModel::~PlayerModel()
{
    // On attend le join propre du thread avant la fermeture
    continueUpdate = false;
    updateThread.join();
}






// = = = = = = = = = = Periodic updates = = = = = = = = = =

void PlayerModel::setHighThreadPriority()
{
    // Si possible : mise à un niveau de priorité pour le thread de mise à jour
    // -> voire même un autre ordonnancement sous certains OS ?
    
#if defined(JUCE_MAC)
    // On n'utilisera pas les fonctions d'accès direct à l'ordonnanceur MACH, trop complexe...
    // - - Identification des politiques d'ordonnancement POSIX - -
    std::map<int, std::string> policyToString;
    policyToString[SCHED_FIFO] = "FIFO"; // 4
    policyToString[SCHED_RR] = "Round-Robin"; // 2
    policyToString[SCHED_OTHER] = "Other"; // 1
    int requestedPolicy = SCHED_FIFO;
    // - - Choix de la priorité - -
    // Le 2 (c-à-d Round-Robin) est bien précisé dans la doc via terminal macOS...
    int minPriority = sched_get_priority_min(requestedPolicy);
    int maxPriority = sched_get_priority_max(requestedPolicy);
    int requestedPriority = maxPriority;
    if (minPriority == maxPriority)
        std::cout << "[Model] The OS does not provide thread priorities." << std::endl;
    // - - Application de la priorité - -
    struct sched_param initialSchedParam;
    int initialPolicy = 0;
    pthread_getschedparam(pthread_self(), &initialPolicy, &initialSchedParam);
    struct sched_param actualSchedParam;
    actualSchedParam.sched_priority = requestedPriority;
    pthread_setschedparam(pthread_self(), requestedPolicy, &actualSchedParam);
    int actualPolicy = 0;
    pthread_getschedparam(pthread_self(), &actualPolicy, &actualSchedParam);
    // Infos graphiques
    std::cout << "[Model Thread] Scheduling was changed from "
    << policyToString[initialPolicy] << "@prio=" << initialSchedParam.sched_priority
    << " to " << policyToString[actualPolicy] << "@prio=" << actualSchedParam.sched_priority
    << std::endl
    << "(" << policyToString[actualPolicy] << " scheduling: minPriority=" << minPriority
    << ", maxPriority=" << maxPriority << ")"
    << std::endl;
    
#else
    /* - - - - - Pour les autres OS - - - - -
     * à développer...
     */
    std::cout << "[Thread Modèle] Pas d'augmentation de priorité pour ce système d'exploitation." << std::endl;
#endif
}

void PlayerModel::update()
{
    setHighThreadPriority();
    
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
                    playState = AsyncParamChange::Play;
                    std::cout << "[Modèle] PLAY (interpolation de type '" << InterpolationTypes::GetInterpolationName(GetInterpolator()->GetType()) << "')" << std::endl;
                    interpolator->OnPlay();
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
        // On prend 1 ms de marge pour la réaction de l'OS en sortie de sleep (TOTALEMENT ARBITRAIRE !)
        if (updateThreadMeasurer.GetElapsedTimeSinceLastNewFrame_us() < updateThreadT_us - 1000)
            std::this_thread::sleep_for(std::chrono::microseconds(updateThreadT_us
                                                                  - updateThreadMeasurer.GetElapsedTimeSinceLastNewFrame_us() ) );
    }
}





// = = = = = = = = = = Property tree (for XML) import/export = = = = = = = = = =


void PlayerModel::SetConfigurationFromTree(bptree::ptree& tree)
{
    // D'abord on appelle le parent qui fait juste du stockage de données
    ControlModel::SetConfigurationFromTree(tree);
    
    // Ensuite on essaie de mettre en route les fonctionnalités, puis
    // on tient le Presenter au courant
    presenter->OnNewConnectionStatus( getMainSpatSender()->TryConnect(),
                                     getMainSpatSender()->GetConfigurationTree() );
}

