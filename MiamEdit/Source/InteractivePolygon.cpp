/*
  ==============================================================================

    InteractivePolygon.cpp
    Created: 13 Apr 2016 3:56:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InteractivePolygon.h"
#include "SceneCanvasComponent.h"

#include "Math.h"

using namespace Miam;


// ===== CONSTRUCTORS & DESTRUCTORS =====

InteractivePolygon::InteractivePolygon(int64_t _Id) :
DrawablePolygon(_Id)
{
    init();
}

InteractivePolygon::InteractivePolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
DrawablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
    init();
}

InteractivePolygon::InteractivePolygon(int64_t _Id, Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour) :
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
    contourPointsInPixels.clear();
    for(int i=0 ; i<contourPoints.size() ; i++)
        contourPointsInPixels.push_back(Point<double>(contourPoints[i].x*parentCanvas->getWidth(),
                                                      contourPoints[i].y*parentCanvas->getHeight()));
    
    // Finally, we update sub triangles
    updateSubTriangles();
}



// ===== Updates =====
void InteractivePolygon::updateSubTriangles()
{
    // Reinitializes the whole list
    subTriangles.clear();
    // We begin by the annoying one
    subTriangles.push_back(SubTriangle(centerInPixels, contourPointsInPixels.back(), contourPointsInPixels.front()));
    // Then add the others
    for (int i = 0; i <contourPointsInPixels.size()-1; i++)
    {
        subTriangles.push_back(SubTriangle(centerInPixels, contourPointsInPixels[i], contourPointsInPixels[i+1]));
    }
}




// ===== INTERACTION COMPUTING =====

double InteractivePolygon::ComputeInteractionWeight(Point<double> T)
{
    double weight = 0.0;
    Point<double> GT = (T - centerInPixels);
    
    // if at center (at 0.5²pixel²) (to prevent 0/0 operations)
    if (GT.getDistanceSquaredFromOrigin() < 0.25)
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