/*
  ==============================================================================

    Math.h
    Created: 7 Apr 2016 3:45:50pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MIAMMATH_H_INCLUDED
#define MIAMMATH_H_INCLUDED

#include "JuceHeader.h"

#include "boost/geometry.hpp"
#include "boost/geometry/geometries/geometries.hpp"
#include "boost/geometry/geometries/polygon.hpp"

typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;

#define M_PI 3.14159265358979323846264338327 // not defined in VS 2015 community !


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
        
		static Point<double> ComputeIntersectionPoint(Point<double> A, Point<double> B, Point<double> C, Point<double> D);

		static bpt ComputeIntersectionPoint(bpt A, bpt B, bpt C, bpt D);
   };
    
    
}


#endif  // MIAMMATH_H_INCLUDED
