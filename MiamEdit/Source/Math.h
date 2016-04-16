/*
  ==============================================================================

    Math.h
    Created: 7 Apr 2016 3:45:50pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include "JuceHeader.h"


//#define M_PI 3.14159265358979323846264338327 // not defined in VS 2015 community !


namespace Miam
{
    
    
    
    class Math {
        
        
        public :
        
        // Positive Modulo (for inputValue >0 or <0 but modulo>0)
        static int Modulo(int inputValue, int modulo);
        
        /// <summary>
        /// Result in Radians.
        ///
        /// Self-made function to be sure of the numerical behavior.
        /// </summary>
        /// <param name="v">Must be non-zero</param>
        /// <returns></returns>
        static double ComputePositiveAngle(Point<double> p);
        
   };
    
    
}


#endif  // MATH_H_INCLUDED
