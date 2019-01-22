/*
  ==============================================================================

    PlayerHelpContent.h
    Created: 22 Jan 2019 11:06:46am
    Author:  Gwendal

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "AppPurpose.h"


namespace Miam {
    
    
    class PlayerHelpContent {
        
        public :
        
        
        /// \brief Help content displayed on the main menu a a mobile app
        static juce::String GetMainHelp(AppPurpose appPurpose);
        
    };

}
