/*
  ==============================================================================

    EditablePolygon.cpp
    Created: 26 Mar 2016 11:08:16am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "EditablePolygon.h"

#include "SceneCanvasComponent.h"

#include <cmath>

#include "Math.h"
#include "CartesianLine.h"

using namespace Miam;


// ========== CONSTRUCTION ==========

EditablePolygon::EditablePolygon(int64_t _Id) :
    InteractivePolygon(_Id)
{
    init();
}

EditablePolygon::EditablePolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
    InteractivePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
    init();
}

EditablePolygon::EditablePolygon(int64_t _Id, Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour) :
    InteractivePolygon(_Id, _center, _contourPoints, _fillColour)
{
    init();
}


// ===== Construction helpers =====
void EditablePolygon::init()
{
    graphicalInit();
    behaviorInit();
}
void EditablePolygon::graphicalInit()
{
    contourColour = Colour(0xAAFFFFFF); // white, but not totally opaque
    
    editingElementsColour = Colours::white; // pure white (more visible)
    contourPointsRadius = 1.4f*contourWidth;
    manipulationPointRadius = centerContourWidth+centerCircleRadius;
}
void EditablePolygon::behaviorInit()
{
    SetActive(false);
    pointDraggedId = EditableAreaPointId::None;
}




// ========== DRAWING ==========

void EditablePolygon::Paint(Graphics& g)
{
    // At first, we draw the normal surface
    DrawablePolygon::Paint(g);
    
    if (isActive)
    {
        g.setColour(editingElementsColour);
        
        // Then, we draw the manipulation points
        for (int i=0 ; i<contourPointsInPixels.size() ; i++)
        {
            g.fillEllipse((float)contourPointsInPixels[i].x-contourPointsRadius,
							(float)contourPointsInPixels[i].y-contourPointsRadius,
                          contourPointsRadius*2.0f,
                          contourPointsRadius*2.0f);
        }
        
        // And finally, the manipulation (rotation & scale) controls
        Line<float> manipulationLine = Line<float>((float)centerInPixels.x,
			(float)centerInPixels.y,
			(float)manipulationPointInPixels.x,
			(float)manipulationPointInPixels.y);
        float dashedLineParameters[] = {4.0f, 4.0f};
        g.drawDashedLine(manipulationLine, dashedLineParameters, 2, centerContourWidth);
        g.fillEllipse((float)manipulationPointInPixels.x-manipulationPointRadius,
			(float)manipulationPointInPixels.y-manipulationPointRadius,
			(float)manipulationPointRadius*2.0f,
			(float)manipulationPointRadius*2.0f);
    }
}






// =========== GRAPHICAL UPDATES ==========

// Whole actualization of pixel-coordinates graphical objects
void EditablePolygon::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    InteractivePolygon::CanvasResized(_parentCanvas);
    
    // Manipulation point (+ line...)
    computeManipulationPoint();
    
    pointDraggingRadius = 0.01f * (parentCanvas->getWidth()+parentCanvas->getHeight())/2.0f; // 1%
}

void EditablePolygon::computeManipulationPoint()
{
    float manipulationLineLengthLeft = 0.25f*(parentCanvas->getWidth()+parentCanvas->getHeight())/2.0f,
    manipulationLineLengthRight = manipulationLineLengthLeft; //px
    /*
    float currentDistance = 0.0f;
    for (int i=0;i<contourPointsInPixels.size();i++)
    {
        // The manipulation line will be longer than the max center-contour distance
        currentDistance = contourPointsInPixels[i].x - centerInPixels.x;
        if (currentDistance > manipulationLineLengthRight)
            manipulationLineLengthRight = currentDistance;
        else if (currentDistance < manipulationLineLengthLeft)
            manipulationLineLengthLeft = currentDistance;
    }
    manipulationLineLengthRight *= 1.3; // length coefficient
    manipulationLineLengthLeft *= -1.3; // negative length coefficient
     */
    // We always try to put ot on the right side first
    if (centerInPixels.x + manipulationLineLengthRight + manipulationPointRadius
        <= parentCanvas->getWidth())
        manipulationPointInPixels = Point<double>(centerInPixels.x
                                                  + manipulationLineLengthRight,
                                                  centerInPixels.y);
    else
        manipulationPointInPixels = Point<double>(centerInPixels.x
                                                  - manipulationLineLengthLeft,
                                                  centerInPixels.y);
}







// ===== SETTERS AND GETTERS =====
void EditablePolygon::SetActive(bool activate)
{
    isActive = activate;
    
    if (isActive)
    {
        fillOpacity = 0.8f;
    }
    else
    {
        fillOpacity = 0.5f;
    }
}





// ===== EDITION FUNCTIONS =====

bool EditablePolygon::HitTest(const Point<double>& hitPoint)
{
    return (contour.contains((float)hitPoint.x, (float)hitPoint.y));
}



bool EditablePolygon::TryBeginPointMove(const Point<double>& hitPoint)
{
    bool hitResult = false;
    
    // Are we grabbing the manipulation dot ?
    if (manipulationPointInPixels.getDistanceFrom(hitPoint)
        < (centerCircleRadius+centerContourWidth)) // same radius than the center
    {
        pointDraggedId = EditableAreaPointId::ManipulationPoint;
        hitResult = true;
    }
    
    // Are we grabbing one of the contour points ?
    for (int i=0; (i < contourPointsInPixels.size() && !hitResult) ; i++)
    {
        if (contourPointsInPixels[i].getDistanceFrom(hitPoint) < pointDraggingRadius)
        {
            pointDraggedId = i;
            hitResult = true;
        }
    }
    
    // Are we grabbing the center ?
    if (!hitResult)
    {
        if (centerInPixels.getDistanceFrom(hitPoint.toDouble())
            < (centerCircleRadius+centerContourWidth))
        {
            pointDraggedId = EditableAreaPointId::Center;
            hitResult = true;
        }
    }
    
    // Finally, was the point inside the polygon ? (which starts a translation)
    if (!hitResult)
    {
        if (HitTest(hitPoint))
        {
            pointDraggedId = EditableAreaPointId::WholeArea;
            lastLocation = hitPoint;
            hitResult = true;
        }
    }
    
    return hitResult;
}

void EditablePolygon::MovePoint(const Point<double>& newLocation)
{
    if (pointDraggedId >= 0)
    {
        if (isNewContourPointValid(newLocation))
        {
            contourPointsInPixels[pointDraggedId] = newLocation;
            contourPoints[pointDraggedId] = Point<double>(
                                            newLocation.x / ((float)parentCanvas->getWidth()) ,
                                            newLocation.y / ((float)parentCanvas->getHeight()) );
        }
    }
    
    else if (pointDraggedId == EditableAreaPointId::ManipulationPoint)
    {
        // Computation of the RotScale transformation needed to move the manipulation
        // point to this new location (RotScale relative to the center)
        // ----- rotation -----
        /* On rapporte le problème de calcul d'angle à zéro, avec le point 1 qui est le
         * manipulationPoint (angle a1), et le point 2 (angle a2) la newLocation.
         * Puis application des formules trigo de soustraction pour trouver cos(a) et 
         * sin(a), avec 'a' l'angle de la petite rotation qui vient d'avoir lieu
         * cos(a) = cos(a2 - a1) = cos(a2)cos(a1) + sin(a2)sin(a1)
         * sin(a) = sin(a2 - a1) = sin(a2)cos(a1) - cos(a2)sin(a1)
         */
        double r1 = centerInPixels.getDistanceFrom(manipulationPointInPixels);
        double r2 = centerInPixels.getDistanceFrom(newLocation);
        double x1 = manipulationPointInPixels.x - centerInPixels.x;
        double x2 = newLocation.x - centerInPixels.x;
        double y1 = manipulationPointInPixels.y - centerInPixels.y;
        double y2 = newLocation.y - centerInPixels.y;
        double cos_a = (x2*x1 + y2*y1)/(r1*r2);
        double sin_a = (y2*x1 - x2*y1)/(r1*r2);
        // ----- size -----
        double size = r2 / r1;
        
        // === Application of this transformation to the whole polygon ===
        // --- size if polygon is still big enough only ---
        double minDistanceFromCenter = 0.0;
        bool wasSizeApplied = false;
        std::vector<Point<double>> newContourPoints;
        for (int i=0 ; i<contourPointsInPixels.size() ;i++)
        {
            newContourPoints.push_back(contourPointsInPixels[i] - centerInPixels);
            newContourPoints[i] = Point<double>(size * newContourPoints[i].x,
                                                size * newContourPoints[i].y);
            if (newContourPoints[i].getDistanceFromOrigin() > minDistanceFromCenter)
                minDistanceFromCenter = newContourPoints[i].getDistanceFromOrigin();
            newContourPoints[i] += centerInPixels;
        }
        if (minDistanceFromCenter >=
            minimumSizePercentage*(parentCanvas->getWidth()+parentCanvas->getHeight())/2.0)
        {
            wasSizeApplied = true;
            contourPointsInPixels = newContourPoints;
            manipulationPointInPixels = newLocation;
        }
        // --- rotation is always applied ---
        for (int i=0 ; i<contourPointsInPixels.size() ;i++)
        {
            contourPointsInPixels[i] -= centerInPixels;
            contourPointsInPixels[i] = Point<double>(cos_a*contourPointsInPixels[i].x
                                                     -sin_a*contourPointsInPixels[i].y,
                                                     sin_a*contourPointsInPixels[i].x
                                                     +cos_a*contourPointsInPixels[i].y);
            contourPointsInPixels[i] += centerInPixels;
        }
        if (!wasSizeApplied)
        {
            // If size wasn't applied, we need to rotate the manipulation point
            manipulationPointInPixels -= centerInPixels;
            manipulationPointInPixels = Point<double>(cos_a*manipulationPointInPixels.x
                                                     -sin_a*manipulationPointInPixels.y,
                                                     sin_a*manipulationPointInPixels.x
                                                      +cos_a*manipulationPointInPixels.y);
            manipulationPointInPixels += centerInPixels;
        }
        
        // After manipulation computation : normalized coordinates update
        for (int i=0; i < contourPointsInPixels.size() ; i++)
        {
            contourPoints[i] = Point<double>(
                                contourPointsInPixels[i].x / ((double)parentCanvas->getWidth()) ,
                                contourPointsInPixels[i].y / ((double)parentCanvas->getHeight()));
        }
    }
    
    else if (pointDraggedId == EditableAreaPointId::Center)
    {
        if (isNewCenterValid(newLocation))
        {
            centerInPixels = newLocation;
            center = Point<double>(newLocation.x / ((double)parentCanvas->getWidth()),
                                   newLocation.y / ((double)parentCanvas->getHeight()) );
            computeManipulationPoint();
        }
    }
    
    else if (pointDraggedId == EditableAreaPointId::WholeArea)
    {
        // Déplacement itératif
        Point<double> translation = newLocation - lastLocation;
        Translate(translation);
        
        // Actualisation en prévision du prochain petit déplacement
        lastLocation = newLocation;
    }
    
    // Graphic updates to all base attributes inherited
    InteractivePolygon::CanvasResized(this->parentCanvas);
}


void EditablePolygon::EndPointMove()
{
    computeManipulationPoint();
    pointDraggedId = EditableAreaPointId::None;
}


void EditablePolygon::Translate(const Point<double>& translation)
{
    centerInPixels += translation;
    center = Point<double>(centerInPixels.x / ((double)parentCanvas->getWidth()),
                           centerInPixels.y / ((double)parentCanvas->getHeight()) );
    for (int i=0; i < contourPointsInPixels.size() ; i++)
    {
        contourPointsInPixels[i] += translation;
        contourPoints[i] = Point<double>(
                                         contourPointsInPixels[i].x / ((double)parentCanvas->getWidth()) ,
                                         contourPointsInPixels[i].y / ((double)parentCanvas->getHeight()));
    }
    
    // Manipulation point (+ line...)
    computeManipulationPoint();
}


bool EditablePolygon::TryCreatePoint(const Point<double>& hitPoint)
{
    int closestPointIndex = -1;
    double closestDistance = parentCanvas->getWidth()+parentCanvas->getHeight();
    // At first, we look for the closest point
    for (int i=0 ; i<contourPointsInPixels.size(); i++)
    {
        double currentDistance = hitPoint.getDistanceFrom(contourPointsInPixels[i]);
        if (currentDistance < closestDistance)
        {
            closestPointIndex = i;
            closestDistance = currentDistance;
        }
    }
    // Then, the other point is always the next
    int otherPointIndex = Math::Modulo(closestPointIndex+1, (int)contourPointsInPixels.size());
    
    // Is the hitPoint to far from the two current points ?
    if ( closestDistance <= 10 * pointDraggingRadius )
    {
        Point<double> newPoint = Point<double>((contourPointsInPixels[closestPointIndex].x+contourPointsInPixels[otherPointIndex].x)/2.0,(contourPointsInPixels[closestPointIndex].y+contourPointsInPixels[otherPointIndex].y)/2.0);
        
        insertPointInPixels(newPoint, otherPointIndex);
        return true;
    }
    else
        return false;
}
String EditablePolygon::TryDeletePoint(const Point<double>& hitPoint)
{
    if (contourPointsInPixels.size() > 3)
    {
        // - same code as in "TryCreatePoint" -
        int closestPointIndex = -1;
        double closestDistance = parentCanvas->getWidth()+parentCanvas->getHeight();
        // At first, we look for the closest point
        for (int i=0 ; i<contourPointsInPixels.size(); i++)
        {
            double currentDistance = hitPoint.getDistanceFrom(contourPointsInPixels[i]);
            if (currentDistance < closestDistance)
            {
                closestPointIndex = i;
                closestDistance = currentDistance;
            }
        }
        // - same code as in "TryCreatePoint" -
        // Is the hitPoint to far from the closest point ?
        if ( closestDistance <= pointDraggingRadius )
        {
            // If we reach this code, we MAY delete the point, if the polygon centre would remain within the new polygon
            if (isCenterValidWithoutContourPoint(closestPointIndex))
            {
                deletePoint(closestPointIndex);
                return String(""); // Ok signal !
            }
            else
                return String("Cannot delete point (move the center first)");
        }
        else
            return String("No point deleted (try clicking closer to an existing point)");
    }
    else
        return String("Cannot delete point (min. 3 points required)");
}
bool EditablePolygon::isCenterValidWithoutContourPoint(int contourPointId)
{
    // We add all point but the one to delete
    Path testContour;
    if (contourPointId>0)
        testContour.startNewSubPath(contourPointsInPixels[0].toFloat());
    else
        testContour.startNewSubPath(contourPointsInPixels[1].toFloat());
    for (int i=1; i<contourPointsInPixels.size() ; i++)
    {
        if (i != contourPointId)
            testContour.lineTo(contourPointsInPixels[i].toFloat());
    }
    testContour.closeSubPath();
    // Then we check wether the center is still inside or not
    return testContour.contains((float)centerInPixels.getX(), (float)centerInPixels.getY());
}






// ===== EDITING HELPERS =====

bool EditablePolygon::isNewContourPointValid(const Point<double>& newLocation)
{
    // Init : we save indexes of adjacent points (that will help build the borders)
    int pointBefore = Math::Modulo(pointDraggedId-1, (int)contourPointsInPixels.size());
    int pointAfter = Math::Modulo(pointDraggedId+1, (int)contourPointsInPixels.size());
    /* Étape 1, on construit les équations des droites suivantes :
     * - droite 1 entre le centre et le point d'avant
     * - droite 2 entre le centre et le point d'après
     */
    CartesianLine droite1 = CartesianLine(centerInPixels,
                                          contourPointsInPixels[pointBefore]);
    CartesianLine droite2 = CartesianLine(centerInPixels,
                                          contourPointsInPixels[pointAfter]);
    /* Étape 2
     * on vérifie ne pas avoir changé de côté par rapport aux 2 lignes considérées
     */
    if ( droite1.PointWentThrough(contourPointsInPixels[pointDraggedId], newLocation)
        || droite2.PointWentThrough(contourPointsInPixels[pointDraggedId], newLocation) )
    {
        return false;
    }
    else
        return true;
}


bool EditablePolygon::isNewCenterValid(const Point<double>& newLocation)
{
    // We check if the center did not cross all the lines, one-by-one
    bool hasCrossed = false;
    for (int i=0 ; (i<contourPointsInPixels.size() && !hasCrossed) ; i++)
    {
        CartesianLine cartesianLine = CartesianLine(contourPointsInPixels[i],
                contourPointsInPixels[Math::Modulo(i+1, (int)contourPointsInPixels.size())]);
        hasCrossed = cartesianLine.PointWentThrough(centerInPixels, newLocation);
    }
    return !hasCrossed;
}


void EditablePolygon::insertPointInPixels(const Point<double>& newContourPoint, int position)
{
    contourPointsInPixels.insert(contourPointsInPixels.begin()+position,
                                 newContourPoint);
    
    recreateNormalizedPoints();
}
void EditablePolygon::deletePoint(int position)
{
    contourPointsInPixels.erase(contourPointsInPixels.begin()+position);
    // barycentrum (G point) computation DELETED FOR NOW (centre doesn't move)
    /*double xG = 0.0, yG = 0.0; // pixel coordinates
    for (int i = 0 ; i<contourPointsInPixels.size() ; i++)
    {
        xG += contourPointsInPixels[i].x;
        yG += contourPointsInPixels[i].y;
    }
    xG /= ((double)contourPointsInPixels.size());
    yG /= ((double)contourPointsInPixels.size());
    centerInPixels = Point<double>(xG, yG);*/
    
    recreateNormalizedPoints();
}

void EditablePolygon::recreateNormalizedPoints()
{
    contourPoints.clear();
    for (int i=0 ; i<contourPointsInPixels.size() ; i++)
    {
        contourPoints.push_back(Point<double>(contourPointsInPixels[i].x/parentCanvas->getWidth(),
                                              contourPointsInPixels[i].y/parentCanvas->getHeight()));
    }
    center = Point<double>(centerInPixels.x/parentCanvas->getWidth(),
                           centerInPixels.y/parentCanvas->getHeight());
}


