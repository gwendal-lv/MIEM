/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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



