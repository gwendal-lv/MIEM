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

// à nouveau des bugs/comportement bizarres en décembre 2018....
//#define M_PI (3.14159265358979323846264338327) // not defined in VS 2018 community !
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
        
        
        static Matrix3D<float> GenerateLookAtMatrix(Vector3D<float> eye, Vector3D<float> center, Vector3D<float> up)
        {
            Vector3D<float> zaxis = (eye - center).normalised();
            Vector3D<float> xaxis = (zaxis ^ up).normalised();
            Vector3D<float> yaxis = -(xaxis ^ zaxis);
            
            Matrix3D<float> viewMatrixTr(xaxis.x, yaxis.x, zaxis.x, 0,
                                         xaxis.y, yaxis.y, zaxis.y, 0,
                                         xaxis.z, yaxis.z, zaxis.z, 0,
                                         -(xaxis * eye), -(yaxis * eye), -(zaxis*eye), 1);
            return viewMatrixTr;
        }
        
        static Matrix3D<float> GenerateOrthoPerspectiveMatrix(float width, float height, float near, float far)
        {
            // coordinates of the projection rectangle on screen
            const float r = width; // right
            const float l = 0.0f; // left
            const float b = 0.0f; // bottom
            const float t = height; // top
            const float f = far;
            const float n = near;
            
            Matrix3D<float> orthoProj(2.0f / (r-l), 0.0f,               0.0f,         0.0f,
                                      0.0f,         2.0f / (t - b),     0.0f,         0.0f,
                                      0.0f, 0.0f,                      -2.0f / (f-n), 0.0f,
                                      -(r+l) / (r-l), -(t+b) / (t-b), -(f+n) / (f-n), 1.0f);
            
            return orthoProj;
        }
   };
    
    
}


#endif  // MIAMMATH_H_INCLUDED
