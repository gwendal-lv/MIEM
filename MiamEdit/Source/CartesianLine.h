/*
  ==============================================================================

    CartesianLine.h
    Created: 8 Apr 2016 11:48:11am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef CARTESIANLINE_H_INCLUDED
#define CARTESIANLINE_H_INCLUDED


#include "JuceHeader.h"



namespace Miam {
    
    
    class CartesianLine
    {
        
        private :
        double a, b, c; // coefficient de l'équation ax + by + c = 0
        
        
        
        public :
        CartesianLine(Point<double> point1, Point<double> point2);
        bool PointWentThrough(Point<double> previousLocation, Point<double> nextLocation);
        
        
    };
    
    
}



#endif  // CARTESIANLINE_H_INCLUDED
