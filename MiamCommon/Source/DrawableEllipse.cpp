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


DrawableEllipse::DrawableEllipse(int64_t _Id) :
	DrawableEllipse(_Id, bpt(0.5f, 0.5f),0.2f,0.2f,Colours::darkgrey)
{

}

DrawableEllipse::DrawableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
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

	contourPoints.outer().push_back(bpt(bcenter.get<0>()                 , bcenter.get<1>() - (b / 2)*yScale));
	contourPoints.outer().push_back(bpt(bcenter.get<0>() + (a / 2)*xScale, bcenter.get<1>()));
	contourPoints.outer().push_back(bpt(bcenter.get<0>(), bcenter.get<1>() + (b / 2)*yScale));
	contourPoints.outer().push_back(bpt(bcenter.get<0>() - (a / 2)*xScale, bcenter.get<1>()));
	
	contourPoints.outer().push_back(bpt(bcenter.get<0>(), bcenter.get<1>() - (b / 2)*yScale)); // to close 
	
	
	createJucePolygon();
}

void DrawableEllipse::createJucePolygon(int width, int height)
{
	contour.clear();

	contour.addEllipse((float)bcenter.get<0>() -((float)a*xScale/2), (float)bcenter.get<1>() -((float)b*yScale/2), (float)a*xScale, (float)b*yScale);

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
