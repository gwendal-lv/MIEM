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

#include "AnimatedPolygon.h"
#include "Follower.h"
#include "GraphicEvent.h"
#include "MultiAreaEvent.h"


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
	Nfollower = 0;
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

int Presenter::getCtrlSourceId(std::shared_ptr<Follower> follower)
{
	if (followerToCtrlSource.left.find(follower) == followerToCtrlSource.left.end())
	{
		DBG("ajoute : " + (String)Nfollower);
		std::pair<std::shared_ptr<Follower>, int> newPair(follower, Nfollower);
		followerToCtrlSource.left.insert(newPair);
		++Nfollower;
	}
	return followerToCtrlSource.left.at(follower);
}

std::shared_ptr<IEditableArea> Presenter::getAreaFromSource(int source)
{
	/*
	if (sourceToArea.find(source) == sourceToArea.end())
	{
		// source has no associated area
	}
	return sourceToArea[source];
	*/
	if (areaToSourceMulti.right.find(source) == areaToSourceMulti.right.end())
	{
		DBG("problemes");
	}
	return areaToSourceMulti.right.at(source);
}

std::shared_ptr<Follower> Presenter::getFollowerFromCtrl(int ctrlId)
{
	if (followerToCtrlSource.right.find(ctrlId) == followerToCtrlSource.right.end())
	{
		DBG("pas de follower associe");
		return nullptr;
	}
	return followerToCtrlSource.right.at(ctrlId);
}

static int updatesCount = 0;
void Presenter::Update() // remettre l'interieur dans graphsessionmanager
{
    std::cout << "Update presenter" << std::endl;
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
	AsyncParamChange param, param2;
	std::shared_ptr<IEditableArea> area;
	std::shared_ptr<Follower> currentFollower;
	std::shared_ptr<GraphicEvent> graphicE;
	
	while (model->TryGetAsyncParamChange(param))
	{
		switch (param.Type)
		{
		case AsyncParamChange::ParamType::Activate :
                
                
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
                
            if (param.Id1 == 555)
            {
                
#ifdef VERSION_TEST_1_MILLION
                if (param.Id2 == 999999)
                {
                    std::cout << "Accusé de réception des 1 000 000 de valeurs via le Modèle" << std::endl;
#endif
                
                    std::cout << "Blink" << std::endl;
                    graphicSessionManager.__Blink();
                
#ifdef VERSION_TEST_1_MILLION
                }
#endif
                
            }
            else {
                
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////
                
			//DBG("Next edge");
			area = getAreaFromSource(param.Id1);
			if (auto anime = std::dynamic_pointer_cast<AnimatedPolygon>(area))
			{
				DBG("nouvelle arete = " + (String)anime->GetNextAreaLength());
				// separer la partie envoie dans le handleeventsync?
				param2.Type = AsyncParamChange::ParamType::Duration;
				param2.Id1 = param.Id1;
				param2.DoubleValue = anime->GetNextAreaLength() / 10;
				SendParamChange(param2);
			}
}
			break;
		case AsyncParamChange::ParamType::Duration :
			DBG("new duration");
			break;
		case AsyncParamChange::ParamType::Position :
			
			//DBG("position = " + (String)param.DoubleValue +" Id "+ (String)param.Id1);
			//area = getAreaFromSource(param.Id1);
			//DBG("bl");
			//if (auto anime = std::dynamic_pointer_cast<AnimatedPolygon>(area))
			//{
				//anime->GetFollower()->setPosition(param.DoubleValue);
			//}
			//DBG("Id du follower = " + (String)param.Id1);
			currentFollower = getFollowerFromCtrl(param.Id1);
			//currentFollower->setPosition(param.DoubleValue);
			//graphicE = currentFollower->setPosition(param.DoubleValue);
			//graphicSessionManager.HandleEventSync(currentFollower->setPosition(param.DoubleValue));
			graphicSessionManager.OnFollowerTranslation(currentFollower->setPosition(param.DoubleValue));
			break;

		default:
			break;
		}
	}
}

