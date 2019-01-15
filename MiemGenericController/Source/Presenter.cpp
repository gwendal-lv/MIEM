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

void Presenter::CompleteInitialisation(PlayerModel* _model)
{
    if (auto castedModel = dynamic_cast<Model*>(_model))
        model = castedModel;
    else
        throw std::logic_error("The player model passed as argument must be a real Model, not only a PlayerModel");
    
    PlayerPresenter::CompleteInitialisation(_model);
}



// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =



// = = = = = = = = = = METHODES = = = = = = = = = =
void Presenter::Update()
{
    // Simple emptying of the lock-free queue
    AsyncParamChange paramChange;
    while (model->TrySendParamChange(paramChange))
    {
        // no processing for now....
        bool test = false;
    }
}


