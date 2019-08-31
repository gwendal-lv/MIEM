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

#include <limits> // tests avant envoi dans les paquets lock-free

#include "GraphicSessionPlayer.h"

// Other includes

#include "PlayerPresenter.h"
#include "PlayerView.h"

#include "MultiAreaEvent.h"
#include "Exciter.h"

#include "SceneEvent.h"

using namespace Miam;


// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionPlayer::GraphicSessionPlayer(PlayerPresenter* presenter_, PlayerView* view_) :
    GraphicControlSessionManager(presenter_),
    presenter(presenter_),
    view(view_)
{    
    // ========= DEFINITION ICI DU NOMBRE DE CANEVAS ====================
    canvasManagers.push_back(std::make_shared<MultiSceneCanvasPlayer>(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
    // 1 seul canevas depuis la v0.2.5
    //canvasManagers.push_back(std::make_shared<MultiSceneCanvasPlayer>(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas2));
    
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
    
    
    // Resize forcé (pour ne pas avoir de trucs de taille zéro)
    view->ForceResized();
    
}

GraphicSessionPlayer::~GraphicSessionPlayer()
{
}






// ===== SETTERS AND GETTERS =====





// ===== UTILITIES =====




/*
std::shared_ptr<MultiSceneCanvasPlayer> GraphicSessionPlayer::getSelectedCanvasAsManager()
{
    auto canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasPlayer>( selectedCanvas);
    if (canvasPtr)
        return canvasPtr;
    else throw std::runtime_error("Canvas cannot be casted as a Miam::MultiSceneCanvasPlayer");
}
*/

// ===== EVENTS FROM THE PRESENTER ITSELF =====
void GraphicSessionPlayer::HandleEventSync(std::shared_ptr<GraphicEvent> event_)
{
    // Event about one area, or several areas at once
    if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(event_))
    {
        handleAreaEventSync(areaE);
    }
}
void GraphicSessionPlayer::handleAreaEventSync(const std::shared_ptr<AreaEvent>& areaE)
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
void GraphicSessionPlayer::handleSingleAreaEventSync(const std::shared_ptr<AreaEvent>& areaE)
{
    // Event about an Area in particular : we may have to update the spat mix !
    if (auto area = std::dynamic_pointer_cast<ControlArea>(areaE->GetConcernedArea()))
    {
        AsyncParamChange paramChange;
        
        switch (areaE->GetType())
        {
            // On n'envoie l'excitation qu'en mode de jeu réel
                // Sinon ça remplirait la lock-free queue du Presenter
            case AreaEventType::ExcitementAmountChanged :
                if (presenter->getAppMode() == PlayerAppMode::Playing)
                {
					// Et on n'envoie que les excitations liées réellement à un état de spat
					if (area->GetStateIndex() >= 0)
					{
                        paramChange = buildExcitementParamChange(area);
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
void GraphicSessionPlayer::OnModelStarted()
{
    for (size_t i = 0 ; i <canvasManagers.size() ; i++)
        canvasManagers[i]->SelectScene(0);
}


bool GraphicSessionPlayer::SelectSceneOfSelectedCanvas(size_t sceneId)
{
    if (sceneId < selectedCanvas->GetScenesCount())
    {
        selectedCanvas->SelectScene((int) sceneId );
        std::cout << "-----> Scene #" << sceneId << " selected from interprocess connection." << std::endl;
        return true;
    }
    else
    {
        std::cout << "-----> WRONG SCENE INDEX (#" << sceneId << ") selected from interprocess connection." << std::endl;
        return false;
    }
}



void GraphicSessionPlayer::CanvasModeChanged(CanvasManagerMode /*canvasMode*/)
{
    
}





// ===== EVENTS TO VIEW =====

void GraphicSessionPlayer::DisplayInfo(String info, int priority)
{
    view->DisplayInfo(info, (priority > 50));
}



// = = = = = = = = = = XML import/export = = = = = = = = = =

void GraphicSessionPlayer::SetFromTree(bptree::ptree& graphicSessionTree)
{
    GraphicControlSessionManager::SetFromTree(graphicSessionTree);
    
    // Plus aucun traitement supplémentaire, pour l'instant...
}






