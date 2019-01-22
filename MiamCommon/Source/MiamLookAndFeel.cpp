/*
  ==============================================================================

    MiamLookAndFeel.cpp
    Created: 4 Nov 2017 12:30:16pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
    // Base code taken from tooltip v4 CPP file, from Juce 5.4.1
    Rectangle<int> bounds (width, height);
    float cornerSize = 2.0f;
    float contourWidth1 = 1.0f;
    float contourWidth2 = 2.0f;

    //g.setColour (findColour (TooltipWindow::backgroundColourId));
    g.setColour(Colours::white);
    g.fillRoundedRectangle (bounds.toFloat(), cornerSize);

    //g.setColour (findColour (TooltipWindow::outlineColourId));
    g.setColour(Colours::darkgrey);
    g.drawRoundedRectangle (bounds.toFloat().reduced (contourWidth1, contourWidth1), cornerSize, contourWidth2);

    layoutTooltipText (text, Colours::darkgrey, width)//findColour (TooltipWindow::textColourId))
    .draw (g, { static_cast<float> (width), static_cast<float> (height) });
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
