/*
  ==============================================================================

    IGraphicSessionManager.h
    Created: 21 Nov 2016 12:10:44pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IGRAPHICSESSIONMANAGER_H_INCLUDED
#define IGRAPHICSESSIONMANAGER_H_INCLUDED


#include <memory>

#include "JuceHeader.h"

#include "MultiSceneCanvasInteractor.h"
#include "MultiCanvasComponent.h"




namespace Miam
{
    // pre-declarations for pointers
    class MultiSceneCanvasInteractor;
    
    
    /// \brief Interface for any manager of a Miam graphic session presenter
    ///
    /// Owns the unique MultiCanvasComponent
    class IGraphicSessionManager {
        
        
        
        
        // = = = = = = = = = = COMMON ATTRIBUTES = = = = = = = = = =
        
        protected :
        
        // Unique, so this is the easiest working way...
        MultiCanvasComponent* multiCanvasComponent;
        
        
        // Graphic objects management
        
        /// \brief Unique Id of the next created (or pasted, or loaded...) area
        int64_t nextAreaId;
        
        // Children Canvases
        std::vector< MultiSceneCanvasInteractor* > canvasManagers;
        MultiSceneCanvasInteractor* selectedCanvas = 0;
        
        
        // states backup
        private :
        bool mouseResizingCanvas = false;
        
        
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :

        /// \brief When a new area is to be created, this gets the value of the next area ID, and increments it
        virtual uint64_t GetNextAreaId();
        
        virtual MultiCanvasComponent* GetMultiCanvasComponent() {return  multiCanvasComponent;}
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IGraphicSessionManager();
        virtual ~IGraphicSessionManager();
        
        
        
        
        /// - - - - - Running mode - - - - -
        public :
        
        /// \brief The reaction of this manager when one its canvas sub-components
        /// changes itself its working mode
        virtual void CanvasModeChanged(CanvasManagerMode) = 0;
        
        
        
        
        
        // - - - - - - canvases managing - - - - - -
        
        public :
        /// \brief Sets the new active canvas and updates corresponding graphic
        /// objects. Must be called by the newly selected canvas itself.
        virtual void SetSelectedCanvas(MultiSceneCanvasInteractor*) = 0;
        
        
        
        
        /// \brief Displays an informative message sent by one of the managed
        /// objects (Miam::EditableScene, Miam::MultiSceneCanvasEditor,
        /// Miam::InteractiveArea, ...)
        virtual void DisplayInfo(String info) = 0;
        
        
        // ----- Events from the Presenter itself -----
        virtual void OnSceneChange(std::shared_ptr<EditableScene> newSelectedScene) {}
        
        
        // - - - - - Mouse Events - - - - -
        virtual void OnBackgroundMouseDown(const MouseEvent &event);
        virtual void OnBackgroundMouseDrag(const MouseEvent &event);
        virtual void OnBackgroundMouseUp(const MouseEvent &event);
        
		
        
        
    };
    
}





#endif  // IGRAPHICSESSIONMANAGER_H_INCLUDED
