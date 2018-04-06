/*
  ==============================================================================

    InterpolationType.h
    Created: 11 Mar 2018 2:16:52pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace Miam
{
    
    
    
    /// \brief Describes the current method
    enum class InterpolationType {
        
            Any = -1, ///< Special value that represents any of the following types of interpolation
        
            None = 0, ///< Interpolator not configured yet
            
            Matrix_Linear, ///< états matriciels, avec interpolation linéaire basique, coefficient par coefficient
            Matrix_ConstantPower, ///< Interpolation à conservation de puissance audio <=> conservation de la somme des amplitudes au carré (si transitions entre matrices de même amplitude/volume au total). Des déformations logarithmiques sont appliquées pour permettre un contrôle + fin des amplitudes audio.
            Matrix_ConstantAmplitude, ///< Interpolation à conservation d'amplitude (de volume) (si transitions entre matrices de même volume total). Des déformations logarithmiques sont appliquées pour permettre un contrôle + fin des amplitudes audio.
        
            InterpolationTypesCount, ///< Contient le nombre d'interpolations différentes, y compris l'interpolation non-définie
    };
    
    
    class InterpolationTypes {
        
        private :
        // Nom des valeurs de enum InterpolationType juste au-dessus
        // déclarés dans ControlModel.cpp ??? pour supprimer erreurs de link
        constexpr static const char* const interpolationNames[] = {
            "",
            "Linear",
            "Quadratic: constant power transitions",
            "Constant volume transitions"
        };
        constexpr static const char* const interpolationShortNames[] = {
            "",
            "Linear",
            "Quadratic",
            "LinearVolume"
        };
        
        public :
        
        /// \brief Renvoie le nom d'un type d'interpolation sous forme de chaîne de caractère,
        /// traduite et/ou raccourcie si demandé.
        static String GetInterpolationName(InterpolationType type,
                                           bool translate = true, bool shortName = false)
        {
            int typeId = (int)type;
            if (translate)
            {
                if (shortName)
                    return TRANS(interpolationShortNames[typeId]);
                else
                    return TRANS(interpolationNames[typeId]);
            }
            else
            {
                if (shortName)
                    return String(interpolationShortNames[typeId]);
                else
                    return String(interpolationNames[typeId]);
            }
        }
        
        /// \brief Transforme un nom d'interpolation en une valeur d'énumération correspondante,
        /// ou bien lance une Miam::ParseException
        static InterpolationType ParseName(std::string interpolationName);
        
    };
    
    
}
