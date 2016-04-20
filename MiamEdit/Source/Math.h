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
    
    
    /// \brief Miam-specific and necessary mathematical utility functions.
    ///
    /// Mostly contains static functions or contants definitions.
    class Math {
        
        
        public :
        
        /// \brief Computes a specific modulo operation
        ///
        /// \return The only modulo result in [[0 ; modulo-1]]
        /// \param inputValue Value to apply the modulo to (may be >0 or <0)
        /// \param modulo Modulo operator value (must be >0)
        static int Modulo(int inputValue, int modulo);
        
        /// \brief Computes an angle that is positive for sure
        ///
        /// Self-made function to be sure of the numerical behavior.
        ///
        /// \return Result in Radians.
        /// \param p Any point but the origin
        static double ComputePositiveAngle(Point<double> p);
        
   };
    
    
}


#endif  // MATH_H_INCLUDED
