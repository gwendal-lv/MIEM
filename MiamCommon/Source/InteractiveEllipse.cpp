/*
  ==============================================================================

    InteractiveEllipse.cpp
    Created: 2 Feb 2017 11:52:00am
    Author:  ayup1

  ==============================================================================
*/

#include "InteractiveEllipse.h"
#include "SceneCanvasComponent.h"

using namespace Miam;


InteractiveEllipse::InteractiveEllipse(int64_t _Id) 
	: DrawableEllipse(_Id)
{
	init();
}

InteractiveEllipse::InteractiveEllipse(int64_t _Id, Point<double> _center, double _a, double _b, Colour _fillColour, float _canvasRatio)
	: DrawableEllipse(_Id,_center,_a,_b,_fillColour,_canvasRatio)
{
	init();
}

void InteractiveEllipse::init()
{
	// foyer, points touchables, ..
}

void InteractiveEllipse::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
	DrawableEllipse::CanvasResized(_parentCanvas);


	// Pixel contour points
	contourPointsInPixels.clear();
	for (size_t i = 0; i<contourPoints.size(); i++)
		contourPointsInPixels.push_back(Point<double>(contourPoints[i].x*parentCanvas->getWidth(), 
			contourPoints[i].y*parentCanvas->getHeight()));

	// Finally, we update sub triangles
	
	computeSurface(); // mettre dans updateSubTriangle?
}

void InteractiveEllipse::computeSurface()
{
	surface = a * b * PI;
}

bool InteractiveEllipse::HitTest(const Point<double>& hitPoint)
{
	return (contour.contains((float)hitPoint.x, (float)hitPoint.y));
}

double InteractiveEllipse::ComputeInteractionWeight(Point<double> T)
{
	// calculer l'interaction
	return 0.0;
}