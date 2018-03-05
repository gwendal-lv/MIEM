/*
  ==============================================================================

    Math.h
    Created: 7 Apr 2016 3:45:50pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MIAMMATH_H_INCLUDED
#define MIAMMATH_H_INCLUDED

#ifdef _MSC_VER

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for C++

// à nouveau des bugs en janvier 2018....
//#define M_PI (3.14159265358979323846264338327) // not defined in VS 2015 community !
#endif

#include <math.h>
#endif

#include "JuceHeader.h"




#include "IDrawableArea.h"




namespace Miam
{
    
    
    /// \brief Miam-specific and necessary mathematical utility functions.
    ///
    /// Mostly contains static functions or contants definitions.
    class Math {
        
        
        public :
        
        /// \brief Computes a platform-independant modulo operation on
        /// negative integers
        ///
        /// For negative input values (the behavior in such a case is
        /// implementation-specific)
        ///
        /// \return The only modulo result in [[0 ; modulo-1]]
        /// \param inputValue Value to apply the modulo to (<0, may be >0)
        /// \param modulo Modulo operator value (must be >0)
        static int Modulo(int inputValue, int modulo);
        
        /// \brief Computes an angle that is positive for sure
        ///
        /// Self-made function to be sure of the numerical behavior.
        ///
        /// \return Result in Radians.
        /// \param p Any point but the origin
        static double ComputePositiveAngle(Point<double> p);

		static double ComputePositiveAngle(bpt p);
        
        /// \brief Computes the intersection point between the lines AB and CD.
		static Point<double> ComputeIntersectionPoint(Point<double> A, Point<double> B, Point<double> C, Point<double> D);

		static bpt ComputeIntersectionPoint(bpt A, bpt B, bpt C, bpt D);
        
        /// \brief Remet la valeur dans l'intervalle
        ///
        /// Parce que ce n'est pas encore dans la STL avec le C++11...
        template<typename T>
        inline static T Clamp(T value, T lowerBound, T upperBound)
        {
            if (value < lowerBound)
                return lowerBound;
            else if (value > upperBound)
                return upperBound;
            else
                return value;
        }
   };
    
    
}


#endif  // MIAMMATH_H_INCLUDED
