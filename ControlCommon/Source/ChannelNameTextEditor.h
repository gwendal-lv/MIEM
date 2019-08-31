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
    class ChannelNameTextEditor : public TextEditor {
        
        // =========== ATTRIBUTES ==========
        private :
        bool isValid = false;
        
        
        // =========== METHODS ==========
        // ction and dtion
        public :
        ChannelNameTextEditor(const String &componentName)
        :
        TextEditor (componentName)
        {
            
        }
        
        
        void SetIsNameValid(bool _isValid)
        {
            isValid = _isValid;
            resetFontAndColour();
        }
        
        
        virtual void enablementChanged() override
        {
            resetFontAndColour();
        }
        
        private :
        void resetFontAndColour()
        {
            if (isValid)
            {
                Font textEditorFont = getFont(); // copie
                textEditorFont.setBold(true); // si pas d'exception, on met en gras
                applyFontToAllText(textEditorFont, false); // ne devient pas la font actuelle
                if (isEnabled())
                    applyColourToAllText(Colours::white);
                else
                    applyColourToAllText(Colours::lightgrey);
            }
            else
            {
                applyFontToAllText(getFont()); // sinon on laisse normal
                if (isEnabled())
                    applyColourToAllText(Colours::orangered);
                else
                    applyColourToAllText(Colours::lightgrey);
            }
        }
        
    };
}
        
