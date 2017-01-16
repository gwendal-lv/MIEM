/*
  ==============================================================================

    Exciter.cpp
    Created: 11 Jan 2017 2:36:45pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Exciter.h"

using namespace Miam;

Exciter::Exciter()
:
EditablePolygon(-1, Point<double>(0.5,0.4), 4, 0.05f, Colours::white, 1.0f), // special id = -1
volume(0.0)
{
    
    // Plain white colour inside
    fillOpacity = 1.0f;
    // Black outside (inverted)
    contourColour = Colours::black;
    
    SetNameVisible(false);
    
    SetActive(true);
    SetEnableTranslationOnly(false);
}
