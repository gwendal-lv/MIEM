/*
  ==============================================================================

    MultiSceneCanvasPlayer.cpp
    Created: 10 Jan 2017 9:37:54pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MultiSceneCanvasPlayer.h"

#include "MultiSceneCanvasComponent.h"

#include "PlayScene.h"


using namespace Miam;

MultiSceneCanvasPlayer::MultiSceneCanvasPlayer(IGraphicSessionManager* graphicSessionManager_, MultiSceneCanvasComponent* canvasComponent_, SceneCanvasComponent::Id selfId_)
:
MultiSceneCanvasInteractor(graphicSessionManager_, canvasComponent_, selfId_)
{
}

MultiSceneCanvasPlayer::~MultiSceneCanvasPlayer()
{
    
}

void MultiSceneCanvasPlayer::AddScene(std::string name, bool selectNewScene)
{
    // We construct a scene that DOES NOT allow the selection of a particular area
    std::shared_ptr<EditableScene> newScene = std::make_shared<PlayScene>(shared_from_this(), canvasComponent->GetCanvas());
    newScene->SetName(name);
    
    MultiSceneCanvasInteractor::AddScene(newScene, selectNewScene);
}




// = = = = = Gestion des évènements des scènes = = = = =

void MultiSceneCanvasPlayer::processSingleAreaEventSync(std::shared_ptr<AreaEvent>& areaE)
{
    MultiSceneCanvasInteractor::processSingleAreaEventSync(areaE);
    
    // - - - Traitement des évènements excitateurs - - -
    // ne concernent pas forcément que la scène actuelle sélectionnée !!
    if ( auto exciter = std::dynamic_pointer_cast<Exciter>( areaE->GetConcernedArea() ) )
    {
        // Ajout, suppression, et déplacement (qui est juste la translation pour l'instant)
        // On teste toutes les aires graphiques de la scène LIÉE À L'EXCITATEUR
        // si on est dans un de ces cas...
        /*
        switch (areaE->GetType()) {
            case AreaEventType::Added :
            case AreaEventType::Deleted :
            case AreaEventType::Translation :
                break;
                
            default:
                break;
        }
         */
    }
}



