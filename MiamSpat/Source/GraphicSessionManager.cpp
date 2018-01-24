/*
  ==============================================================================

    GraphicSessionManager.cpp
    Created: 28 Mar 2016 5:27:18pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <limits> // tests avant envoi dans les paquets lock-free

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
    // Event about one area, or several areas at once
    if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(event_))
    {
        handleAreaEventSync(areaE);
    }
    
    // Scene information
    /*
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
     */
}
void GraphicSessionManager::handleAreaEventSync(const std::shared_ptr<AreaEvent>& areaE)
{
    // Events about several areas at once
    if (auto multiAreaE = std::dynamic_pointer_cast<MultiAreaEvent>(areaE))
    {
        // Constructeur de Copie vers un nouvel évènement simple
        auto singleMainAreaE = std::make_shared<AreaEvent>( multiAreaE.get() );
        handleSingleAreaEventSync(singleMainAreaE); // on est sûr que c'est un évnmt simple
        // Pour les autres : il faut faire un appel récursif à cette fonction au cas où l'on
        // a des MultiEvent imbriqués (cas très courant)
        for (size_t i = 0 ; i < multiAreaE->GetOtherEventsCount() ; i++)
            handleAreaEventSync(multiAreaE->GetOtherEvent(i));
    }
    // Event about a SINGLE Area
    else
    {
        handleSingleAreaEventSync(areaE);
    }
}
void GraphicSessionManager::handleSingleAreaEventSync(const std::shared_ptr<AreaEvent>& areaE)
{
    // Event about an Area in particular : we may have to update the spat mix !
    if (auto area = std::dynamic_pointer_cast<SpatArea>(areaE->GetConcernedArea()))
    {
        AsyncParamChange paramChange;
        
        switch (areaE->GetType())
        {
            // On n'envoie l'excitation qu'en mode de jeu réel
            case AreaEventType::ExcitementAmountChanged :
                if (presenter->getAppMode() == AppMode::Playing)
                {
					// Et on n'envoie que les excitations liées réellement à un état de spat
					if (area->GetSpatStateIndex() >= 0)
					{
						paramChange.Type = AsyncParamChange::ParamType::Excitement;
						paramChange.DoubleValue = area->GetTotalAudioExcitement();

						// Attention : pour les IDs, on déclenche une grosse exception si on dépasse...
						if (area->GetSpatStateIndex() < std::numeric_limits<int>::max())
							paramChange.Id1 = (int)area->GetSpatStateIndex();
						else
							throw std::overflow_error("Spat state Index is too big to fit into an 'int'. Cannot send the concerned lock-free parameter change.");
						if (area->GetId() < std::numeric_limits<int>::max())
							paramChange.Id2 = (int)area->GetId();
						else
							throw std::overflow_error("Area UID is too big to fit into an 'int'. Cannot send the concerned lock-free parameter change.");

						presenter->SendParamChange(paramChange);
					}
                }
                break;
                
            default:
                break;
        }
    }
    // Event about an Exciter in particular : ici on ne fait rien....
    /*
    else if (auto exciter = std::dynamic_pointer_cast<Exciter>(areaE->GetConcernedArea()))
    {
        switch (areaE->GetType())
        {
            default:
                break;
        }
    }
     */
}
void GraphicSessionManager::OnModelStarted()
{
    for (size_t i = 0 ; i <canvasManagers.size() ; i++)
        canvasManagers[i]->SelectScene(0);
}



void GraphicSessionManager::CanvasModeChanged(CanvasManagerMode /*canvasMode*/)
{
    
}





// ===== EVENTS TO VIEW =====

void GraphicSessionManager::DisplayInfo(String info, int /*priority*/)
{
    view->DisplayInfo(info);
}



// = = = = = = = = = = XML import/export = = = = = = = = = =

void GraphicSessionManager::SetFromTree(bptree::ptree& graphicSessionTree)
{
    GraphicSpatSessionManager::SetFromTree(graphicSessionTree);
    
    // Plus aucun traitement supplémentaire, pour l'instant...
}






