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

#include "AmusingScene.h"
#include "AnimatedPolygon.h"
#include <cmath>

using namespace Amusing;
using namespace Miam;


void GraphicSessionManager::run()
{
	// need getPoint
	// si play -> verifier si le sommet suivant est inclu dans un autre polygon
	// si oui -> calculer l'intersection, ce sera le prochain "sommet"
	//           le prochain son sera associe au polygone rencontre
	
	//auto currentArea = std::dynamic_pointer_cast<EditablePolygon>( GetSelectedArea());
	//currentArea->
	// wait()
}

void GraphicSessionManager::playSound()
{
	//startThread();
}


// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(Presenter* presenter_, View* view_) ://IPresenter* presenter_, View* view_) :
	IGraphicSessionManager(presenter_), Thread("collision thread"),
	view(view_),
	myPresenter(presenter_)
{

	editScene = view->GetMainContentComponent()->GetSceneEditionComponent();
	// ICI ON CHARGE DES TRUCS
	// Canvases const count defined here
	// 1 SEUL CANVAS PAR EXEMPLE
	// On doit créer les sous-objets graphiques de canevas (View) avant de
	// les transmettre au sous-module de gestion de canevas (Presenter) que l'on crée
	// d'ailleurs ici aussi.
	canvasManagers.push_back(new MultiSceneCanvasManager(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
	/*
	canvasManagers.push_back(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(),  SceneCanvasComponent::Id::Canvas2));
	*/

	for (size_t i = 0; i<canvasManagers.size(); i++)
	{
		// After canvases are created : scenes creation
		// DEFAULT SCENES, TO BE CHANGED
		canvasManagers[i]->AddScene("Scène 1, pour le plaisir");
		canvasManagers[i]->AddScene("Scène 2 oh ouuiiii");
		canvasManagers[i]->AddScene("Scène jamais 2 sans 3");
	}

	// Links to the view module
	view->CompleteInitialization(this, multiCanvasComponent);
	editScene->CompleteInitialization(this);// , multiCanvasComponent);

	// And states of the canvases are forced
	for (size_t i = 0; i<canvasManagers.size(); i++)
		canvasManagers[i]->SetMode(CanvasManagerMode::Unselected);


	// SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
	nextAreaId = 0; // plus tard : valeur contenue dans le fichier de sauvegarde
	canvasManagers.front()->SetMode(CanvasManagerMode::SceneOnlySelected);


}

GraphicSessionManager::~GraphicSessionManager()
{
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



void GraphicSessionManager::SetSelectedCanvas(MultiSceneCanvasInteractor* _selectedCanvas)
{

	// We do something only if there has been a change
	if (selectedCanvas != _selectedCanvas)
	{
		// At first : unselection of previous canvas...
		if (selectedCanvas)
		{
			selectedCanvas->SetMode(CanvasManagerMode::Unselected);
			selectedCanvas->CallRepaint();
		}

		/*
		selectedCanvas = dynamic_cast<MultiSceneCanvasEditor*>(_selectedCanvas);
		if (!selectedCanvas)
		throw std::runtime_error(std::string("The canvas to be selected is only an Interactor, and not an Editor (no editing features...)"));
		*/
		// Pas besoin du cast en canvas "éditeur", pour le Miam Spat un simple
		// "interacteur" va suffire
		selectedCanvas = _selectedCanvas;

		selectedCanvas->SetMode(CanvasManagerMode::SceneOnlySelected);

		//setMode(GraphicSessionMode::CanvasSelected);

		multiCanvasComponent->resized();
	}
	else
	{
		// rien du tout
	}
}

MultiSceneCanvasManager* GraphicSessionManager::getSelectedCanvasAsManager()
{
	MultiSceneCanvasManager* canvasPtr = dynamic_cast<MultiSceneCanvasManager*>(selectedCanvas);
	if (canvasPtr)
		return canvasPtr;
	else throw std::runtime_error("Canvas cannot be casted as a Miam::MultiSceneCanvasManager");
}



// ===== EVENTS FROM THE PRESENTER ITSELF =====
void GraphicSessionManager::HandleEventSync(std::shared_ptr<GraphicEvent> event_)
{
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
				param.Type = Miam::AsyncParamChange::ParamType::Activate;
				param.Id1 = myPresenter->getSourceID(area);
				//param.Id2 = ; //  = type de la source
				if (auto anime = std::dynamic_pointer_cast<AnimatedPolygon> (area))
				{
					param.Id2 = anime->GetContourSize();
					if(ADSR == 1)
						param.DoubleValue = anime->GetAreteLength() / speed;
				}
				DBG("Nbre cote = " + (String)param.Id2);
				myPresenter->SendParamChange(param);
				DBG("Send");
				break;
			case AreaEventType::Deleted:
				param.Type = Miam::AsyncParamChange::ParamType::Volume;
				param.DoubleValue = 0;
				myPresenter->SendParamChange(param);
				DBG("Area deleted");
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
					if(auto anime = std::dynamic_pointer_cast<AnimatedPolygon> (area))
						param.DoubleValue = anime->GetAreteLength() / speed;
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
					DBG("not an anime");
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
						DBG("envoi duration");
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
		switch (controlE->GetType())
		{
		case ControlEventType::Play:
			param.Type = Miam::AsyncParamChange::ParamType::Play;
			myPresenter->SendParamChange(param);
			break;
		case ControlEventType::Pause:
			param.Type = Miam::AsyncParamChange::ParamType::Pause;
			myPresenter->SendParamChange(param);
			break;
		case ControlEventType::Stop:
			param.Type = Miam::AsyncParamChange::ParamType::Stop;
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
		if (auto canvas = std::dynamic_pointer_cast<AmusingScene>(selectedCanvas->GetSelectedScene()))
			canvas->AddAnimatedArea(GetNextAreaId());
		else
			selectedCanvas->GetSelectedScene()->AddDefaultArea(GetNextAreaId());
	}
}

void GraphicSessionManager::OnAddSquare()
{
	if (selectedCanvas)
	{
		if (auto canvas = std::dynamic_pointer_cast<AmusingScene>(selectedCanvas->GetSelectedScene()))
			canvas->AddNedgeArea(GetNextAreaId(),4);
		else
			selectedCanvas->GetSelectedScene()->AddDefaultArea(GetNextAreaId());
	}
}

void GraphicSessionManager::OnAddTriangle()
{
	if (selectedCanvas)
	{
		if (auto canvas = std::dynamic_pointer_cast<AmusingScene>(selectedCanvas->GetSelectedScene()))
			canvas->AddNedgeArea(GetNextAreaId(), 3);
		else
			selectedCanvas->GetSelectedScene()->AddDefaultArea(GetNextAreaId());
	}
}

void GraphicSessionManager::OnAddCircle()
{
	if (selectedCanvas)
	{
		if (auto canvas = std::dynamic_pointer_cast<AmusingScene>(selectedCanvas->GetSelectedScene()))
			canvas->AddNedgeArea(GetNextAreaId(), 20);
		else
			selectedCanvas->GetSelectedScene()->AddDefaultArea(GetNextAreaId());
	}
}

