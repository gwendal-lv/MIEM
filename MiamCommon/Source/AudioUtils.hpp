/*
  ==============================================================================

    AudioUtils.hpp
    Created: 1 Nov 2017 6:40:11pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <cmath>
#include <vector>
#include <string>

#include "AudioDefines.h"
#include "InteractionParameters.h"

namespace Miam
{
    
    template<typename T>
    struct GenericExcitement {
        T Linear{};
        T Audio{};
    };
    typedef struct GenericExcitement<double> Excitement;
    
    
    struct InOutChannelsName
    {
        public :
        std::vector<std::string> Inputs;
        std::vector<std::string> Outputs;
        
        /// \brief Opérateur d'assignation surchargé pour garder les données des E/S d'indices
        /// plus grands que les indices des E/S à assigner
        InOutChannelsName& operator=(const InOutChannelsName& copy)
        {
            for (size_t i=0 ; i<copy.Inputs.size() ; i++)
                this->Inputs[i] = copy.Inputs[i];
            for (size_t i=0 ; i<copy.Outputs.size() ; i++)
                this->Outputs[i] = copy.Outputs[i];
            return *this;
        }
    };
    
    
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
        /// via une fonction puissance 2^(volume*facteur)
        static T ApplyLowVolumePrecisionDistorsion(T inputVolume,
                                                   T precisionFactor = InteractionParameters::LowVolumePrecisionFactor)
        {
            return (std::pow(2, inputVolume * precisionFactor) - (T)1.0)
                    / (std::pow(2, precisionFactor) - (T)1.0);
        }
        
    };
    
}

