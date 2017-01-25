/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

#include "AmusingModel.h"
#include "View.h"


#include "JuceHeader.h"

using namespace Amusing;


// - - - - - Contruction and Destruction - - - - -

Presenter::Presenter(View* _view) :
    view(_view),
    appMode(AppMode::Loading), // app is loading while the Model hasn't fully loaded yet

    graphicSessionManager(this, _view)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    view->GetMainContentComponent()->resized();
    
    // HERE, WE SHOULD LOAD THE DEFAULT FILE
    //graphicSessionManager.__LoadDefaultTest();
    
    
    appModeChangeRequest(AppMode::None);
	Nsources = 0;
}


void Presenter::CompleteInitialisation(AmusingModel* _model)
{
    // Self init
    model = _model;
	//view->CompleteInitialization(model);
	//graphicSessionManager.CompleteInitialization(model);
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


int Presenter::getSourceID(std::shared_ptr<IEditableArea> area)
{
	/*
	if (areaToSource.find(area) == areaToSource.end())
	{
		// area has no associated source yet
		areaToSource[area] = Nsources;
		sourceToArea[Nsources] = area;
		++Nsources;
	}
	return areaToSource[area];
	*/
	/*
	if (areaToSourceMulti.left.find(area) == areaToSourceMulti.left.end())
	{
		//areaToSourceMulti.left[area] = Nsources;
		//std::pair<int, std::shared_ptr<IEditableArea>> newPair(area,Nsources);
		areaToSourceMulti.left.insert();
		++Nsources;
	}
	*/
	return areaToSourceMulti.left[area];
}

std::shared_ptr<IEditableArea> Presenter::getAreaFromSource(int source)
{
	if (sourceToArea.find(source) == sourceToArea.end())
	{
		// source has no associated area
	}
	return sourceToArea[source];
}