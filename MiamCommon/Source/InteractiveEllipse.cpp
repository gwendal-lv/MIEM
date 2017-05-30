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

	int L1 = contourPoints.outer().size();
	// Pixel contour points
	contourPointsInPixels.clear();
	for (size_t i = 0; i<contourPoints.outer().size(); i++)
		contourPointsInPixels.outer().push_back(bpt(contourPoints.outer().at(i).get<0>()*parentCanvas->getWidth(), 
			contourPoints.outer().at(i).get<1>()*parentCanvas->getHeight()));
	int L2 = contourPoints.outer().size();
	if (L1 != L2)
		DBG("STOOOOOOOOOOOOOOOOOOOOOOOOOP");
	// Finally, we update sub triangles
	
	computeSurface(); // mettre dans updateSubTriangle?
}

void InteractiveEllipse::computeSurface()
{
	surface = a * b * PI;
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

