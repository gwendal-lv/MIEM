/*
  ==============================================================================

    InteractiveArea.h
    Created: 13 Apr 2016 3:56:27pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef INTERACTIVEAREA_H_INCLUDED
#define INTERACTIVEAREA_H_INCLUDED

#include "JuceHeader.h"



namespace Miam
{
    
    /// \brief Abstract class that defines an interface for all areas that can have an interaction
    /// with a user (via mouse, touch, MIDI events...)
    class InteractiveArea
    {
        
        // Construction/destruction
        public :
        virtual ~InteractiveArea();
        
        
        // Interactions computing
        public :
        /// \brief Computes a coefficient that quantifies the interaction between a
        /// given point and this area.
        ///
        /// This coefficient is called the "interaction weight", and is guaranteed to be a
        /// spatial-continuous variable.
        ///
        /// \param T The "hit point" at which the amount of interaction is computed.
        /// \return The interaction weight in [0.0 ; 1.0]
        virtual double ComputeInteractionWeight(Point<double> T) = 0;
        
        // Common attributes
        protected :
        std::vector<Point<double>> contourPointsInPixels; ///< Caracteristic points (coordinates in pixels) along the contour of the area.
        
    };
    
    
}



#endif  // INTERACTIVEAREA_H_INCLUDED
