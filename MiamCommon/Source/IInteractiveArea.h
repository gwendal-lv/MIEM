/*
  ==============================================================================

    IInteractiveArea.h
    Created: 8 Oct 2016 11:37:34am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IINTERACTIVEAREA_H_INCLUDED
#define IINTERACTIVEAREA_H_INCLUDED


#include "IDrawableArea.h"

namespace Miam
{
    
    /// \brief Pure interface for all areas that can have an interaction
    /// with a user (via mouse, touch, MIDI events...). See Miam::InteractiveArea
    class IInteractiveArea : public virtual IDrawableArea
    {
        
        
        // Construction/destruction
        public :
        IInteractiveArea() {}
        virtual ~IInteractiveArea() {}
        
        
        // Interactions computing
        public :
        
        /// \brief Collision test : tells whether the given point is inside the area
        /// or not
        virtual bool HitTest(const Point<double>& hitPoint) = 0;
 
        /// \brief Computes a coefficient that quantifies the interaction between a
        /// given point and this area.
        ///
        /// This coefficient is called the "interaction weight", and is guaranteed to be a
        /// spatial-continuous variable.
        ///
        /// \param T The "hit point" at which the amount of interaction is computed.
        /// \return The interaction weight in [0.0 ; 1.0]
        virtual double ComputeInteractionWeight(Point<double> T) = 0;
        
        
    };
    
    
}




#endif  // IINTERACTIVEAREA_H_INCLUDED
