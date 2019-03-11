/*
  ==============================================================================

    ExperimentsLookAndFeel.h
    Created: 11 Mar 2019 1:59:13pm
    Author:  Gwendal

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace Miam
{
    
    class ExperimentsLookAndFeel : public LookAndFeel_V4
    {
        public :
        virtual Font getTextButtonFont(TextButton &, int buttonHeight) override;
    };
    
}
