/*
  ==============================================================================

    TextUtils.h
    Created: 11 Oct 2017 8:36:32pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <string>

#include "JuceHeader.h"

#define Miam_OscIntInAddressTag     "[int]"
#define Miam_OscFloatInAddressTag   "[float]"

namespace Miam {
    
    class TextUtils
    {
        private :
        static std::string oscIntInAddressTag;
        static std::string oscFloatInAddressTag;
        
        public :
        
        /// \brief Finds a filename from a single-line set of arguments given to the program as
        /// command-line arguments. The filename is the value given after the "-session" option.
        ///
        /// \returns The filename without quote marks, or an empty string if nothing was found
        static std::string FindFilenameInCommandLineArguments(std::string commandLineToParse);
        
        /// \brief Sépare une chaîne de caractères du type "/adresseOSC/et/des/choses [int] 2
        /// [int] 647 .... [float] 45.2 [float] 32.5 ...." vers un message Osc
        ///
        /// Seul l'espace peut servir de séparateur
        static OSCMessage ParseStringToJuceOscMessage(const std::string& stringToParse);
    };
}
