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

#include "IInteractiveArea.h"



namespace Miam
{
    
    /// \brief Abstract class that defines an interface for all areas that can have an interaction
    /// with a user (via mouse, touch, MIDI events...)
    class InteractiveArea : public virtual IInteractiveArea
    {
        
        // Construction/destruction
        public :
        virtual ~InteractiveArea();
        
        
        // Interactions computing (common implementations of the interface here)
        public :

        
        
        // Common attributes
        protected :
        std::vector<Point<double>> contourPointsInPixels; ///< Caracteristic points (coordinates in pixels) along the contour of the area.
        
		protected:
			double surface;
    };
    
    
}



#endif  // INTERACTIVEAREA_H_INCLUDED
