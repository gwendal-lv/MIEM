/*
  ==============================================================================

    MultiSceneCanvasManager.cpp
    Created: 10 Jan 2017 9:37:54pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MultiSceneCanvasManager.h"

#include "MultiSceneCanvasComponent.h"

#include "SpatScene.h"


using namespace Miam;

MultiSceneCanvasManager::MultiSceneCanvasManager(IGraphicSessionManager* graphicSessionManager_, MultiSceneCanvasComponent* canvasComponent_, SceneCanvasComponent::Id selfId_)
:
MultiSceneCanvasInteractor(graphicSessionManager_, canvasComponent_, selfId_)
{
}

MultiSceneCanvasManager::~MultiSceneCanvasManager()
{
    
}

void MultiSceneCanvasManager::AddScene(std::string name, bool selectNewScene)
{
    // We construct a scene that DOES NOT allow the selection of a particular area
    std::shared_ptr<EditableScene> newScene = std::make_shared<SpatScene>(shared_from_this(), canvasComponent->GetCanvas());
    newScene->SetName(name);
    
    MultiSceneCanvasInteractor::AddScene(newScene, selectNewScene);
}




// = = = = = Gestion des évènements des scènes = = = = =

void MultiSceneCanvasManager::processSingleAreaEventSync(std::shared_ptr<AreaEvent>& areaE)
{
    MultiSceneCanvasInteractor::processSingleAreaEventSync(areaE);
    
    // - - - Traitement des évènements excitateurs - - -
    // ne concernent pas forcément que la scène actuelle sélectionnée !!
    if ( auto exciter = std::dynamic_pointer_cast<Exciter>( areaE->GetConcernedArea() ) )
    {
        // Ajout, suppression, et déplacement (qui est juste la translation pour l'instant)
        // On teste toutes les aires graphiques de la scène LIÉE À L'EXCITATEUR
        // si on est dans un de ces cas...
        switch (areaE->GetType()) {
            case AreaEventType::Added :
            case AreaEventType::Deleted :
            case AreaEventType::Translation :
                
                // DANS CHAQUE SCÈNE LORS DU DÉPLACEMENT DE L'EXCITATEUR ???
                // DANS CHAQUE SCÈNE LORS DU DÉPLACEMENT DE L'EXCITATEUR ???
                // DANS CHAQUE SCÈNE LORS DU DÉPLACEMENT DE L'EXCITATEUR ???
                // DANS CHAQUE SCÈNE LORS DU DÉPLACEMENT DE L'EXCITATEUR ???
                // DANS CHAQUE SCÈNE LORS DU DÉPLACEMENT DE L'EXCITATEUR ???
                // DANS CHAQUE SCÈNE LORS DU DÉPLACEMENT DE L'EXCITATEUR ???
                // DANS CHAQUE SCÈNE LORS DU DÉPLACEMENT DE L'EXCITATEUR ???
                // DANS CHAQUE SCÈNE LORS DU DÉPLACEMENT DE L'EXCITATEUR ???
                /*
                if (auto scene = areaE->GetConcernedScene())
                {
                    
                }
                else
                    throw std::logic_error("Scene must be provided within the currently processed event");
                */
                
                
                
                break;
                
            default:
                break;
        }
    }
}



