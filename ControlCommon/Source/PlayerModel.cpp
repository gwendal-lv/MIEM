/*
  ==============================================================================

    PlayerModel.cpp
    Created: 24 Jan 2017 9:09:12am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <map>
#include <string>
#include <algorithm>

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
ControlModel(presenter_, false), // control does not launch its update thread
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
}






// = = = = = = = = = = Periodic updates = = = = = = = = = =

void PlayerModel::setHighThreadPriority()
{
#ifndef JUCE_WINDOWS // toutes plateformes POSIX
    pthread_setname_np(/*pthread_self(), */"MIEM Model::update Thread"); // pas de TID pour FreeBSD 2003... (doc macOS)
#endif
    
    
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
    
    ControlModel::update();
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

