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

#include <sstream>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -

Presenter::Presenter(View* _view) :
    view(_view),
    appMode(AppMode::Loading), // app is loading while the Model hasn't fully loaded yet

    graphicSessionManager(_view, this),
    spatStatesEditionManager(_view),
    settingsManager(_view, this)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    
}

void Presenter::CompleteInitialisation(Model* _model)
{
    // Self init
    model = _model;
    SpatPresenter::CompleteInitialisation(_model);
    // Sub-modules
    spatStatesEditionManager.CompleteInitialisation(model->GetSpatInterpolator());
    graphicSessionManager.CompleteInitialisation(model->GetSpatInterpolator());
    settingsManager.CompleteInitialisation(model);
    
    // Actual mode change here
    // App mode changer to Scenes Edition by default (should be stored within the file ?)
    appModeChangeRequest(AppMode::EditSpatScenes);
    
    // Loading of the first XML session file (default if nothing else provided)
    std::string firstFileName = "../../../../../SpatCommon/Sessions/Default.miam";
    try {
        LoadSession(firstFileName);
    }
    catch (XmlReadException& e)
    {
        // Also display in a new window
        view->DisplayInfo(std::string("[Reading XML] ") + e.what(), true);
        std::cout << std::string("[Reading XML] ") << e.what() << std::endl;
    }
    // Updates (graphical mostly) just after
    spatStatesEditionManager.UpdateView();
    
    //graphicSessionManager.__LoadDefaultTest();
}





AppMode Presenter::appModeChangeRequest(AppMode newAppMode)
{
    // First check : are we running a new mode ?
    if (newAppMode != appMode)
    {
        view->ChangeAppMode(AppMode::Loading);
        
        // - - - - - PRE-CHANGE PROCESSING - - - - -
        switch(appMode)
        {
            case AppMode::EditSpatStates :
                // If leaving the matrices editing : data save before changing mode
                spatStatesEditionManager.OnLeaveSpatStatesEdition();
                break;
                
            default :
                break;
        }
        
        // Internal+graphical update
        appMode = newAppMode;
        view->ChangeAppMode(appMode);
        
        // - - - - - POST-CHANGE PROCESSING - - - - -
        switch(appMode)
        {
            case AppMode::EditSpatStates :
                // At leats : reloading of maybe changed data from other mode
                spatStatesEditionManager.OnEnterSpatStatesEdition();
                break;
                
            case AppMode::EditSpatScenes :
                // At leats : reloading of maybe changed data from other mode
                graphicSessionManager.OnEnterSpatScenesEdition();
                break;
                
            default :
                break;
        }
    }
    
    
    // Actual new mode
    return appMode;
}



void Presenter::Update()
{
    // Check of Model messages !!!!!!
}




