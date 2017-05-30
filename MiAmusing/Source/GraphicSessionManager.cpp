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


void GraphicSessionManager::SetAllChannels()
{
	for (int i = 0; i < canvasManagers.size(); i++)
	{
		std::shared_ptr<MultiSceneCanvasManager> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManagers[i]);
		canvasPtr->SetAllChannels();
	}
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
	bool ok = true;
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
					DBG("Complete Area");
					param.Id1 = myPresenter->getSourceID(area);
					param.Type = Miam::AsyncParamChange::ParamType::Activate;
					param.Id2 = 1; // 1 pour activer la source, 0 pour la supprimer
					if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					{
						param.IntegerValue = myPresenter->getChannel(amusingScene);
					}
					myPresenter->SendParamChange(param); // envoie l'ordre de creer/ detruire une source audio
					//DBG("GSM : construct a new audio polygon, please. Id : " + (String)param.Id1);
					param.Type = Miam::AsyncParamChange::ParamType::Source;
					i = 0;
					while(complete->getAllPercentages(i, param.DoubleValue)) // envoie l'info de chaque cotés au model
					{
						//DBG("GSM : the side " + (String)i + "is = " +(String)param.DoubleValue);
						param.Id2 = i;
						myPresenter->SendParamChange(param);
						++i;
					}
					
				}
				
				//myPresenter->SendParamChange(param);
				DBG("Send");
				break;
			case AreaEventType::Deleted:
				param.Type = Miam::AsyncParamChange::ParamType::Activate;
				param.Id1 = myPresenter->getSourceID(area);
				param.Id2 = 0;
				myPresenter->SendParamChange(param);
				
				break;
			case AreaEventType::PointDragBegins :
				DBG("PointDragBegins");
				break;
			case AreaEventType::PointDragStops :
				DBG("PointDragStops");
				break;
			case AreaEventType::ShapeChanged :
				DBG("Shape Changed");
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					param.Id1 = myPresenter->getSourceID(area);
					param.Type = Miam::AsyncParamChange::ParamType::Activate;
					param.Id2 = 1; // 1 pour activer la source, 0 pour la supprimer
					if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					{
						DBG("channel I send : " + (String)myPresenter->getChannel(amusingScene));
						param.IntegerValue = myPresenter->getChannel(amusingScene);
					}
					myPresenter->SendParamChange(param); // envoie l'ordre de creer/ detruire une source audio

					
					param.Type = Miam::AsyncParamChange::ParamType::Source;
					i = 0;
					//DBG("before while");
					while (complete->getAllPercentages(i, param.DoubleValue))
					{
						//DBG("cote to send : " + (String)i);
						param.Id2 = i;
						myPresenter->SendParamChange(param);
						++i;
					}
					param.Id2 = 1000; // indiquera que l'on a envoye la position de tous les points
					myPresenter->SendParamChange(param);
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
				//DBG("RotScale");
				
				break;
				//case AreaEventType::
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
			param.Type = Miam::AsyncParamChange::ParamType::Play;
			param.IntegerValue = myPresenter->getTempo();
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

void GraphicSessionManager::SetMidiChannel(int ch)
{
	if (selectedCanvas)
	{
		if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(getSelectedCanvasAsManager()->GetSelectedScene()))
		{
			DBG("channel to send : " + (String)ch);
			myPresenter->setChannel(amusingScene, ch);
			getSelectedCanvasAsManager()->resendToModel();
		}
	}
}

void GraphicSessionManager::OnTempoChanged(int newTempo) // voir si laisser comme ca, pcq pas d'interpretation necessaire pour le tempo
{
	myPresenter->setTempo(newTempo);
	HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Play)));
}