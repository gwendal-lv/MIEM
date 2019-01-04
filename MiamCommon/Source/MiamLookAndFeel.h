/*
  ==============================================================================

    MiamLookAndFeel.h
    Created: 4 Nov 2017 12:30:16pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace Miam
{
    class MiamLookAndFeel : public LookAndFeel_V4
    {
        
        
        // ===================== ATTRIBUTS ===========================
        private :
        
        float contoursWidth;
        float cornersSize; // pixels

        float mouseOverBrightnessChange;
        
        bool transparentTextBoxBackground;
        
        
        // ===================== MÉTHODES ===========================
        
        
        public :
        // - - - - - - - - - - Construction - - - - - - - - - -
        MiamLookAndFeel(bool _transparentTextBoxBackground = false);
        
        
        
        // - - - - - - - - - - Méthodes spécifiques aux boutons - - - - - - - - - -
        void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                                   bool isMouseOverButton, bool isButtonDown) override;
        
        // - - - - - - - - - - Méthodes spécifiques aux Text Editors - - - - - - - - - -
        void fillTextEditorBackground (Graphics &, int width, int height, TextEditor &) override;
        
    };
}
