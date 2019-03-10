/*
  ==============================================================================

    BasicInterpolationCurve.hpp
    Created: 10 Mar 2019 3:05:14pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <utility> // std::swap depuis C++11

#include "InterpolationTypes.h"

namespace Miam
{
    /// \brief Class for fast and basic interpolation of 1-D values between 2 points.
    /// Monotonically increasing functions only;
    ///
    /// Inputs are noted x, outputs are noted y
    ///
    /// \remark Boost of course has some interpolation functionnalities... But here we just need
    /// very simple and basic functionnalities (with custom and simple interface)
    template<typename T>
    class BasicInterpolationCurve
    {
        // ========== ATTRIBUTES ==========
        private :
        const T minX, maxX, minY, maxY;
        const T deltaX, deltaY;
        const T centerX, centerY;
        ParamInterpolationType interpolationType;
        
        
        
        // ========== METHODS ==========
        /// \brief Default constructor, with default values corresponding to normalised
        /// input values and scaled interpolated output values.
        BasicInterpolationCurve(ParamInterpolationType _interpolationType,
                                T _minY, T _maxY, T _minX = (T)0.0, T _maxX = (T)1.0)
        :
        interpolationType(_interpolationType),
        // assignation of const attributes, with check for monotonic increase
        minX( (_minX < _maxX) ? _minX : _maxX ), maxX( (_minX < _maxX) ? _maxX : _minX ),
        minY( (_minY < _maxY) ? _minY : _maxY ), maxY( (_minY < _maxY) ? _maxY : _minY ),
        // pre-computed values, for computation speed
        deltaX(maxX - minX), deltaY(maxY - minY),
        centerX((maxX + minX) / ((T) 2.0)),
        centerY((maxY + minY) / ((T) 2.0))
        {
            assert( (minX < maxX)  && (minY < maxY) ); // monotonically increasing functions only
        }
        
        
        /// \brief Computes the interpolated output (y) value that corresponds
        /// to the given input (x) value
        inline T InterpolateValue(T inputX) const
        {
            // security checks (for debug only ???)
            if (inputX < minX)
                return minY;
            else if (inputX > maxX)
                return maxY;
            
            // actual computation if OK
            else
            {
                switch(interpolationType)
                {
                    case ParamInterpolationType::Independant_Linear :
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        return minY + ((inputX - minX) / deltaX) * deltaY;
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                
                    case ParamInterpolationType::Independant_Threshold :
                        return (inputX < centerX) ? minY : maxY;
                
                    default : //
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        return minY + ((inputX - minX) / deltaX) * deltaY;
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                }
            }
        }
        
    }
}
