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
AppPurpose App::appPurpose = AppPurpose::Spatialisation;

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
    
    PlayerPresenter::CompleteInitialisation(model);
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =



// = = = = = = = = = = PERIODIC UPDATES = = = = = = = = = =
void Presenter::Update()
{
    AsyncParamChange paramChange;
    while (model->TryGetAsyncParamChange(paramChange))
    {
        std::cout << paramChange.DoubleValue << std::endl;
    }
}
