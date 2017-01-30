/*
  ==============================================================================

    MultiCanvasComponent.h
    Created: 22 Nov 2016 5:40:30pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
        
        std::vector<ScopedPointer<MultiSceneCanvasComponent>> multiSceneCanvasComponents;
        
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
