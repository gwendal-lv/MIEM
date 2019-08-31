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

#ifndef MULTICANVASCOMPONENT_H_INCLUDED
#define MULTICANVASCOMPONENT_H_INCLUDED

#include <vector>

#include "JuceHeader.h"

#include "SceneCanvasComponent.h"
#include "MultiSceneCanvasComponent.h"

using namespace Miam;

namespace Miam {
    
    // pre-declarations for pointer members
    class IGraphicSessionManager;
    
    
    
    class MultiCanvasComponent : public Component
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        // Link back to manager module
        IGraphicSessionManager* graphicSessionManager;
        
		protected :
        std::vector<std::unique_ptr<MultiSceneCanvasComponent>> multiSceneCanvasComponents;
        
		private :
        // Only one because we know that there is only 2 canvases available...
        float normalizedBarXPos = 0.61803398875f; // inverse du nombre d'or...trololo 
        Rectangle<float> canvasResizeBar;
        Path canvasResizeDiamond;
        AffineTransform canvasResizeDiamondTransform;
        
        // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
        public :
        
        //ScopedPointer<MultiSceneCanvasComponent> GetMultiSceneCanvasComponent(size_t i) {return multiSceneCanvasComponents[i];}
        
        int GetBarXPos() {return juce::roundToInt(canvasResizeBar.getCentreX());}
        
        // Will also call the necessary graphic updates
        void SetBarXPos(int newXPos);
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        MultiCanvasComponent(IGraphicSessionManager* _graphicSessionManager);
        virtual ~MultiCanvasComponent() {};
        
        virtual void paint(Graphics& g) override;
        virtual void resized() override;
        
        void CompleteInitialization();
        
        void PrepareVisible();
        void PrepareUnvisible();
        
		void ReleaseOpengGLResources();
        
		//void update() override;
        
        // - - - - - Mouse management - - - - -
        virtual void mouseDown (const MouseEvent &event) override;
        virtual void mouseDrag (const MouseEvent &event) override;
        virtual void mouseUp (const MouseEvent &event) override;
        
        
        // - - - - - Canvases : Add and Delete - - - - -
        virtual MultiSceneCanvasComponent* AddCanvas();
    };
    
    
}



#endif  // MULTICANVASCOMPONENT_H_INCLUDED
