/*
  ==============================================================================

    IDrawableArea.h
    Created: 7 Oct 2016 11:10:46am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/



// = = = = = = = = = SUPPRESSION D'UN WARNING Visual Studio = = = = = = = = = = =
// Warning dû à l'héritage virtuel... Alors que c'est prévu normalement en C++11 !
#ifdef _MSC_VER // indique que c'est une version de visual studio
	#pragma warning( disable : 4250 )
#endif
// = = = = = = = = = SUPPRESSION D'UN WARNING Visual Studio = = = = = = = = = = =




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
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        IDrawableArea(){}
        virtual ~IDrawableArea() {}
        /// \brief To be overriden within any concrete area that inherits from this.
        virtual IDrawableArea* Clone() const = 0;
        
        
        
        
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
