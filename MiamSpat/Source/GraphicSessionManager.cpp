/*
  ==============================================================================

    GraphicSessionManager.cpp
    Created: 28 Mar 2016 5:27:18pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "GraphicSessionManager.h"


// Other includes

#include "Presenter.h"
#include "View.h"

#include "MultiAreaEvent.h"
#include "Exciter.h"

#include "SceneEvent.h"

using namespace Miam;


// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(Presenter* presenter_, View* view_) :
    GraphicSpatSessionManager(presenter_),
    presenter(presenter_),
    view(view_)
{
    // SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
    nextAreaId = 0; // plus tard : valeur chargée depuis dans le fichier de sauvegarde

    
    // DEFINITION DU NOMBRE DE CANEVAS
    canvasManagers.push_back(std::make_shared<MultiSceneCanvasManager>(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
    canvasManagers.push_back(std::make_shared<MultiSceneCanvasManager>(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas2));
    
    // Links to the view module
    view->CompleteInitialization(this, multiCanvasComponent);
    
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
    {
        // After canvases are created : scenes creation
        // Juste 1 scène pour ne pas avoir ne pointeur sur NULL
        canvasManagers[i]->AddScene("[Scène vide]", true);
    }
    
    
    
    // And states of the canvases are forced
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
        canvasManagers[i]->SetMode(CanvasManagerMode::PlayingWithExciters);
    
    
    // Resize forcé (pour ne pas avoir de trucs de taille zéro
    view->GetMainContentComponent()->resized();
    
}

GraphicSessionManager::~GraphicSessionManager()
{
}






// ===== SETTERS AND GETTERS =====





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
    // Events about several areas at once
    if (auto multiAreaE = std::dynamic_pointer_cast<MultiAreaEvent>(event_))
    {
        // Constructeur de Copie vers un nouvel évènement simple
        auto singleMainAreaE = std::make_shared<AreaEvent>( multiAreaE.get() );
        handleSingleAreaEventSync(singleMainAreaE);
        // Pour les autres : on envoie juste les pointeurs
        for (size_t i = 0 ; i < multiAreaE->GetOtherEventsCount() ; i++)
            handleSingleAreaEventSync(multiAreaE->GetOtherEvent(i));
    }
    // Event about a SINGLE Area
    else if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(event_))
    {
        handleSingleAreaEventSync(areaE);
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
void GraphicSessionManager::handleSingleAreaEventSync(std::shared_ptr<AreaEvent>& areaE)
{
    // Event about an Exciter in particular : we'll have to update the spat mix !
    if (auto exciter = std::dynamic_pointer_cast<Exciter>(areaE->GetConcernedArea()))
    {
        switch (areaE->GetType())
        {
            case AreaEventType::Added :
            case AreaEventType::Deleted :
            case AreaEventType::Translation :
                
                break;
                
            default:
                break;
        }
    }
    /*
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
     */
}



void GraphicSessionManager::CanvasModeChanged(CanvasManagerMode /*canvasMode*/)
{
    
}





// ===== EVENTS TO VIEW =====

void GraphicSessionManager::DisplayInfo(String info)
{
    view->DisplayInfo(info);
}



// = = = = = = = = = = XML import/export = = = = = = = = = =

void GraphicSessionManager::SetFromTree(bptree::ptree& graphicSessionTree)
{
    GraphicSpatSessionManager::SetFromTree(graphicSessionTree);
    
    // Plus aucun traitement supplémentaire, pour l'instant...
}






