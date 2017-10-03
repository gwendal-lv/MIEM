/*
  ==============================================================================

    InteractiveEllipse.cpp
    Created: 2 Feb 2017 11:52:00am
    Author:  ayup1

  ==============================================================================
*/

#include "InteractiveEllipse.h"
#include "SceneCanvasComponent.h"
#include "MiamMath.h"
using namespace Miam;


InteractiveEllipse::InteractiveEllipse(int64_t _Id) 
	: DrawableEllipse(_Id)
{
	init();
}



InteractiveEllipse::InteractiveEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio)
	: DrawableEllipse(_Id, _center, _a, _b, _fillColour, _canvasRatio)
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
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(parentCanvas->getWidth(), parentCanvas->getHeight());
	boost::geometry::transform(contourPoints, contourPointsInPixels, scaler);

	// Finally, we update sub triangles
	
	computeSurface(); // mettre dans updateSubTriangle?
}

void InteractiveEllipse::computeSurface()
{
	surface = a * b * M_PI;
}

bool InteractiveEllipse::HitTest(double x, double y)
{
	return (contour.contains((float)x, (float)y));
}

double InteractiveEllipse::ComputeInteractionWeight(bpt T)
{
	// calculer l'interaction
	return 0.0;
}

