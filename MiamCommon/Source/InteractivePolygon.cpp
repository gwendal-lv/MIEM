/*
  ==============================================================================

    InteractivePolygon.cpp
    Created: 13 Apr 2016 3:56:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InteractivePolygon.h"
#include "SceneCanvasComponent.h"

#include "MiamMath.h"

using namespace Miam;


// ===== CONSTRUCTORS & DESTRUCTORS =====

InteractivePolygon::InteractivePolygon(int64_t _Id) :
DrawablePolygon(_Id)
{
    init();
}

InteractivePolygon::InteractivePolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
DrawablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
    init();
}

InteractivePolygon::InteractivePolygon(int64_t _Id, bpt _center, bpolygon& _contourPoints, Colour _fillColour) :
DrawablePolygon(_Id, _center, _contourPoints, _fillColour)
{
    init();
}



void InteractivePolygon::init()
{
    // updateSubTriangles(); NO : can't build them ! No contour Points yet
}




void InteractivePolygon::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    DrawablePolygon::CanvasResized(_parentCanvas);
    
    
    // Pixel contour points
	bcontourPointsInPixels.clear();
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scale(parentCanvas->getWidth(), parentCanvas->getHeight());
	boost::geometry::transform(bcontourPoints, bcontourPointsInPixels, scale);
    
    // Finally, we update sub triangles
    updateSubTriangles();
	computeSurface(); // mettre dans updateSubTriangle?
}



// ===== Updates =====
void InteractivePolygon::updateSubTriangles()
{
    // Reinitializes the whole list
    subTriangles.clear();
	
	// We begin by the annoying one
	subTriangles.push_back(SubTriangle(bcenterInPixels, bcontourPointsInPixels.outer().back(), bcontourPointsInPixels.outer().front()));
	// Then add the others
	for (size_t i = 0; i <bcontourPointsInPixels.outer().size() - 1; i++)
	{
		subTriangles.push_back(SubTriangle(bcenterInPixels, bcontourPointsInPixels.outer().at(i), bcontourPointsInPixels.outer().at(i+1)));
	}
}

void InteractivePolygon::computeSurface()
{
	double S = 0;
	for (size_t i = 0; i < subTriangles.size(); ++i)
		S += subTriangles[i].getSurface();
	surface = S;
}


// ===== INTERACTION COMPUTING =====


bool InteractivePolygon::HitTest(double x, double y)
{
    return (contour.contains((float)x, (float)y));
}



double InteractivePolygon::ComputeInteractionWeight(bpt T)
{
    double weight = 0.0;
    bpt GT(T.get<0>() - bcenterInPixels.get<0>(), T.get<1>() - bcenterInPixels.get<1>());
    
    // if at center (at 0.5²pixel²) (to prevent 0/0 operations)
    if (boost::geometry::distance(bpt(0,0),GT)<0.25)//GT.getDistanceSquaredFromOrigin() < 0.25)
        weight = 1.0;
    // else, we can compute an angle using atan and the 4 quadrants
    else
    {
        double angle = Math::ComputePositiveAngle(GT);
        weight = findSubTriangle(angle).ComputeInteractionWeight(T);
    }
    
    return weight;
}

SubTriangle& InteractivePolygon::findSubTriangle(double angle)
{
    int i = 0;
    while (!subTriangles[i].ContainsAngle(angle))
        i++;
    
    return subTriangles[i];
}

double InteractivePolygon::GetSurface()
{
	return surface;
}