/*
  ==============================================================================

    MultiSceneCanvasManager.h
    Created: 10 Jan 2017 9:37:54pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MULTISCENECANVASMANAGER_H_INCLUDED
#define MULTISCENECANVASMANAGER_H_INCLUDED


#include "MultiSceneCanvasEditor.h"


namespace Miam {
    
    
    // Simple declarations
    
    
    /// \brief
    class MultiSceneCanvasManager : public MultiSceneCanvasEditor {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        
        
        
        // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
        public :
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // ------ Construction and Destruction ------
        
        // same args as MultiSceneCanvasEditor
        MultiSceneCanvasManager(IGraphicSessionManager* graphicSessionManager_, MultiSceneCanvasComponent* canvasComponent_, SceneCanvasComponent::Id selfId_);
        virtual ~MultiSceneCanvasManager();
        
        // ------ Scenes managing : Add and Delete ------
        virtual void AddScene(std::string name) override;
    };
    
    
}



#endif  // MULTISCENECANVASMANAGER_H_INCLUDED
