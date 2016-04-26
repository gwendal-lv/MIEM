/*
  ==============================================================================

    DrawableArea.cpp
    Created: 26 Mar 2016 12:04:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "DrawableArea.h"

#include "SceneCanvasComponent.h"

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
    contourWidth = 2.0f;
    centerContourWidth = contourWidth*1.5f;
}




void DrawableArea::Paint(Graphics& g)
{
    g.setColour(contourColour);
    g.drawEllipse((float)centerInPixels.x-centerCircleRadius,
		(float)centerInPixels.y-centerCircleRadius,
        centerCircleRadius*2.0f, centerCircleRadius*2.0f, centerContourWidth);
    
    String name = String("[[[undefined ") + String(Id) + String("]]]");
    
    g.setColour(Colours::black); // black shadow
    g.drawSingleLineText(name,
		(int)centerInPixels.x+1,
		(int)(centerInPixels.y-centerCircleRadius*2+1));
    g.setColour(Colours::white); // white text
    g.drawSingleLineText(name,
		(int)centerInPixels.x,
		(int)(centerInPixels.y-centerCircleRadius*2));
}

void DrawableArea::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    parentCanvas = _parentCanvas;
    centerInPixels.setXY(center.x*parentCanvas->getWidth(), center.y*parentCanvas->getHeight());
}



void DrawableArea::SetFillColour(Colour newColour)
{
    fillColour = newColour;
}


