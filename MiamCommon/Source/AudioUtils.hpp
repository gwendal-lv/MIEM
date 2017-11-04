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
        
    };
    
}

