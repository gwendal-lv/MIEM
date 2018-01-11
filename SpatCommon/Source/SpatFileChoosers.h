/*
  ==============================================================================

    SpatFileChoosers.h
    Created: 18 Nov 2017 5:06:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <boost/predef.h> // OS-specific defines

#include "JuceHeader.h"

#include <string>

#include "PathUtils.h"

#define Miam_SessionFileExtension       "mspat"

namespace Miam
{
    

    class LoadFileChooser : public FileChooser
    {
        public :
        LoadFileChooser() :
        FileChooser("Please select a MIAM session to load: ",
                    PathUtils::GetSessionsFolderDefaultPath(),
                    std::string("*.") + Miam_SessionFileExtension,
                    true)
        {
#if defined( __MIAMOBILE )
            /*
             * Only desktop version can at the moment use these generic Pop-up File Choosers
             */
            //assert(0);
            // Pour l'instant : version DEVELOP de Juce qui permet des filechooser ASYNC natifs iOS/Android
#endif
        }
    };
    
    
    class SaveFileChooser : public FileChooser
    {
        public :
        SaveFileChooser() :
        FileChooser("Save MIAM session as...",
                    PathUtils::GetSessionsFolderDefaultPath(),
                    std::string("*.") + Miam_SessionFileExtension,
                    true)
        {
#if defined( __MIAMOBILE )
            /*
             * Only desktop version can at the moment use these generic Pop-up File Choosers
             */
            //assert(0);
            // Pour l'instant : version DEVELOP de Juce qui permet des filechooser ASYNC natifs iOS/Android
#endif
            
        }
    };
    
    
}
