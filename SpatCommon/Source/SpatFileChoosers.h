/*
  ==============================================================================

    SpatFileChoosers.h
    Created: 18 Nov 2017 5:06:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


#define Miam_SessionFileExtension       "mspat"


namespace Miam
{
    
    class LoadFileChooser : public FileChooser
    {
        public :
        LoadFileChooser() :
        FileChooser("Please select a MIAM session to load: ",
                    File::getSpecialLocation(File::SpecialLocationType::userMusicDirectory),
                    std::string("*.") + Miam_SessionFileExtension,
                    true)
        {}
    };
    
    
    class SaveFileChooser : public FileChooser
    {
        public :
        SaveFileChooser() :
        FileChooser("Save MIAM session as...",
                    File::getSpecialLocation(File::SpecialLocationType::userMusicDirectory),
                    std::string("*.") + Miam_SessionFileExtension,
                    true)
        {}
    };
    
}
