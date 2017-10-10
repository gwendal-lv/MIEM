/*
==============================================================================

GraphicSessionManager.cpp
Created: 28 Mar 2016 5:27:18pm
Author:  Gwendal Le Vaillant

==============================================================================
*/

#include "GraphicSessionManager.h"


// Other includes

#include "IPresenter.h"
#include "Presenter.h"
#include "View.h"

#include "AreaEvent.h"
#include "Exciter.h"

#include "SceneEvent.h"

#include "ControlEvent.h"

#include "MultiAreaEvent.h"

#include "AmusingScene.h"
#include "AnimatedPolygon.h"
#include "EditableEllipse.h"
#include "Follower.h"
#include "CompletePolygon.h"
#include <cmath>

#include "MultiCanvasComponentAmusing.h"

using namespace Amusing;
using namespace Miam;





// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(Presenter* presenter_, View* view_) ://IPresenter* presenter_, View* view_) :
	IGraphicSessionManager(presenter_),
	view(view_),
	myPresenter(presenter_),
	myMultiCanvasComponent(new MultiCanvasComponentAmusing(this)),
	deleting(false)
{

	editScene = view->GetMainContentComponent()->GetSceneEditionComponent();
	// ICI ON CHARGE DES TRUCS
	// Canvases const count defined here
	// 1 SEUL CANVAS PAR EXEMPLE
	// On doit créer les sous-objets graphiques de canevas (View) avant de
	// les transmettre au sous-module de gestion de canevas (Presenter) que l'on crée
	// d'ailleurs ici aussi.
	canvasManagers.push_back(std::make_shared<MultiSceneCanvasManager>(this, myMultiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
	myMultiCanvasComponent->CompleteInitialization();
	
	
	/*
	canvasManagers.push_back(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(),  SceneCanvasComponent::Id::Canvas2));
	*/
	//canvasManagers.back()
	for (size_t i = 0; i<canvasManagers.size(); i++)
	{
		// After canvases are created : scenes creation
		// DEFAULT SCENES, TO BE CHANGED
		canvasManagers[i]->AddScene("Scene 1");
		canvasManagers[i]->AddScene("Scene 2");
		canvasManagers[i]->AddScene("Scene 3");
	}
	
	// Links to the view module
	view->CompleteInitialization(this, myMultiCanvasComponent);
	editScene->CompleteInitialization(this);// , multiCanvasComponent);
	DBG("view + edit scene initialized");
	// And states of the canvases are forced
	for (size_t i = 0; i<canvasManagers.size(); i++)
		canvasManagers[i]->SetMode(CanvasManagerMode::Unselected);
	DBG("mode set");

	// SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
	nextAreaId = 0; // plus tard : valeur contenue dans le fichier de sauvegarde
	canvasManagers.front()->SetMode(CanvasManagerMode::SceneOnlySelected);
	DBG("front mode selected");

}

GraphicSessionManager::~GraphicSessionManager()
{
	DBG("GraphicSessionManager destructor");
	delete myMultiCanvasComponent;
}


// Testing purposes only
void GraphicSessionManager::__LoadDefaultTest()
{
	//srand(2016); // GRAINE fixée ici


	//for (size_t i = 0; i<canvasManagers.size(); i++)
	//	canvasManagers[i]->__AddTestAreas();
}




// ===== SETTERS AND GETTERS =====

std::shared_ptr<IEditableArea> GraphicSessionManager::GetSelectedArea()
{
	if (selectedCanvas)
		return getSelectedCanvasAsManager()->GetSelectedArea();
	else
		return nullptr;
}


void GraphicSessionManager::SetAllChannels()
{
	for (int i = 0; i < (int)canvasManagers.size(); i++)
	{
		std::shared_ptr<MultiSceneCanvasManager> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManagers[i]);
		canvasPtr->SetAllChannels();
	}
	
	if (auto myCanvasManager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManagers[0]))
		myCanvasManager->__AddAreasForTest();
	myMultiCanvasComponent->MouseSimulatorInit();
}

void GraphicSessionManager::SetAllChannels(StringArray opt)
{
	for (int i = 0; i < (int)canvasManagers.size(); i++)
	{
		std::shared_ptr<MultiSceneCanvasManager> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManagers[i]);
		canvasPtr->SetAllChannels();
	}

	if (auto myCanvasManager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManagers[0]))
		myCanvasManager->__AddAreasForTest();
	myMultiCanvasComponent->MouseSimulatorInit(opt);
}

void GraphicSessionManager::TryToClose()
{
	view->TryToClose();
}

// ===== UTILITIES =====




std::shared_ptr<MultiSceneCanvasManager> GraphicSessionManager::getSelectedCanvasAsManager()
{
	std::shared_ptr<MultiSceneCanvasManager> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasManager>(selectedCanvas);
	if (canvasPtr)
		return canvasPtr;
	else throw std::runtime_error("Canvas cannot be casted as a Miam::MultiSceneCanvasManager");
}


std::shared_ptr<MultiSceneCanvasEditor> GraphicSessionManager::getSelectedCanvasAsEditable()
{
	std::shared_ptr<MultiSceneCanvasEditor> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasEditor>(selectedCanvas);
	if (canvasPtr)
		return canvasPtr;
	else throw std::runtime_error("Canvas not selected, or canvas cannot be casted a Miam::MultiSceneCanvasEditor");
}

int GraphicSessionManager::circleToNote(int numCirc)
{
	//int gamme[7] = { 60, 62, 64, 65, 67, 69, 71 }; // juste
	//int gamme[7] = { 36, 38, 39, 41, 43, 45, 46 }; // 2 bemols
	int gamme[7] = { 48, 50, 52, 53, 55, 57, 59 };
	return gamme[numCirc];
}

// ===== EVENTS FROM THE PRESENTER ITSELF =====
void GraphicSessionManager::HandleEventSync(std::shared_ptr<GraphicEvent> event_)
{
	
	
	////////////////////
	int ADSR = 1; //////
	///////////////////

	Miam::AsyncParamChange param;
	
	
	
	// Event about an Area
	
	if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(event_))
	{
		if (auto multiE = std::dynamic_pointer_cast<MultiAreaEvent>(event_))
		{
			//DBG("multiareaevent");
			//getSelectedCanvasAsEditable()->CallRepaint
			//getSelectedCanvasAsManager()->handleUpdateNowIfNeeded
			if (multiE->GetType() == AreaEventType::Selected)
				DBG("deselection + selection");
		}

		// Event about an Exciter in particular : we'll have to update the spat mix !
		//if (auto exciter = std::dynamic_pointer_cast<Exciter>(areaE->GetConcernedArea()))
		//{
		//	//std::cout << "mix à mettre à jour" << std::endl;
		//}
		else if (auto area = std::dynamic_pointer_cast<EditableArea>(areaE->GetConcernedArea()))
		{
			int i;
			switch (areaE->GetType())
			{
			case AreaEventType::NothingHappened:
				//DBG("NothingHappened");
				break;
			case AreaEventType::Added:
				DBG("Area Added");
				//param.Type = Miam::AsyncParamChange::ParamType::Activate;
				
				
				if (auto circle = std::dynamic_pointer_cast<EditableEllipse>(area))
				{
					param.Id2 = 20;
					if (ADSR == 1)
						param.DoubleValue = 20;
				}
				
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					DBG("Complete Area");
					param.Id1 = myPresenter->getTimeLineID(area);
					param.Id2 = 1024;
					param.Type = Miam::AsyncParamChange::ParamType::Activate;
					param.IntegerValue = 1; // 1 pour activer la source, 0 pour la supprimer
					if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					{
						param.IntegerValue = myPresenter->getChannel(amusingScene);
					}
					else
						param.IntegerValue = 0;
					param.FloatValue = 1; // set initial speed to 1;
					myPresenter->SendParamChange(param); // envoie l'ordre de creer/ detruire une source audio
					//DBG("GSM : construct a new audio polygon, please. Id : " + (String)param.Id1);
					param.Type = Miam::AsyncParamChange::ParamType::Source;
					i = 0;
					while (complete->getAllPercentages(i, param.DoubleValue) && complete->getAllDistanceFromCenter(i, param.IntegerValue))
					{
						//param.IntegerValue = 60 + 2* (param.IntegerValue);
						param.IntegerValue = circleToNote(param.IntegerValue);
						//DBG("noteToSend = " + (String)param.IntegerValue);
						param.Id2 = i;
						param.FloatValue = (float)myPresenter->getVelocityArea(complete);
						myPresenter->SendParamChange(param);
						++i;
					}
					i = 0;

				
				}
				
				if (auto cursorArea = std::dynamic_pointer_cast<Cursor>(area))
				{
					DBG("Cursor");
					param.Type = Miam::AsyncParamChange::ParamType::Activate;
					//param.Id1 = 1024;
					if (auto amusingS = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					{
						if (auto associateArea = cursorArea->getAssociateArea())//amusingS->getAssociateArea(cursorArea))
						{
							if (auto associatePolygon = std::dynamic_pointer_cast<CompletePolygon>(associateArea))
								param.Id1 = myPresenter->getTimeLineID(associatePolygon);
						}
					}
					param.Id2 = myPresenter->getReadingHeadID(cursorArea);
					param.IntegerValue = 1;
					param.DoubleValue = cursorArea->getSpeed();
					myPresenter->SendParamChange(param);
				}
				//myPresenter->SendParamChange(param);
				DBG("Send");
				break;
			case AreaEventType::Deleted:
				if (auto cursorArea = std::dynamic_pointer_cast<Cursor>(area))
				{
					param.Type = Miam::AsyncParamChange::ParamType::Activate;
					param.Id1 = 1024;
					param.Id2 = myPresenter->getReadingHeadID(cursorArea);

					param.IntegerValue = 0;
					myPresenter->deleteReadingHeadRef(cursorArea);
					myPresenter->SendParamChange(param);
				}
				else if(auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					param.Type = Miam::AsyncParamChange::ParamType::Activate;
					param.Id1 = myPresenter->getTimeLineID(area);
					param.Id2 = 1024;
					param.IntegerValue = 0;
					myPresenter->SendParamChange(param);
				}
				
				
				break;
			case AreaEventType::PointDragBegins :
				DBG("PointDragBegins");
				break;
			case AreaEventType::PointDragStops :
				DBG("PointDragStops");
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					param.Type = AsyncParamChange::ParamType::None;
					//param.Id1 = myPresenter->getSourceID(area);
					//param.Type = Miam::AsyncParamChange::ParamType::Activate;
					//param.Id2 = 1; // 1 pour activer la source, 0 pour la supprimer
					//if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					//{
					//	DBG("channel I send : " + (String)myPresenter->getChannel(amusingScene));
					//	param.IntegerValue = myPresenter->getChannel(amusingScene);
					//}
					//else
					//	param.IntegerValue = 1;
					//myPresenter->SendParamChange(param); // envoie l'ordre de creer/ detruire une source audio

					param.Id1 = myPresenter->getTimeLineID(area);
					param.Type = Miam::AsyncParamChange::ParamType::Source;
					i = 0;
					////DBG("before while");
					while (complete->getAllPercentages(i, param.DoubleValue) && complete->getAllDistanceFromCenter(i, param.IntegerValue))
					{
						//param.IntegerValue = 60 + (2*param.IntegerValue);
						param.IntegerValue = circleToNote(param.IntegerValue);
						//DBG("noteToSend = " + (String)param.IntegerValue);
						param.Id2 = i;
						param.FloatValue = (float)myPresenter->getVelocityArea(complete);
						myPresenter->SendParamChange(param);
						++i;
					}
					//i = 0;


					//param.Id2 = 1000; // indiquera que l'on a envoye la position de tous les points
					//myPresenter->SendParamChange(param);
					//}
					//DBG("finish");
				}
				//myPresenter->SendParamChange(param);
				break;
			case AreaEventType::ShapeChanged :
				DBG("shapeChanged");
				if (auto cursor = std::dynamic_pointer_cast<Cursor>(area))
				{
					DBG("Cursor's shape changed");
					param.Type = AsyncParamChange::ParamType::Play;
					param.Id1 = myPresenter->getReadingHeadID(cursor);
					param.DoubleValue = cursor->getSpeed();
					myPresenter->SendParamChange(param);
				}
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					//param.Id1 = myPresenter->getSourceID(area);
					//param.Type = Miam::AsyncParamChange::ParamType::Activate;
					//param.Id2 = 1; // 1 pour activer la source, 0 pour la supprimer
					//if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					//{
					//	DBG("channel I send : " + (String)myPresenter->getChannel(amusingScene));
					//	param.IntegerValue = myPresenter->getChannel(amusingScene);
					//}
					//myPresenter->SendParamChange(param); // envoie l'ordre de creer/ detruire une source audio
					DBG("Polygon's shape changed");

					//param.Id1 = myPresenter->getTimeLineID(area);
					//param.Type = Miam::AsyncParamChange::ParamType::Activate;
					//param.Id2 = 1024;
					//param.IntegerValue = 1; // 1 pour activer la source, 0 pour la supprimer
					//if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					//{
					//	param.IntegerValue = myPresenter->getChannel(amusingScene);
					//}
					//else
					//	param.IntegerValue = 0;
					//param.FloatValue = (float)(myPresenter->getSpeedArea(complete));
					//myPresenter->SendParamChange(param);

					

					param.Id1 = myPresenter->getTimeLineID(area);
					//param.Id1 = myPresenter->getSourceID(area);
					param.Type = Miam::AsyncParamChange::ParamType::Source;
					i = 0;
					//DBG("before while");
					while (complete->getAllPercentages(i, param.DoubleValue) && complete->getAllDistanceFromCenter(i, param.IntegerValue))
					{
						if (param.DoubleValue == 0 && i != 0)
							DBG("aaaaaaarg");
						//DBG("cote to send : " + (String)i);
						//param.IntegerValue = 60 + (2*param.IntegerValue);
						param.IntegerValue = circleToNote(param.IntegerValue);
						//DBG("noteToSend = " + (String)param.IntegerValue);
						param.Id2 = i;
						param.FloatValue = (float)myPresenter->getVelocityArea(complete);
						myPresenter->SendParamChange(param);
						++i;
					}
					i = 0;
					//param.Id2 = 1000; // indiquera que l'on a envoye la position de tous les points
					//myPresenter->SendParamChange(param);
					//}
					//DBG("finish");
				}
				myPresenter->SendParamChange(param);
				break;
			case AreaEventType::Translation :
				//DBG("Translation");
				
				//area-> get center height --> volume
				break;
			case AreaEventType::RotScale :
				DBG("RotScale");
				
				break;
			case AreaEventType::Selected :
				DBG("selection");
				break;
				//case AreaEventType::
			case AreaEventType::CursorChanged:
				if (auto cursor = std::dynamic_pointer_cast<Cursor>(area))
				{
					param.Type = AsyncParamChange::Update;
					param.Id1 = myPresenter->getTimeLineID(cursor->getAssociateArea());
					param.Id2 = myPresenter->getReadingHeadID(cursor);
					param.DoubleValue = cursor->getPositionInAssociateArea(); //cursor->getSpeed();
					param.FloatValue = (float)cursor->getSpeed();//cursor->getPositionInAssociateArea();
					DBG("phi : " + (String)param.FloatValue);
					myPresenter->SendParamChange(param);
				}
				break;
			default:
				break;
			}
		}
	}
	else if (auto sceneE = std::dynamic_pointer_cast<SceneEvent>(event_))
	{
		DBG("scene event type = " + (String)(int)sceneE->GetType());
		switch (sceneE->GetType())
		{
		case SceneEventType::Added:
			DBG("Scene added = " + sceneE->GetNewScene()->GetName());
			//DBG((String)param.Type)
			
			
			
			break;
		case SceneEventType::Deleted:
			DBG("Scene deleted");
			break;
		case SceneEventType::NothingHappened:
			DBG("Nothing happened to scene");
			if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(sceneE->GetNewScene()))
			{
				//myPresenter->setChannel(amusingScene, 1);
			}
			break;
		case SceneEventType::SceneChanged:
			DBG("Scene Changed");
			if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(sceneE->GetNewScene()))
			{
				
				if(amusingScene->GetDrawableObjectsCount() > 0)
					editScene->setMidiChannel(myPresenter->getChannel(sceneE->GetNewScene()));
					
				for (int i = 0; i < (int)amusingScene->GetDrawableObjectsCount(); i++)
				{
					if (auto area = amusingScene->GetDrawableObject(i))//myPresenter->getSceneArea(amusingScene, i))
					{
						if (auto areaC = std::dynamic_pointer_cast<CompletePolygon>(area))
						{
							param.Type = AsyncParamChange::ParamType::UdpPort;
							param.Id1 = myPresenter->getTimeLineID(areaC);
							param.IntegerValue = myPresenter->getChannel(sceneE->GetNewScene());
							myPresenter->SendParamChange(param);
						}
						
					}
				}

			}
			break;
		default:
			break;
		}
	}
	else if(auto controlE = std::dynamic_pointer_cast<ControlEvent>(event_ ))
	{
		bool testCompletePolygon = true;
		switch (controlE->GetType())
		{
		case ControlEventType::Play:
			if (testCompletePolygon == false)
				OnAddFollower();
			DBG("play clicked");
			param.Type = Miam::AsyncParamChange::ParamType::Duration;
			param.IntegerValue = myPresenter->getTempo();
			param.FloatValue = myPresenter->getMasterVolume();
			myPresenter->SendParamChange(param);
			break;
		case ControlEventType::Pause:
			DBG("pause clicked");
			param.Type = Miam::AsyncParamChange::ParamType::Pause;
			myPresenter->SendParamChange(param);
			break;
		case ControlEventType::Stop:
			DBG("stop clicked");
			param.Type = Miam::AsyncParamChange::ParamType::Stop;
			param.Id1 = 255;
			myPresenter->SendParamChange(param);
			break;
		default:
			break;
		}
	}
}


void GraphicSessionManager::CanvasModeChanged(CanvasManagerMode canvasMode)
{
	switch (canvasMode)
	{
	case Miam::CanvasManagerMode::Null:
		break;
	case Miam::CanvasManagerMode::Loading:
		break;
	case Miam::CanvasManagerMode::Loaded:
		break;
	case Miam::CanvasManagerMode::Unselected:
		DBG("area unselected");
		break;
	case Miam::CanvasManagerMode::SceneOnlySelected:
		DBG("scene selected");
		break;
	case Miam::CanvasManagerMode::CanvasSelected:
		break;
	case Miam::CanvasManagerMode::AreaSelected:
		DBG("area selected");
		break;
	case Miam::CanvasManagerMode::EditingArea:
		break;
	case Miam::CanvasManagerMode::WaitingForPointCreation:
		break;
	case Miam::CanvasManagerMode::WaitingForPointDeletion:
		break;
	default:
		break;
	}
}








// ===== EVENTS TO VIEW =====

void GraphicSessionManager::DisplayInfo(String info)
{
	view->DisplayInfo(info);
}

void GraphicSessionManager::OnAddArea()
{
	if (selectedCanvas)
	{
		if (auto canvas = std::dynamic_pointer_cast<AmusingScene>(getSelectedCanvasAsEditable()))
			canvas->AddAnimatedArea(GetNextAreaId());
		else
			getSelectedCanvasAsEditable()->AddDefaultArea(GetNextAreaId());
	}
}

void GraphicSessionManager::OnFollowerTranslation(std::shared_ptr<GraphicEvent> graphicE)
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->OnFollowerTranslation(graphicE);
}

void GraphicSessionManager::OnAudioPosition(double position)
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->OnAudioPosition(position);
}

void GraphicSessionManager::SetAllAudioPositions(double position)
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->SetAllAudioPositions(position);
}

void GraphicSessionManager::SetAudioPositions(std::shared_ptr<Cursor> area, double position)
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->SetAudioPositions(area, position);
}

void GraphicSessionManager::OnAddSquare()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->AddNedgeArea(GetNextAreaId(), 4);
	}
}

void GraphicSessionManager::OnAddTriangle()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->AddNedgeArea(GetNextAreaId(), 3);
	}
}

void GraphicSessionManager::OnAddCircle()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->AddNedgeArea(GetNextAreaId(), 20);
	}
}

void GraphicSessionManager::OnAddTrueCircle()
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->AddTrueCircle(GetNextAreaId());
}

void GraphicSessionManager::OnAddFollower()
{
	if (selectedCanvas)
	{
		int N = getSelectedCanvasAsManager()->getNumberArea();
		for (int i = 0; i < N; ++i)
		{
			getSelectedCanvasAsManager()->AddFollower(GetNextAreaId());
		}
	}
}

void GraphicSessionManager::OnDelete()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->OnDelete();
		deleting = true;
	}
}

void GraphicSessionManager::OnAddComplete()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->AddCompleteArea();
	}
}

void GraphicSessionManager::OnDeviceOptionsClicked()
{
	view->ShowDeviceOptionsDialog();
}

void GraphicSessionManager::SetMidiChannel(int ch)
{
	if (selectedCanvas)
	{
		if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(getSelectedCanvasAsManager()->GetSelectedScene()))
		{
			DBG("channel to send : " + (String)ch);
			myPresenter->setChannel(amusingScene, ch);
			//getSelectedCanvasAsManager()->resendToModel();
			HandleEventSync(std::shared_ptr<SceneEvent>(new SceneEvent(getSelectedCanvasAsManager(), amusingScene, SceneEventType::SceneChanged)));
		}
	}
}

void GraphicSessionManager::setSpeedArea(std::shared_ptr<IEditableArea> area, double speed)
{
	myPresenter->setSpeedArea(area,speed);
}

void GraphicSessionManager::setVelocityArea(std::shared_ptr<IEditableArea> area, double speed)
{
	myPresenter->setVelocityArea(area, speed);
}

double GraphicSessionManager::getSpeed(std::shared_ptr<IEditableArea> area)
{
	return myPresenter->getSpeedArea(area);
}

double GraphicSessionManager::getVelocity(std::shared_ptr<IEditableArea> area)
{
	return myPresenter->getVelocityArea(area);
}

void GraphicSessionManager::OnTempoChanged(int newTempo) // voir si laisser comme ca, pcq pas d'interpretation necessaire pour le tempo
{
	myPresenter->setTempo(newTempo);
	HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Play)));
}

void GraphicSessionManager::OnMasterVolumeChanged(float newVolume)
{
	myPresenter->setMasterVolume(newVolume);
	HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Play)));
}