/*
  ==============================================================================

    AudioUtils.hpp
    Created: 1 Nov 2017 6:40:11pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <cmath>

#include "AudioDefines.h"


namespace Miam
{
    
    template<typename T>
    struct GenericExcitement {
        T Linear{};
        T Audio{};
    };
    typedef struct GenericExcitement<double> Excitement;
    
    
    template<typename T>
    class AudioUtils
    {
        public :
        
        static bool IsVolumeDifferenceSignificant(T referenceVolume, T volumeToCompare)
        {
            // On calcule la différence entre les deux en dB
            // Pas de test du log10 qui va être infini... Bien géré par le processeur de mon macbook....
            T referenceVolume_dB = (T)(20.0) * std::log10(referenceVolume);
            T volumeToCompare_dB = (T)(20.0) * std::log10(volumeToCompare);
            
            // Puis on compare au seuil en #define
            return ( std::abs(referenceVolume_dB - volumeToCompare_dB) > (T)Miam_SignificantVolumeDifference_dB );
        }
        
        /// \brief Calcule si la valeur est petite que le define 'Miam_MinVolume'
        static bool IsVolumeNegligible(T volume)
        {
            return std::abs(volume) < ((T)Miam_MinVolume) ;
        }
        
        /// \brief Calcule si la valeur est plus de 10 fois plus petite que le define 'Miam_MinVolume'
        static bool IsVolumeVeryNegligible(T volume)
        {
            return std::abs(volume) < (Miam_MinVolume / (T)10.0);
        }
        
        /// \brief Transforme un volume normalisé en un autre volume normalisé,
        /// via une fonction puissance dont la dérivée seconde est paramétré par un #define
        static T ApplyLowVolumePrecisionDistorsion(T inputVolume)
        {
            // Calcul optimisé du dénominateur (constant avec le define)
            const int denom = (1 << (int)Miam_LowVolumePrecisionFactor) - 1; // 2^facteur - 1
            T num = std::pow(2, inputVolume * (T)Miam_LowVolumePrecisionFactor);
            return num / (T)denom;
        }
        
    };
    
}

