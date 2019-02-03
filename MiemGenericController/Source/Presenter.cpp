/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

#include "Model.h"
#include "View.h"

#include "JuceHeader.h"


#include "AppPurpose.h"
#include "MiemDefaultSessions.h"

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

void Presenter::CompleteInitialisation(Model* _model)
{
    // - - - Init des attributs privés puis des parents - - -
    model = _model;
    PlayerPresenter::CompleteInitialisation(_model);
}

void Presenter::OnLoadDefaultSession()
{
    LoadSession(DefaultSessions::GetDefaultSessionCode_mcs());
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =



// = = = = = = = = = = METHODES = = = = = = = = = =
void Presenter::processParamChangeFromModel(AsyncParamChange const & paramChange)
{
    switch(paramChange.Type)
    {
        // no specific event treated specifically here... Control/Player/Presenter will do the ob
            
            // All untreated events : directed to parent class
        default :
            PlayerPresenter::processParamChangeFromModel(paramChange);
            break;
    }
}


