/*
  ==============================================================================

    MiamLookAndFeel.cpp
    Created: 4 Nov 2017 12:30:16pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MiamLookAndFeel.h"

using namespace Miam;


MiamLookAndFeel::MiamLookAndFeel()
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
