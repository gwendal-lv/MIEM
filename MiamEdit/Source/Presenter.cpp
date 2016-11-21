/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

#include "View.h"


#include "JuceHeader.h"

using namespace Miam;


Presenter::Presenter(View* _view) :
    view(_view),
    appMode(AppMode::Loading), // app is loading while the Model hasn't fully loaded yet

    graphicSessionManager(_view)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    
    
    // HERE, WE SHOULD LOAD THE DEFAULT FILE
    graphicSessionManager.__LoadDefaultTest();
    
    // App mode changer to Scenes Edition by default (should be stored within the file ?)
    appModeChangeRequest(AppMode::EditSpatScenes);
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


