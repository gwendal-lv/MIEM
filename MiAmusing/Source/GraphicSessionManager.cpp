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

using namespace Amusing;
using namespace Miam;





// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(Presenter* presenter_, View* view_) ://IPresenter* presenter_, View* view_) :
	IGraphicSessionManager(presenter_),
	view(view_),
	myPresenter(presenter_),
	deleting(false)
{

	editScene = view->GetMainContentComponent()->GetSceneEditionComponent();
	// ICI ON CHARGE DES TRUCS
	// Canvases const count defined here
	// 1 SEUL CANVAS PAR EXEMPLE
	// On doit créer les sous-objets graphiques de canevas (View) avant de
	// les transmettre au sous-module de gestion de canevas (Presenter) que l'on crée
	// d'ailleurs ici aussi.
	canvasManagers.push_back(std::make_shared<MultiSceneCanvasManager>(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
	multiCanvasComponent->CompleteInitialization();
	DBG("pushed");
//	canvasManagers.back()->CompleteInitialization(canvasManagers.back());
	DBG("initialized");
	
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
	view->CompleteInitialization(this, multiCanvasComponent);
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
}


// Testing purposes only
void GraphicSessionManager::__LoadDefaultTest()
{
	srand(2016); // GRAINE fixée ici


	for (size_t i = 0; i<canvasManagers.size(); i++)
		canvasManagers[i]->__AddTestAreas();
}




// ===== SETTERS AND GETTERS =====

std::shared_ptr<IEditableArea> GraphicSessionManager::GetSelectedArea()
{
	if (selectedCanvas)
		return getSelectedCanvasAsManager()->GetSelectedArea();
	else
		return nullptr;
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


// ===== EVENTS FROM THE PRESENTER ITSELF =====
void GraphicSessionManager::HandleEventSync(std::shared_ptr<GraphicEvent> event_)
{
	int i;
	////////////////////
	int ADSR = 1; //////
	///////////////////

	Miam::AsyncParamChange param;
	double S;
	double f;
	int speed = 10;
	// Event about an Area
	
	if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(event_))
	{
		if (auto multiE = std::dynamic_pointer_cast<MultiAreaEvent>(event_))
		{
			//DBG("multiareaevent");
			//getSelectedCanvasAsEditable()->CallRepaint
			//getSelectedCanvasAsManager()->handleUpdateNowIfNeeded
		}

		// Event about an Exciter in particular : we'll have to update the spat mix !
		if (auto exciter = std::dynamic_pointer_cast<Exciter>(areaE->GetConcernedArea()))
		{
			//std::cout << "mix à mettre à jour" << std::endl;
		}
		else if (auto area = std::dynamic_pointer_cast<EditableArea>(areaE->GetConcernedArea()))
		{
			switch (areaE->GetType())
			{
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
					param.Id1 = myPresenter->getSourceID(area);
					param.Type = Miam::AsyncParamChange::ParamType::Activate;
					myPresenter->SendParamChange(param);
					param.Type = Miam::AsyncParamChange::ParamType::Source;
					i = 0;
					while (complete->getAllPercentages(i, param.DoubleValue));
					{
						param.Id2 = i;
						myPresenter->SendParamChange(param);
						++i;
					}
				}
				
				//myPresenter->SendParamChange(param);
				DBG("Send");
				break;
			case AreaEventType::Deleted:
				
				param.Type = Miam::AsyncParamChange::ParamType::Volume;
				param.DoubleValue = 0;
				myPresenter->SendParamChange(param);
				if (auto anime = std::dynamic_pointer_cast<AnimatedPolygon> (area))
				{
					DBG("Area deleted");
					if (deleting)
					{
						DBG("deleter les followers mtn");
						getSelectedCanvasAsManager()->deleteUnusedFollowers();
					}
					
				}
				else if (auto follower = std::dynamic_pointer_cast<Follower>(area))
				{
					DBG("follower a deleter !");
					param.Type = Miam::AsyncParamChange::ParamType::Stop;
					param.Id1 = myPresenter->getCtrlSourceId(follower);
				}
				break;
			case AreaEventType::PointDragBegins :
				DBG("PointDragBegins");
				break;
			case AreaEventType::PointDragStops :
				DBG("PointDragStops");
				break;
			case AreaEventType::ShapeChanged :
				DBG("Shape Changed");
				if (ADSR == 0)
				{
					S = area->GetSurface();
					f = 20 * pow(10, S / 4000);
					param.Type = Miam::AsyncParamChange::ParamType::Frequency;
					param.Id1 = myPresenter->getSourceID(area);
					param.DoubleValue = f;
				}
				else if (ADSR == 1)
				{
					param.Type = Miam::AsyncParamChange::ParamType::Frequency;
					param.Id1 = myPresenter->getSourceID(area);
					if (auto anime = std::dynamic_pointer_cast<AnimatedPolygon> (area))
					{
						param.DoubleValue = anime->GetAreteLength() / speed;
					}
				}
				myPresenter->SendParamChange(param);
				break;
			case AreaEventType::Translation :
				//DBG("Translation");
				if (auto anime = std::dynamic_pointer_cast<AnimatedPolygon> (area))
				{
					//DBG("H = " + (String)anime->GetHeight());
					param.Type = Miam::AsyncParamChange::ParamType::Volume;
					param.Id1 = myPresenter->getSourceID(area);
					param.DoubleValue = 1.5 * (301 - anime->GetHeight()) / (301 - 68);
					myPresenter->SendParamChange(param);
				}
				else
				{
					//DBG("not an anime");
				}
				//area-> get center height --> volume
				break;
			case AreaEventType::RotScale :
				//DBG("RotScale");
				if (ADSR == 0)
				{
					if (auto anime = std::dynamic_pointer_cast<AnimatedPolygon> (area))
					{
						//DBG("H = " + (String)anime->GetHeight());
						S = area->GetSurface();
						f = 20 * pow(10, S / 4000);
						param.Type = Miam::AsyncParamChange::ParamType::Frequency;
						param.Id1 = myPresenter->getSourceID(area);
						param.DoubleValue = f;
						myPresenter->SendParamChange(param);
					}
					else
						DBG("not an anime");
				}
				else if(ADSR == 1)
				{
					if (auto anime = std::dynamic_pointer_cast<AnimatedPolygon> (area))
					{
						//DBG("envoi duration");
						param.Type = Miam::AsyncParamChange::ParamType::Duration;
						param.Id1 = myPresenter->getSourceID(area);
						param.DoubleValue = anime->GetAreteLength() / speed;
						myPresenter->SendParamChange(param);
					}
				}
				
				break;
				//case AreaEventType::
			default:
				break;
			}
		}
	}
	else if (auto sceneE = std::dynamic_pointer_cast<SceneEvent>(event_))
	{
		
		switch (sceneE->GetType())
		{
		case SceneEventType::Added:
			DBG("Scene added");
			//DBG((String)param.Type)
			
			break;
		case SceneEventType::Deleted:
			DBG("Scene deleted");
			break;
		case SceneEventType::NothingHappened:
			DBG("Nothing happened");
			break;
		case SceneEventType::SceneChanged:
			DBG("Scene Changed");
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
			param.Type = Miam::AsyncParamChange::ParamType::Play;
			myPresenter->SendParamChange(param);
			break;
		case ControlEventType::Pause:
			param.Type = Miam::AsyncParamChange::ParamType::Pause;
			myPresenter->SendParamChange(param);
			break;
		case ControlEventType::Stop:
			param.Type = Miam::AsyncParamChange::ParamType::Stop;
			param.Id1 = 255;
			myPresenter->SendParamChange(param);
			break;
		default:
			break;
		}
	}
}


void GraphicSessionManager::CanvasModeChanged(CanvasManagerMode /*canvasMode*/)
{

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
	view->ShowDeviceOptionsDialog(myPresenter->getAudioDeviceManager());
}