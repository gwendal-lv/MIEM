/*
  ==============================================================================

    DrawableEllipse.cpp
    Created: 2 Feb 2017 2:17:07pm
    Author:  ayup1

  ==============================================================================
*/

#include "DrawableEllipse.h"
#include "SceneCanvasComponent.h"

using namespace Miam;
using namespace Amusing;

DrawableEllipse::DrawableEllipse(int64_t _Id) :
	DrawableEllipse(_Id, Point<double>(0.5f, 0.5f),0.2f,0.2f,Colours::darkgrey)
{

}

DrawableEllipse::DrawableEllipse(int64_t _Id, Point<double> _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
	DrawableArea(_Id,_center,_fillColour), a(_a), b(_b)
{
	//float xScale, yScale;
	if (_canvasRatio > 1.0f) // ratio of an landscape-oriented window
	{
		xScale = 1.0f / _canvasRatio;
		yScale = 1.0f;
	}
	else // ratio of an portrait-oriented window
	{
		xScale = 1.0f;
		yScale = 1.0f*_canvasRatio;
	}

	DBG("a scale = " + (String)((a / 2)*xScale));
	DBG("b scale = " + (String)((b / 2)*yScale));
	
	contourPoints.push_back(Point<double>(center.x                 , center.y - (b / 2)*yScale));
	contourPoints.push_back(Point<double>(center.x + (a / 2)*xScale, center.y                 ));
	contourPoints.push_back(Point<double>(center.x                 , center.y + (b / 2)*yScale));
	contourPoints.push_back(Point<double>(center.x - (a / 2)*xScale, center.y                 ));
	
	
	
	
	createJucePolygon();
}

void DrawableEllipse::createJucePolygon(int width, int height)
{
	contour.clear();

	DBG("(xc,yc) = (" + (String)centerInPixels.getX() + "," + (String)centerInPixels.getY() + ")");
	DBG("width = " + (String)(a));
	DBG("height = " + (String)(b));

	contour.addEllipse(center.x -(a*xScale/2), center.y-(b*yScale/2), a*xScale, b*yScale);

	contour.applyTransform(AffineTransform::scale((float)width, (float)height));
}

DrawableEllipse::~DrawableEllipse()
{
}

// Called by the parent component (which is a canvas)
void DrawableEllipse::Paint(Graphics& g)
{
	//DBG("DrawableEllipse::paint");
	g.setColour(fillColour);
	g.setOpacity(fillOpacity);
	g.fillPath(contour);

	g.setColour(contourColour);
	g.strokePath(contour, PathStrokeType(contourWidth));

	// Parent's drawings on top of these ones
	DrawableArea::Paint(g);
}

void DrawableEllipse::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
	DrawableArea::CanvasResized(_parentCanvas);

	createJucePolygon(parentCanvas->getWidth(), parentCanvas->getHeight());
}