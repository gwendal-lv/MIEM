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
#include "View.h"

#include "AreaEvent.h"
#include "Exciter.h"

#include "SceneEvent.h"

using namespace Miam;


// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(IPresenter* presenter_, View* view_) :
    IGraphicSessionManager(presenter_),
    view(view_)
{

    
    // ICI ON CHARGE DES TRUCS
    // Canvases const count defined here
    // 1 SEUL CANVAS PAR EXEMPLE
    // On doit créer les sous-objets graphiques de canevas (View) avant de
    // les transmettre au sous-module de gestion de canevas (Presenter) que l'on crée
    // d'ailleurs ici aussi.
    canvasManagers.push_back(std::make_shared<MultiSceneCanvasManager>(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
    canvasManagers.back()->CompleteInitialization(canvasManagers.back());
    /*
    canvasManagers.push_back(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(),  SceneCanvasComponent::Id::Canvas2));
     */
    
    // Links to the view module
    view->CompleteInitialization(this, multiCanvasComponent);
    
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
    {
        // After canvases are created : scenes creation
        // DEFAULT SCENES, TO BE CHANGED
        canvasManagers[i]->AddScene("Scène 1, pour le plaisir");
        canvasManagers[i]->AddScene("Scène 2 oh ouuiiii");
        canvasManagers[i]->AddScene("Scène jamais 2 sans 3");
    }
    
    
    
    // And states of the canvases are forced
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
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
    
    
    for(size_t i=0 ; i<canvasManagers.size(); i++)
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
    auto canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasManager>( selectedCanvas);
    if (canvasPtr)
        return canvasPtr;
    else throw std::runtime_error("Canvas cannot be casted as a Miam::MultiSceneCanvasManager");
}


// ===== EVENTS FROM THE PRESENTER ITSELF =====
void GraphicSessionManager::HandleEventSync(std::shared_ptr<GraphicEvent> event_)
{
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
			case AreaEventType::Added :
				//DBG("Area Added");
				break;
			case AreaEventType::Deleted :
				//DBG("Area deleted");
				break;
			default:
				break;
			}
		}
    }
	else if (auto sceneE = std::dynamic_pointer_cast<SceneEvent>(event_))
	{
		switch (sceneE->GetType())
		{
		case SceneEventType::Added :
			//DBG("Scene added");
			break;
		case SceneEventType::Deleted :
			//DBG("Scene deleted");
			break;
		case SceneEventType::NothingHappened :
			//DBG("Nothing happened");
			break;
		case SceneEventType::SceneChanged :
			//DBG("Scene Changed");
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




