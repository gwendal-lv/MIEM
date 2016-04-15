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
    
    
    class InteractiveArea
    {
        
        // Construction
        public :
        virtual ~InteractiveArea();
        
        
        // Interactions computing
        public :
        virtual double ComputeInteractionWeight(Point<double> T) = 0;
        
        // Common attributes
        protected :
        std::vector<Point<double>> contourPointsInPixels;
        
    };
    
    
}



#endif  // INTERACTIVEAREA_H_INCLUDED
