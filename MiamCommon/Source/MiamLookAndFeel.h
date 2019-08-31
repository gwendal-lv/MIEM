/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
        virtual void fillTextEditorBackground (Graphics &, int width, int height, TextEditor &) override;
        virtual void drawTextEditorOutline (Graphics &, int width, int height, TextEditor &) override;
        
        
        // - - - - - - - - - - Méthodes spécifiques aux Tooltips - - - - - - - - - -
        void drawTooltip (Graphics& g, const String& text, int width, int height) override;
        
        
        // - - - - - internal helpers - - - - -
        private :
        /// \brief Extracted and modified from Juce tooltip v2 CPP file (Juce 5.4.1)
        static TextLayout layoutTooltipText (const String& text, Colour colour, int width);
        
    };
}
