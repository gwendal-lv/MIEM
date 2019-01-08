/*
  ==============================================================================

    TextUtils.h
    Created: 11 Oct 2017 8:36:32pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <string>
#include <algorithm> // std::min

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
        /// Seul l'espace peut servir de séparateur.
        ///
        /// Might throw a Miam::ParseException if necessary
        static OSCMessage ParseStringToJuceOscMessage(const std::string& stringToParse);
        
        template<typename T>
        static std::u16string ConvertNumberToU16string(T number, int numberOfCharactersToDisplay = -1)
        {
            std::u16string stringU16 = u"";
            std::string stringASCII = std::to_string(number);
            
            size_t u16StringFinalLength = stringASCII.length();
            if (numberOfCharactersToDisplay >= 0)
                u16StringFinalLength = std::min(u16StringFinalLength, (size_t(numberOfCharactersToDisplay)));
                
            for (int i = 0; i < u16StringFinalLength; ++i)
                stringU16 += char16_t(stringASCII[i]);
            return stringU16;
        }
        
        
        static std::string GetLimitedDigitsString(double value, int numberOfDigits)
        {
            // On récupère d'abord des infos sur ce nombre
            bool isPositive = (value >= 0.0);
            double valueAbs = isPositive ? value : (-value);
            int nbFiguresBeforeComa;
            if (valueAbs < 1.0)
                nbFiguresBeforeComa = 1; // on compte le zéro pour l'affichage
            else
                // the + 0.00001 is here to compensate for log10(10^x) < x
                // due to numeric approximations (for double on macOS, intel proc...)
                nbFiguresBeforeComa = (int)std::ceil(std::log10(valueAbs + 0.00001));
            
            std::stringstream numberStream;
            numberStream << std::fixed << std::setprecision(numberOfDigits-nbFiguresBeforeComa) << value;
            std::string result = numberStream.str();
            if (isPositive)
                result = "+" + result;
            
            return result;
        }
    };
}
