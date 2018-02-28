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

namespace Miam
{
    

    class LoadFileChooser : public FileChooser
    {
        public :
        LoadFileChooser(std::initializer_list<AppPurpose> appTypeArgs) :
        FileChooser("Please select a MIEM session to load: ",
                    PathUtils::GetSessionsFolderDefaultPath(),
                    PathUtils::GenerateAllowedFilePatterns(appTypeArgs),
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
        SaveFileChooser(std::initializer_list<AppPurpose> appTypeArgs) :
        FileChooser("Save MIEM session as...",
                    PathUtils::GetSessionsFolderDefaultPath(),
                    PathUtils::GenerateAllowedFilePatterns(appTypeArgs),
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
