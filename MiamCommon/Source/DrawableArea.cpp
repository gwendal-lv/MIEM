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

DrawableArea::DrawableArea(int64_t _Id, bpt _center, Colour _fillColour)
{
	Id = _Id;

	bcenter = _center;

	centerCircleRadius = 5;

	fillColour = _fillColour;
	fillOpacity = 1.0;

	contourColour = Colours::white;
	contourWidth = 2.0f;
	centerContourWidth = contourWidth*1.5f;

	isNameVisible = true; // par défaut
}


void DrawableArea::Paint(Graphics& g)
{
    g.setColour(contourColour);
    g.drawEllipse((float)bcenterInPixels.get<0>()-centerCircleRadius,
		(float)bcenterInPixels.get<1>()-centerCircleRadius,
        centerCircleRadius*2.0f, centerCircleRadius*2.0f, centerContourWidth);
    
    String name = String("[[[undefined ") + String(Id) + String("]]]");
    
    if (isNameVisible)
    {
        g.setColour(Colours::black); // black shadow
        g.drawSingleLineText(name,
                             (int)bcenterInPixels.get<0>()+1,
                             (int)(bcenterInPixels.get<1>()-centerCircleRadius*2+1));
        g.setColour(Colours::white); // white text
        g.drawSingleLineText(name,
                             (int)bcenterInPixels.get<0>(),
                             (int)(bcenterInPixels.get<1>() -centerCircleRadius*2));
    }
}

void DrawableArea::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    parentCanvas = _parentCanvas;
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scale(parentCanvas->getWidth(), parentCanvas->getHeight());
	boost::geometry::transform(bcenter, bcenterInPixels, scale);
}



void DrawableArea::SetFillColour(Colour newColour)
{
    fillColour = newColour;
}


