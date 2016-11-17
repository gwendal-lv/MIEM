/*
  ==============================================================================

    IDrawableArea.h
    Created: 7 Oct 2016 11:10:46am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IDRAWABLEAREA_H_INCLUDED
#define IDRAWABLEAREA_H_INCLUDED

#include "JuceHeader.h"


// Simple declaration for a pointer
class SceneCanvasComponent;



namespace Miam
{
    
    
    /// \brief Interface of the Miam::DrawableArea (which is an abtract object)
    class IDrawableArea
    {
        public :
        
        
        IDrawableArea(){}
        virtual ~IDrawableArea() {}
        
        /// \brief Function called by a SceneCanvasComponent to draw the Area
        ///
        /// The whole painting job is at the moment performed by the CPU only, but should be
        /// replaced by OpenGL drawing soon.
        virtual void Paint(Graphics& g) = 0;
        /// \brief This function should be called when the area is associated to a (new) canvas,
        /// when its canvas is resized, or when a drawing refresh is needed.
        ///
        /// It updates real-screen pixel coordinates from internally stored normalized coordinates
        /// of all points.
        virtual void CanvasResized(SceneCanvasComponent* parentCanvas) = 0;
        
        
        // ----- Setters and Getters -----
        
        virtual int64_t GetId() = 0;
        virtual void SetId(int64_t _Id) = 0;
        virtual Colour GetFillColour() = 0;
        virtual void SetFillColour(Colour newColour) = 0;
        
        
        
    };
    
    
}


#endif  // IDRAWABLEAREA_H_INCLUDED
