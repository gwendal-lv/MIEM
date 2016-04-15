/*
  ==============================================================================

    DrawableArea.cpp
    Created: 26 Mar 2016 12:04:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "DrawableArea.h"

using namespace Miam;


// Common graphics managing code for all possible areas...


DrawableArea::DrawableArea(int64_t _Id, Point<double> _center, Colour _fillColour)
{
    Id = _Id;
    
    center = _center;
    
    centerCircleRadius = 5;
    
    fillColour = _fillColour;
    fillOpacity = 1.0;
    
    contourColour = Colours::white;
    contourWidth = 2.0;
    centerContourWidth = contourWidth*1.5;
}




void DrawableArea::Paint(Graphics& g)
{
    g.setColour(contourColour);
    g.drawEllipse(centerInPixels.x-centerCircleRadius, centerInPixels.y-centerCircleRadius,
                  centerCircleRadius*2, centerCircleRadius*2, centerContourWidth);
    
    String name = String("[[[undefined ") + String(Id) + String("]]]");
    
    g.setColour(Colours::black);
    g.drawSingleLineText(name, centerInPixels.x+1, centerInPixels.y-centerCircleRadius*2+1);
    g.setColour(Colours::white);
    g.drawSingleLineText(name, centerInPixels.x, centerInPixels.y-centerCircleRadius*2);
}

void DrawableArea::CanvasResized(int width, int height)
{
    centerInPixels.setXY( center.x*width, center.y*height);
}



void DrawableArea::SetFillColour(Colour newColour)
{
    fillColour = newColour;
}


