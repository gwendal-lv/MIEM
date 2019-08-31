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

#ifndef PLAYSCENE_H_INCLUDED
#define PLAYSCENE_H_INCLUDED

#include "EditableScene.h"


namespace Miam {
    
    
    /// NÉCESSAIRE ? LE MIAM EDIT DEVRA LUI AUSSI FAIRE BOUGER DES EXCITATEURS...
    /// MAIS PEUT-ÊTRE LES EXCITATEURS INITIAUX ALORS ?
    class PlayScene : public EditableScene
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        
        public :
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        PlayScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_);
        virtual ~PlayScene();
        
        
        // By-pass of "areas editing" methods
        // Only the exciters will be moved ! And not the spat areas
        
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE) override;
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE) override;
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE) override;
    };
    
    
    
}



#endif  // PLAYSCENE_H_INCLUDED
