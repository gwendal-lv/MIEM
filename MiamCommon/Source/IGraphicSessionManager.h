/*
  ==============================================================================

    IGraphicSessionManager.h
    Created: 21 Nov 2016 12:10:44pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IGRAPHICSESSIONMANAGER_H_INCLUDED
#define IGRAPHICSESSIONMANAGER_H_INCLUDED



#include "JuceHeader.h"

#include "MultiSceneCanvasInteractor.h"




namespace Miam
{
    // pre-declarations for pointers
    class MultiSceneCanvasInteractor;
    
    
    class IGraphicSessionManager {
        
        
        
        public :
        
        virtual ~IGraphicSessionManager();
        
        
        
        // - - - - - Getters and Setters - - - - -
        
        /// \brief When a new area is to be created, this gets the value of the next area ID, and increments it
        virtual uint64_t GetNextAreaId() = 0;
        
        
        
        
        /// - - - - - Running mode - - - - -
        
        /// \brief The reaction of this manager when one its canvas sub-components
        /// changes itself its working mode
        virtual void CanvasModeChanged(CanvasManagerMode) = 0;
        
        
        
        
        
        // ------ canvases managing ------
        
        public :
        /// \brief Sets the new active canvas and updates corresponding graphic
        /// objects. Must be called by the newly selected canvas itself.
        virtual void SetSelectedCanvas(MultiSceneCanvasInteractor*) = 0;
        
        
        
        
        
        /// \brief Displays an informative message sent by one of the managed
        /// objects (Miam::EditableScene, Miam::MultiSceneCanvasEditor,
        /// Miam::InteractiveArea, ...)
        virtual void DisplayInfo(String info) = 0;
        
        
        
    };
    
}





#endif  // IGRAPHICSESSIONMANAGER_H_INCLUDED
