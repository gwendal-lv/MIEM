/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

//#include "Model.h"
#include "View.h"


#include "JuceHeader.h"

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -

Presenter::Presenter(View* _view) :
    view(_view),
    appMode(AppMode::Loading), // app is loading while the Model hasn't fully loaded yet

    graphicSessionManager(_view)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    view->GetMainContentComponent()->resized();
    
    // HERE, WE SHOULD LOAD THE DEFAULT FILE
    graphicSessionManager.__LoadDefaultTest();
    
    
    appModeChangeRequest(AppMode::None);
}


void Presenter::CompleteInitialisation(Model* _model)
{
    // Self init
    model = _model;
	view->CompleteInitialization(model);
	graphicSessionManager.CompleteInitialization(model);
}





AppMode Presenter::appModeChangeRequest(AppMode newAppMode)
{
    // First check : are we running a new mode ?
    if (newAppMode != appMode)
    {
        view->ChangeAppMode(AppMode::Loading);
        
        appMode = newAppMode;
        view->ChangeAppMode(appMode);
    }
    
    
    // Actual new mode
    return appMode;
}


