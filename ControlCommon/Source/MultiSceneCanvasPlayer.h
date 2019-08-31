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

#ifndef MULTISCENECANVASPLAYER_H_INCLUDED
#define MULTISCENECANVASPLAYER_H_INCLUDED


#include "MultiSceneCanvasInteractor.h"


namespace Miam {
    
    
    // Simple declarations
    
    
    /// \brief
    class MultiSceneCanvasPlayer : public MultiSceneCanvasInteractor {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        
        
        
        // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
        public :
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // - - - - - - Construction and Destruction - - - - - -
        
        // same args as MultiSceneCanvasEditor
        MultiSceneCanvasPlayer(IGraphicSessionManager* graphicSessionManager_, MultiSceneCanvasComponent* canvasComponent_, SceneCanvasComponent::Id selfId_);
        virtual ~MultiSceneCanvasPlayer();
        
        // - - - - - - Scenes managing : Add and Delete - - - - - -
        virtual void AddScene(std::string name, bool selectNewScene = false) override;
        
        
        
        // - - - - - - Gestion des évènements des scènes - - - - - -
        protected :

        /// \brief Après avoir retransmis la commande à la classe parente, sélectionnera les évènements
        /// concernant les aires intéressantes,
        /// puis crée et envoie un évènement
        /// directement au GraphicSessionPlayer concernant les seules aires
        /// dont le degré d'excitation aura changé
        ///
        /// Le graphicsessionmanager traduit et envoie au presenter ce qu'il faut
        virtual void processSingleAreaEventSync(std::shared_ptr<AreaEvent>& areaE) override;
        

    };
    
    
}



#endif  // MULTISCENECANVASPLAYER_H_INCLUDED
