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
    //presenter->CompleteInitialisation(this); // posait des problèmes....
}

PlayerModel::~PlayerModel()
{
}






// = = = = = = = = = = Periodic updates = = = = = = = = = =

void PlayerModel::launchUpdateThread()
{
    // Launch of thread, at the specified frequency
    continueUpdate = true;
    // Using a c++11 lambda function for class member calling
    updateThread = std::thread( [this] {this->update();} );
}
void PlayerModel::setHighThreadPriority()
{
    // contents generalised into utility static methods
    PeriodicUpdateThread::SetThreadName("MIEM Model::update Thread");
    PeriodicUpdateThread::SetHighThreadPriority();
}

void PlayerModel::update()
{
    setHighThreadPriority();
    
    // Main loop, with a "while(continueUpdate)" inside
    ControlModel::update();
}




// = = = = = = = = Events from Presenter = = = = = = = =
bool PlayerModel::ResetOscConfigurationFromTree(bptree::ptree& tree)
{
    bool baseResult = ControlModel::ResetOscConfigurationFromTree(tree);
    
    presenter->OnNewConnectionStatus( baseResult,
                                     getMainSpatSender()->GetConfigurationTree() );
    
    return baseResult;
}




// = = = = = = = = = = Property tree (for XML) import/export = = = = = = = = = =


void PlayerModel::SetConfigurationFromTree(bptree::ptree& tree)
{
    // D'abord on appelle le parent qui fait juste du stockage de données
    ControlModel::SetConfigurationFromTree(tree);
    
    // Ensuite on essaie de mettre en route les fonctionnalités, puis
    // on tient le Presenter au courant
    // - Connection status
    presenter->OnNewConnectionStatus( getMainSpatSender()->TryConnect(),
                                     getMainSpatSender()->GetConfigurationTree() );
    // - Master Gain slider
    
}

