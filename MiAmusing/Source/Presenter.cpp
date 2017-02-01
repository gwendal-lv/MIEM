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
	
	if (areaToSourceMulti.left.find(area) == areaToSourceMulti.left.end())
	{
		//areaToSourceMulti.left[area] = Nsources;
		std::pair<std::shared_ptr<IEditableArea>, int> newPair(area, Nsources);
		areaToSourceMulti.left.insert(newPair);
		++Nsources;
	}
	
	return areaToSourceMulti.left.at(area);
}

std::shared_ptr<IEditableArea> Presenter::getAreaFromSource(int source)
{
	if (sourceToArea.find(source) == sourceToArea.end())
	{
		// source has no associated area
	}
	return sourceToArea[source];
}

static int updatesCount = 0;
void Presenter::Update()
{
	/*
	auto param = AsyncParamChange();
	for (int i = 0; i<100; i++)
	{
		updatesCount++;
		param.IntegerValue = updatesCount;
		SendParamChange(param);
	}
	DBG("la dedans");
	DBG(std::to_string(param.IntegerValue));
	*/
	//DBG("La");
	AsyncParamChange param;
	if (model->TryGetAsyncParamChange(param))
	{
		switch (param.Type)
		{
		case AsyncParamChange::ParamType::Activate :
			DBG("Next edge");
			break;
		case AsyncParamChange::ParamType::Duration :
			DBG("new duration");
			break;
		default:
			break;
		}
	}
}

