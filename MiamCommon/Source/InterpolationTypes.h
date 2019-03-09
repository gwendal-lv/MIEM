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
    // - - - - - - - - - - FULL MATRICES interpolation - - - - - - - - - -
    
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
        // déclarés dans le .cpp
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
        
        /// \brief Return true if the interpolation type an actual one (e.g. linear, constant power, ...)
        /// and not a "utility" one (e.g. None, Count, ...)
        static bool IsActualInterpolationType(InterpolationType type)
        {
            return (static_cast<int>(InterpolationType::None) < static_cast<int>(type))
            && (static_cast<int>(type) < static_cast<int>(InterpolationType::InterpolationTypesCount));
        }
        
    };
    
    
    
    
    
    // - - - - - - - - - - INDEPENDANT PARAMETERS interpolation - - - - - - - - - -
    
    /// \brief Describes the interpolation method for a single param
    enum class ParamInterpolationType {
        
        Any = -1, ///< Special value that represents any of the following types of interpolation
        
        None = 0, ///< Interpolator not configured yet
        
        Independant_Threshold, ///< On/Off curve, threshold at (max-min)/2.0
        
        Independant_Exp, ///< Interpolation à base d'exponentielle: courbe très molle qui met du temps à aller jusqu'aux valeurs maximales. Adaptée pour l'interpolation de fréquences audio
        Independant_Soft, ///< Courbe un peu plus douce que linéaire (augmente peu au départ)
        Independant_Linear, ///< avec interpolation linéaire basique, coefficient par coefficient
        Independant_Hard, ///< Courbe un peu plus dure que linéaire (augmente beaucoup au départ)
        Independant_Log, ///< Interpolation à base de log: courbe très dure, qui va très vite vers les valeurs maximales
        
        InterpolationTypesCount, ///< Contient le nombre d'interpolations différentes, y compris l'interpolation non-définie
    };
    
    class ParamInterpolationTypes {
        
        private :
        // Nom des valeurs de enum InterpolationType juste au-dessus
        // déclarés dans le .cpp
        constexpr static const char* const interpolationNames[] = {
            "",
            "On/Off curve: threshold at 0.5",
            "Exponential curve",
            "Soft curve",
            "Linear curve: standard for controllers",
            "Hard curve",
            "Logarithmic curve: for audio frequencies"
        };
        constexpr static const char* const interpolationShortNames[] = {
            "",
            "Threshold",
            "Exp",
            "Soft",
            "Linear",
            "Hard",
            "Log"
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
        static ParamInterpolationType ParseName(std::string interpolationName);
        
        /// \brief Return true if the interpolation type an actual one (e.g. linear, constant power, ...)
        /// and not a "utility" one (e.g. None, Count, ...)
        static bool IsActualInterpolationType(ParamInterpolationType type)
        {
            return (static_cast<int>(ParamInterpolationType::None) < static_cast<int>(type))
            && (static_cast<int>(type) < static_cast<int>(ParamInterpolationType::InterpolationTypesCount));
        }
        
    };
    
    
    
    
}
