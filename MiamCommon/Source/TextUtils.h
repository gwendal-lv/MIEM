/*
  ==============================================================================

    TextUtils.h
    Created: 11 Oct 2017 8:36:32pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <string>

namespace Miam {
    class TextUtils
    {
        public :
        
        /// \brief Finds a filename from a single-line set of arguments given to the program as
        /// command-line arguments. The filename is the value given after the "-session" option.
        ///
        /// \returns The filename without quote marks, or an empty string if nothing was found
        static std::string FindFilenameInCommandLineArguments(std::string commandLineToParse);
    };
}
