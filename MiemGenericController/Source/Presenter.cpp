/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

#include "PlayerModel.h"
#include "View.h"

#include "JuceHeader.h"


#include "AppPurpose.h"

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -
AppPurpose App::appPurpose = AppPurpose::GenericController;

Presenter::Presenter(View* _view) :
    PlayerPresenter(_view),

    view(_view),

    graphicSessionManager(this, _view)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    //view->GetMainContentComponent()->resized();
    
    
    appModeChangeRequest(PlayerAppMode::Loading);
}



// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =



// = = = = = = = = = = METHODES = = = = = = = = = =



