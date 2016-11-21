/*
  ==============================================================================

    MultiSceneCanvasEditor.h
    Created: 20 Nov 2016 11:06:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MULTISCENECANVASEDITOR_H_INCLUDED
#define MULTISCENECANVASEDITOR_H_INCLUDED



#include "MultiSceneCanvasInteractor.h"


using namespace Miam;

namespace Miam {
    // Pre-declaration of pointers
    
    
    
    
    /// \brief
    class MultiSceneCanvasEditor : public MultiSceneCanvasInteractor
    {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        
        
        public :
        
        MultiSceneCanvasEditor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId);
        
        
        
        
        // ----- Running Mode -----
        
        
        // ------ Setters and Getters ------
        
        virtual std::shared_ptr<IEditableArea> GetSelectedArea();
        
        // ------ Areas managing : Add and Delete ------
        
        virtual void DeleteSelectedArea();
        
        
        // ---------- Events from a canvas component (from View) ----------
        public :
        
        virtual void OnNewColour(Colour colour);
        
        virtual void OnSendBackward();
        virtual void OnSendToBack();
        virtual void OnBringForward();
        virtual void OnBringToFront();
        
        
        
    };
    
}





#endif  // MULTISCENECANVASEDITOR_H_INCLUDED
