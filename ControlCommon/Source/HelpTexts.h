/*
  ==============================================================================

    HelpTexts.h
    Created: 4 Jan 2019 11:02:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
    

namespace Miam
{
    class HelpTexts {
        
        public :
        
        
        /// \brief Generic help text
        static juce::String GetHideHelpButton()
        {return TRANS("[-]     Hide help information");}
        /// \brief Generic help text
        static juce::String GetShowHelpButton()
        {return TRANS("[+]     Show help information");}
        
        
        /// \brief Help content related to the playable/editable/interactive scenes
        static juce::String GetScenesGeneralPresentation()
        {return TRANS("voila \n sur 2 lignes \n\n\n voir bien mieux que cela selon ce que l'on dasirae en faire ?");}
        
        
    };
}
