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
#include "MiamLookAndFeel.h"

using namespace Miam;


MiamLookAndFeel::MiamLookAndFeel(bool _transparentTextBoxBackground) :
    transparentTextBoxBackground(_transparentTextBoxBackground)
{
    contoursWidth = 1.0f;
    cornersSize = 4.0f;
    
    mouseOverBrightnessChange = 0.25f;
}


void MiamLookAndFeel::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                           bool isMouseOverButton, bool isButtonDown)
{
    Rectangle<float> buttonArea = button.getLocalBounds().toFloat();
    
    // On oublie totalement pour l'instant les "connected on top/bottom/left/right", etc.

    // Avec bord blanc pour rappeler les formes géométriques dans le canevas
    g.setColour(Colours::white.withAlpha(0.5f));
    g.fillRoundedRectangle(buttonArea, cornersSize);
    
    // Remplissage simple arrondi
    Colour actualBackgroundColour;
    if (!isMouseOverButton && !isButtonDown) // état normal
        actualBackgroundColour = backgroundColour;
    else if (isButtonDown) // button down : devient transparent (idée : "rentre" dans le background
                                                            // pour déclencher une action...)
        actualBackgroundColour = backgroundColour.withMultipliedAlpha(0.5f);
    else if (isMouseOverButton) // mouse over : plus lumineux, ou - lumineux si ce n'était pas possible
    {
        if (backgroundColour.getBrightness() < (1.0f - mouseOverBrightnessChange))
            actualBackgroundColour = backgroundColour.withBrightness(backgroundColour.getBrightness() + mouseOverBrightnessChange);
        else
            actualBackgroundColour = backgroundColour.withBrightness(backgroundColour.getBrightness() - mouseOverBrightnessChange);
    }
    auto buttonReduced = buttonArea.reduced(contoursWidth);
    g.setColour (actualBackgroundColour);
    g.fillRoundedRectangle(buttonReduced, cornersSize);
}


void MiamLookAndFeel::fillTextEditorBackground (Graphics & g, int /*width*/, int /*height*/,
                                                TextEditor & textEditor)
{
    auto textEditorArea = textEditor.getLocalBounds();
    if (textEditor.isEnabled())
    {
        if (!transparentTextBoxBackground)
            g.setColour(Colours::darkgrey);
        else
            g.setColour(Colours::transparentWhite);
    }
    else
    {
        if (!transparentTextBoxBackground)
            g.setColour(Colours::grey);
        else
            g.setColour(Colours::transparentBlack);
    }
    
    g.fillRect(textEditorArea);
}
void MiamLookAndFeel::drawTextEditorOutline(Graphics & g, int width, int height, TextEditor & textEditor)
{
    // Normal draw if background is not transparent
    if (! transparentTextBoxBackground)
        LookAndFeel_V4::drawTextEditorOutline(g, width, height, textEditor);
    else // else, we don't draw any contour
        return;
}



void MiamLookAndFeel::drawTooltip (Graphics& g, const String& text, int width, int height)
{
    // Retour aux tooltips de base....
    LookAndFeel_V4::drawTooltip(g, text, width, height);
    return;
}

TextLayout MiamLookAndFeel::layoutTooltipText (const String& text, Colour colour, int width)
{
    const float tooltipFontSize = 13.0f;
    const int maxToolTipWidth = width - 20;
    
    AttributedString s;
    s.setJustification (Justification::centred);
    s.append (text, Font (tooltipFontSize, Font::plain), colour);
    
    TextLayout tl;
    tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
    return tl;
}
