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
#include "Cursors.h"


using namespace Amusing;


// - - - - - Contruction and Destruction - - - - -

Presenter::Presenter(View* _view) :
    view(_view), test(),
    appMode(AppMode::Loading), // app is loading while the Model hasn't fully loaded yet

    graphicSessionManager(this, _view)
{
	test.insert(std::pair<int, double>(2, 5.5));
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    view->GetMainContentComponent()->resized();
    
    // HERE, WE SHOULD LOAD THE DEFAULT FILE
    //graphicSessionManager.__LoadDefaultTest();
	//lastPosition = 0;
    
    appModeChangeRequest(AppMode::None);
	Nsources = 0;
	Ncursors = 0;
	Nfollower = 0;
	tempo = 50;
	masterVolume = 0.5f;
	SetAllChannels();

	
}

void Presenter::SetAllChannels()
{
	graphicSessionManager.SetAllChannels();
}


void Presenter::CompleteInitialisation(AmusingModel* _model)
{
    // Self init
    model = _model;
	view->CompleteInitialization(model);
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

void Presenter::setTempo(int newTempo)
{
	tempo = newTempo;
}

int Presenter::getTempo()
{
	return tempo;
}

void Presenter::setMasterVolume(float newVolume)
{
	masterVolume = newVolume;
}

float Presenter::getMasterVolume()
{
	return masterVolume;
}

void Presenter::setSpeedArea(std::shared_ptr<IEditableArea> area, double speed)
{
	areaToSpeed[area] = speed;
}

double Presenter::getSpeedArea(std::shared_ptr<IEditableArea> area)
{
	if (areaToSpeed.find(area) == areaToSpeed.end())
		areaToSpeed[area] = 1;
	return areaToSpeed[area];
}

void Presenter::setVelocityArea(std::shared_ptr<IEditableArea> area, double velocity)
{
	areaToVelocity[area] = velocity;
}

double Presenter::getVelocityArea(std::shared_ptr<IEditableArea> area)
{
	if (areaToVelocity.find(area) == areaToVelocity.end())
		areaToVelocity[area] = 64;
	return areaToVelocity[area];
}

void Presenter::addOctave(std::shared_ptr<IEditableArea> newArea)
{
	if (octave.find(newArea) == octave.end())
	{
		octave[newArea] = 6;
	}
}

int Presenter::getNote(std::shared_ptr<IEditableArea> area, int circle)
{
	if (octave.find(area) != octave.end())
	{
		if (octave.at(area) == 10 && circle > 4)
			return octave.at(area) * 12 + gamme[4];
		else
			return octave.at(area) *12+ gamme[circle];
	}
	else
	{
		return 0;
	}
}

void Presenter::setOctave(std::shared_ptr<IEditableArea> currentArea, int newOctave)
{
	if (octave.find(currentArea) != octave.end())
		octave[currentArea] = newOctave;
	
}

void Presenter::setChannel(std::shared_ptr<EditableScene> scene,int channel)
{
	DBG("size of the map = " + (String)sceneToChannel.size());
	sceneToChannel[scene] = channel;
	//test[5] = 2;
	//test.insert(std::pair<int, double>(3, 5.8));
	//graphicSessionManager.HandleEventSync(std::shared_ptr<SceneEven)
}

int Presenter::getOctave(std::shared_ptr<IEditableArea> area)
{
	if (octave.find(area) != octave.end())
		return octave[area];
	else
		return 6;
}

int Presenter::getChannel(std::shared_ptr<EditableScene> scene)
{
	if (sceneToChannel.empty() || sceneToChannel.find(scene) == sceneToChannel.end())
		sceneToChannel[scene] = 1;
	return sceneToChannel[scene];
}

int Presenter::getTimeLineID(std::shared_ptr<IEditableArea> area)
{
	
	if (areaToSourceMulti.left.find(area) == areaToSourceMulti.left.end())
	{
		//areaToSourceMulti.left[area] = Nsources;
		std::pair<std::shared_ptr<IEditableArea>, int> newPair(area, Nsources);
		areaToSourceMulti.left.insert(newPair);
		//lastPositions.insert(std::pair<int, double>(Nsources, 0)); // add a new elt so we could retain the last cursor's position of the area
		++Nsources;
	}
	
	return areaToSourceMulti.left.at(area);
}

int Presenter::getReadingHeadID(std::shared_ptr<Cursor> cursor)
{
	if (cursorToReadingHead.left.find(cursor) == cursorToReadingHead.left.end())
	{
		std::pair<std::shared_ptr<Cursor>, int> newPair(cursor, Ncursors);
		cursorToReadingHead.left.insert(newPair);
		lastPositions.insert(std::pair<int, double>(Ncursors, 0));
		++Ncursors;
	}
	return cursorToReadingHead.left.at(cursor);
}

void Presenter::deleteReadingHeadRef(std::shared_ptr<Cursor> cursor)
{
	if (cursorToReadingHead.left.find(cursor) != cursorToReadingHead.left.end())
	{
		lastPositions.erase(cursorToReadingHead.left.at(cursor));
		//cursorToReadingHead.erase(cursor);
		cursorToReadingHead.left.erase(cursor);
	}
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

std::shared_ptr<Cursor> Presenter::getCursor(int m_Id)
{
	if (cursorToReadingHead.right.find(m_Id) == cursorToReadingHead.right.end())
	{
		DBG("no cursor associated to this playing head");
		return nullptr;
	}
	return cursorToReadingHead.right.at(m_Id);
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
		DBG("no area associated to this time line");
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

double Amusing::Presenter::computeFrequency(double surface)
{
	double W = view->GetMainContentComponent()->getWidth();
	double H = view->GetMainContentComponent()->getHeight();

	double minSize = 0.03 * (W + H) / 2.0;
	double maxSize = W * H;

	double baseF = 50;

	double minE = 0;
	double maxE = 2 + log10(3.0);

	double exp = minE + (maxE - minE) * (surface - minSize) / (maxSize - minSize);

	return pow(10.0,exp);
}

static int updatesCount = 0;
void Presenter::Update() // remettre l'interieur dans graphsessionmanager
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
	
	std::shared_ptr<GraphicEvent> graphicE;

	
	while (model->TryGetAsyncParamChange(param))
	{
		switch (param.Type)
		{
		case AsyncParamChange::ParamType::Activate :
			break;
		case AsyncParamChange::ParamType::Duration :
			DBG("new duration");
			break;
		case AsyncParamChange::ParamType::Position :
			
			
				//DBG("recu : " + (String)(1000 * param.DoubleValue));
				//DBG("param received");
				//graphicSessionManager.OnAudioPosition(param.DoubleValue);
				//graphicSessionManager.SetAllAudioPositions(param.DoubleValue);

			if (lastPositions.find(param.Id1) != lastPositions.end())
			{
				if(getTimeLineID(getCursor(param.Id1)->getAssociateArea()) == param.Id2)
					lastPositions.at(param.Id1) = param.DoubleValue;
			}
			break;

		default:
			break;
		}
	}

	for (std::map<int, double>::iterator it = lastPositions.begin(); it != lastPositions.end(); ++it)
	{
		//graphicSessionManager.SetAllAudioPositions(0);
		graphicSessionManager.SetAudioPositions(getCursor(it->first), it->second);
		//DBG((String)getTimeLineID(getCursor(it->first)->getAssociateArea()) + " " + (String)it->second);
	}
	//graphicSessionManager.SetAllAudioPositions(lastPosition);
}

//AudioDeviceManager* Presenter::getAudioDeviceManager()
//{
//	return model->sharedAudioDeviceManager;//getAudioDeviceManager();
//}

void Presenter::removeDeviceManagerFromOptionWindow()
{
	view->removeDeviceManagerFromOptionWindow();
}

//void Presenter::setAudioDeviceManager(AudioDeviceManager* deviceManager)
//{
//	view->setDeviceSelectorComponent(deviceManager);
//}