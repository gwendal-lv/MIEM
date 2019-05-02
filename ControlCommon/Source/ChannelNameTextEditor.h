/*
  ==============================================================================

    ChannelNameTextEditor.h
    Created: 9 Mar 2019 1:16:44pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
        
