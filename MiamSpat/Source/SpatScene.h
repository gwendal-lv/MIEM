/*
  ==============================================================================

    SpatScene.h
    Created: 11 Jan 2017 10:50:21am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATSCENE_H_INCLUDED
#define SPATSCENE_H_INCLUDED

#include "EditableScene.h"


namespace Miam {
    
    
    /// NÉCESSAIRE ? LE MIAM EDIT DEVRA LUI AUSSI FAIRE BOUGER DES EXCITATEURS...
    /// MAIS PEUT-ÊTRE LES EXCITATEURS INITIAUX ALORS ?
    class SpatScene : public EditableScene
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        
        public :
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        SpatScene(MultiSceneCanvasInteractor* canvasManager_, SceneCanvasComponent* canvasComponent_);
        virtual ~SpatScene();
        
        
        // By-pass of "areas editing" methods
        // Only the exciters will be moved ! And not the spat areas
        
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE) override;
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE) override;
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE) override;
    };
    
    
    
}



#endif  // SPATSCENE_H_INCLUDED
