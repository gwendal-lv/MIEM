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

#include "MiamMath.h"
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

EditablePolygon::EditablePolygon(int64_t _Id, bpt _center, bpolygon& _contourPoints, Colour _fillColour) :
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
        
        if (!enableTranslationOnly)
        {
            // Then, we draw the coutour draggable points
            for (size_t i=0 ; i<bcontourPointsInPixels.outer().size() ; i++)
            {
                g.fillEllipse((float)bcontourPointsInPixels.outer().at(i).get<0>()-contourPointsRadius,
							  (float)bcontourPointsInPixels.outer().at(i).get<1>() -contourPointsRadius,
                              contourPointsRadius*2.0f,
                              contourPointsRadius*2.0f);
            }
            
            // And finally, the manipulation (rotation & scale) main control
            Line<float> manipulationLine = Line<float>((float)bcenterInPixels.get<0>(),
                                                       (float)bcenterInPixels.get<1>(),
                                                       (float)bmanipulationPointInPixels.get<0>(),
                                                       (float)bmanipulationPointInPixels.get<1>());
            float dashedLineParameters[] = {4.0f, 4.0f};
            g.drawDashedLine(manipulationLine, dashedLineParameters, 2, centerContourWidth);
            g.fillEllipse((float)bmanipulationPointInPixels.get<0>() -manipulationPointRadius,
                          (float)bmanipulationPointInPixels.get<1>() -manipulationPointRadius,
                          (float)manipulationPointRadius*2.0f,
                          (float)manipulationPointRadius*2.0f);
        }
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
    if (bcenterInPixels.get<0>() + manipulationLineLengthRight + manipulationPointRadius
        <= parentCanvas->getWidth())
        bmanipulationPointInPixels = bpt(bcenterInPixels.get<0>()
                                                  + manipulationLineLengthRight,
                                                  bcenterInPixels.get<1>());
    else
        bmanipulationPointInPixels = bpt(bcenterInPixels.get<0>()
                                                  - manipulationLineLengthLeft,
                                                  bcenterInPixels.get<1>());
}







// ===== SETTERS AND GETTERS =====
void EditablePolygon::SetActive(bool activate)
{
    EditableArea::SetActive(activate);
    
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




AreaEventType EditablePolygon::TryBeginPointMove(const Point<double>& hitPoint)
{
	AreaEventType eventType = AreaEventType::NothingHappened;

	// ONE TOUCH POINT IS AUTHORIZED BY AREA, AT THE MOMENT
	// And this is decided... Here
	if (pointDraggedId != EditableAreaPointId::None)
	{
		eventType = AreaEventType::AnotherMonoTouchPointDragAlreadyBegun;
		return eventType;
	}

    // If any manipulation is authorized (not only the translation)
    if (!enableTranslationOnly)
    {
        // Are we grabbing the manipulation dot ?
        if (boost::geometry::distance(bmanipulationPointInPixels, bpt(hitPoint.x, hitPoint.y))
            < (centerCircleRadius+centerContourWidth)) // same radius than the center
        {
            pointDraggedId = EditableAreaPointId::ManipulationPoint;
			eventType = AreaEventType::PointDragBegins;
        }
        
        // Are we grabbing one of the contour points ?
        for (size_t i=0; (i < bcontourPointsInPixels.outer().size() && (eventType!=AreaEventType::PointDragBegins)) ; i++)
        {
            if (boost::geometry::distance(bcontourPointsInPixels.outer().at(i), bpt(hitPoint.x, hitPoint.y)) < pointDraggingRadius)
            {
                pointDraggedId = (int)i;
				eventType = AreaEventType::PointDragBegins;
            }
        }
        
        // Are we grabbing the center ?
        if (eventType != AreaEventType::PointDragBegins)
        {
			
            if (boost::geometry::distance(bcenterInPixels, bpt(hitPoint.x, hitPoint.y))
                < (centerCircleRadius+centerContourWidth))
            {
                pointDraggedId = EditableAreaPointId::Center;
				eventType = AreaEventType::PointDragBegins;
            }
        }
    }

    // Finally, was the point inside the polygon ? (which starts a translation)
    if (eventType != AreaEventType::PointDragBegins)
    {
        if (HitTest(hitPoint))
        {
            pointDraggedId = EditableAreaPointId::WholeArea;
            lastLocation = hitPoint;
			eventType = AreaEventType::PointDragBegins;
        }
    }
    
	return eventType;
}

AreaEventType EditablePolygon::TryMovePoint(const Point<double>& newLocation)
{
	bpt bnewLocation(newLocation.x, newLocation.y);
    AreaEventType areaEventType = AreaEventType::NothingHappened;
    
    // Simple contour point dragging
    if (pointDraggedId >= 0)
    {
        if ( parentCanvas->getLocalBounds().contains(newLocation.toInt())
            && isNewContourPointValid(newLocation))
        {
            //contourPointsInPixels[pointDraggedId] = newLocation;
            //contourPoints[pointDraggedId] = Point<double>(
            //                                newLocation.x / (double)parentCanvas->getWidth(),
            //                                newLocation.y / (double)parentCanvas->getHeight());
			bcontourPointsInPixels.outer().at(pointDraggedId) = bpt(newLocation.x, newLocation.y);
			bcontourPoints.outer().at(pointDraggedId) = bpt(newLocation.x / (double)parentCanvas->getWidth(),
															newLocation.y / (double)parentCanvas->getHeight());

            areaEventType = AreaEventType::ShapeChanged;
        }
    }
    
    else if (pointDraggedId == EditableAreaPointId::ManipulationPoint)
    {
        // Rotation will be applied anyway...
        // Security needed for point to stay within the canvas ?
        areaEventType = AreaEventType::RotScale;
        
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
		double r1 = boost::geometry::distance(bcenterInPixels, bmanipulationPointInPixels);//centerInPixels.getDistanceFrom(manipulationPointInPixels);
        double r2 = boost::geometry::distance(bcenterInPixels, bnewLocation);//centerInPixels.getDistanceFrom(newLocation);
        double x1 = bmanipulationPointInPixels.get<0>() - bcenterInPixels.get<0>();//manipulationPointInPixels.x - centerInPixels.x;
        double x2 = bnewLocation.get<0>() - bcenterInPixels.get<0>();//newLocation.x - centerInPixels.x;
        double y1 = bmanipulationPointInPixels.get<1>() - bcenterInPixels.get<1>();
        double y2 = bnewLocation.get<1>() - bcenterInPixels.get<1>();
		double cos_a = (x2*x1 + y2*y1)/(r1*r2);
        double sin_a = (y2*x1 - x2*y1)/(r1*r2);
        // ----- size -----
		double size = r2 / r1;
        
        // === Application of this transformation to the whole polygon ===
        // --- size if polygon is still big enough only ---
        double minDistanceFromCenter = 0.0;
        bool wasSizeApplied = false;
		bpolygon bnewContourPoints;//std::vector<Point<double>> newContourPoints;
        for (size_t i=0 ; i<bcontourPointsInPixels.outer().size() ;i++)
        {
			
			std::vector<bpt> result;
			DBG("bcontourPointsInPixels : (" + (String)bcontourPointsInPixels.outer().at(i).get<0>() +" , " + (String)bcontourPointsInPixels.outer().at(i).get<1>() +")");
			DBG("bcenterInPixels : (" + (String)bcenterInPixels .get<0>()+" , " + (String)bcenterInPixels.get<1>() +")");
			boost::geometry::difference(bcontourPointsInPixels.outer().at(i), bcenterInPixels, result);
			//result.push_back(bpt(bcontourPointsInPixels.outer().at(i).get<0>() - bcenterInPixels.get<0>(), bcontourPointsInPixels.outer().at(i).get<1>() - bcenterInPixels.get<1>()));
			//DBG("result : (" + (String)result.front().get<0>()+" , " + (String)result.front().get<1>() +")");
			//result.push_back(bpt(bcontourPointsInPixels.outer().at(i).get<0>() - bcenterInPixels.get<0>(), bcontourPointsInPixels.outer().at(i).get<1>() - bcenterInPixels.get<1>()));
            //bnewContourPoints.outer().push_back(result.front() );

			bnewContourPoints.outer().push_back(bpt(bcontourPointsInPixels.outer().at(i).get<0>() - bcenterInPixels.get<0>(),
				bcontourPointsInPixels.outer().at(i).get<1>() - bcenterInPixels.get<1>()));

            bnewContourPoints.outer().at(i) =bpt(size * bnewContourPoints.outer().at(i).get<0>(),
                                                size * bnewContourPoints.outer().at(i).get<1>());
            if (boost::geometry::distance(bnewContourPoints.outer().at(i), bpt(0,0)) > minDistanceFromCenter)
                minDistanceFromCenter = boost::geometry::distance(bnewContourPoints.outer().at(i), bpt(0, 0));

			//boost::geometry::strategy::transform::translate_transformer<double, 2, 2> tr(bcenterInPixels.get<0>(), bcenterInPixels.get<1>());
			//boost::geometry::transform(bnewContourPoints.outer().at(i), bnewContourPoints.outer().at(i), tr);
			bnewContourPoints.outer().at(i).set<0>(bnewContourPoints.outer().at(i).get<0>() + bcenterInPixels.get<0>());
			bnewContourPoints.outer().at(i).set<1>(bnewContourPoints.outer().at(i).get<1>() + bcenterInPixels.get<1>());
        }

		//boost::geometry::strategy::transform::translate_transformer<double, 2, 2> tr(bcenterInPixels.get<0>(), bcenterInPixels.get<1>());
		//boost::geometry::transform(bnewContourPoints, bnewContourPoints, tr);

        if (minDistanceFromCenter >=
            minimumSizePercentage*(parentCanvas->getWidth()+parentCanvas->getHeight())/2.0)
        {
            wasSizeApplied = true;
            bcontourPointsInPixels = bnewContourPoints;
            bmanipulationPointInPixels = bnewLocation;
        }
        // --- rotation is always applied ---
        for (size_t i=0 ; i<bcontourPointsInPixels.outer().size() ;i++)
        {
			bcontourPointsInPixels.outer().at(i).set<0>(bcontourPointsInPixels.outer().at(i).get<0>() - bcenterInPixels.get<0>());
			bcontourPointsInPixels.outer().at(i).set<1>(bcontourPointsInPixels.outer().at(i).get<1>() - bcenterInPixels.get<1>());
			bcontourPointsInPixels.outer().at(i) = bpt(cos_a*bcontourPointsInPixels.outer().at(i).get<0>()
                                                     -sin_a*bcontourPointsInPixels.outer().at(i).get<1>(),
                                                     sin_a*bcontourPointsInPixels.outer().at(i).get<0>()
                                                     +cos_a*bcontourPointsInPixels.outer().at(i).get<1>());
			//boost::geometry::strategy::transform::translate_transformer<double, 2, 2> tr(bcenterInPixels.get<0>(), bcenterInPixels.get<1>());
			//boost::geometry::transform(bcontourPoints.outer().at(i), bcontourPoints.outer().at(i), tr);
			bcontourPointsInPixels.outer().at(i).set<0>(bcontourPointsInPixels.outer().at(i).get<0>() + bcenterInPixels.get<0>());
			bcontourPointsInPixels.outer().at(i).set<1>(bcontourPointsInPixels.outer().at(i).get<1>() + bcenterInPixels.get<1>());
        }
        if (!wasSizeApplied)
        {
            // If size wasn't applied, we need to rotate the manipulation point
            manipulationPointInPixels -= centerInPixels;
			bmanipulationPointInPixels.set<0>(bmanipulationPointInPixels.get<0>() - bcenterInPixels.get<0>());
			bmanipulationPointInPixels.set<1>(bmanipulationPointInPixels.get<1>() - bcenterInPixels.get<1>());
            bmanipulationPointInPixels = bpt(cos_a*bmanipulationPointInPixels.get<0>()
                                                     -sin_a*bmanipulationPointInPixels.get<1>(),
                                                     sin_a*bmanipulationPointInPixels.get<0>()
                                                      +cos_a*bmanipulationPointInPixels.get<1>());
			bmanipulationPointInPixels.set<0>(bmanipulationPointInPixels.get<0>() + bcenterInPixels.get<0>());
			bmanipulationPointInPixels.set<1>(bmanipulationPointInPixels.get<1>() + bcenterInPixels.get<1>());
        }
        
        // After manipulation computation : normalized coordinates update
        for (size_t i=0; i < bcontourPointsInPixels.outer().size() ; i++)
        {
            //contourPoints[i] = Point<double>(
            //                    contourPointsInPixels[i].x / ((double)parentCanvas->getWidth()) ,
            //                    contourPointsInPixels[i].y / ((double)parentCanvas->getHeight()));
			bcontourPoints.outer().at(i) = bpt(bcontourPointsInPixels.outer().at(i).get<0>() / ((double)parentCanvas->getWidth()),
											   bcontourPointsInPixels.outer().at(i).get<1>() / ((double)parentCanvas->getHeight()));
        }
    }
    
    else if (pointDraggedId == EditableAreaPointId::Center)
    {
        if (isNewCenterValid(newLocation))
        {
            centerInPixels = newLocation;
			bcenterInPixels = bnewLocation;
            center = Point<double>(newLocation.x / ((double)parentCanvas->getWidth()),
                                   newLocation.y / ((double)parentCanvas->getHeight()) );

			bcenter = bpt(bnewLocation.get<0>() / ((double)parentCanvas->getWidth()),
						  bnewLocation.get<1>() / ((double)parentCanvas->getHeight()));
            
            computeManipulationPoint();
            areaEventType = AreaEventType::ShapeChanged;
        }
    }
    
    // Déplacement itératif
    else if (pointDraggedId == EditableAreaPointId::WholeArea)
    {
        // If translation leads to an out-of-canvas polygon, we just cancel it... No numeric issue
        // with double-precision floating-point numbers...
        Rectangle<double> boundingBoxContour = contour.getBounds().toDouble();
        Point<double> translation = newLocation - lastLocation;
        boundingBoxContour.translate(translation.getX(), translation.getY());
         // does new contour would be inside the canvas ?
        if (parentCanvas->getLocalBounds().contains(boundingBoxContour.toNearestInt()))
        {
            Translate(translation);
            areaEventType = AreaEventType::Translation;
            // Actualisation en prévision du prochain petit déplacement
            lastLocation = newLocation;
        }
    }
    
    // Graphic updates to all base attributes inherited
    if (areaEventType != AreaEventType::NothingHappened)
        InteractivePolygon::CanvasResized(this->parentCanvas);
    return areaEventType;
}


AreaEventType EditablePolygon::EndPointMove()
{
	// Always initialized, if some more complex code is written below, later
	AreaEventType eventType = AreaEventType::NothingHappened;

	// The point drag is always stopped without any check, for now
    computeManipulationPoint();
    pointDraggedId = EditableAreaPointId::None;
	eventType = AreaEventType::PointDragStops;

	return eventType;
}


void EditablePolygon::Translate(const Point<double>& translation) // remplacer par la translation de boost
{
	bcenterInPixels.set<0>(bcenterInPixels.get<0>() + translation.x);
	bcenterInPixels.set<1>(bcenterInPixels.get<1>() + translation.y);
    bcenter = bpt(bcenterInPixels.get<0>() / ((double)parentCanvas->getWidth()),
                           bcenterInPixels.get<1>() / ((double)parentCanvas->getHeight()) );
    for (size_t i=0; i < bcontourPointsInPixels.outer().size() ; i++)
    {
        //contourPointsInPixels[i] += translation;
		bcontourPointsInPixels.outer().at(i).set<0>(bcontourPointsInPixels.outer().at(i).get<0>() + translation.x);
		bcontourPointsInPixels.outer().at(i).set<1>(bcontourPointsInPixels.outer().at(i).get<1>() + translation.y);
        bcontourPoints.outer().at(i) = bpt(
											bcontourPointsInPixels.outer().at(i).get<0>() / ((double)parentCanvas->getWidth()) ,
											bcontourPointsInPixels.outer().at(i).get<1>() / ((double)parentCanvas->getHeight()));
    }
    
    // Manipulation point (+ line...)
    computeManipulationPoint();
}


bool EditablePolygon::TryCreatePoint(const Point<double>& hitPoint)
{
    int closestPointIndex = -1;
    double closestDistance = parentCanvas->getWidth()+parentCanvas->getHeight();
    // At first, we look for the closest point
    for (size_t i=0 ; i<bcontourPointsInPixels.outer().size(); i++)
    {
		double currentDistance = boost::geometry::distance(bcontourPointsInPixels.outer().at(i),bpt(hitPoint.x,hitPoint.y));//hitPoint.getDistanceFrom(contourPointsInPixels[i]);
        if (currentDistance < closestDistance)
        {
            closestPointIndex = (int)i;
            closestDistance = currentDistance;
        }
    }
    // Then, the other point is always the next
    int otherPointIndex = Math::Modulo(closestPointIndex+1, (int)bcontourPointsInPixels.outer().size());
    
    // Is the hitPoint to far from the two current points ?
    if ( closestDistance <= 10 * pointDraggingRadius )
    {
        bpt newPoint = bpt((bcontourPointsInPixels.outer().at(closestPointIndex).get<0>()+bcontourPointsInPixels.outer().at(otherPointIndex).get<0>())/2.0,(bcontourPointsInPixels.outer().at(closestPointIndex).get<1>()+bcontourPointsInPixels.outer().at(otherPointIndex).get<1>())/2.0);
        
        insertPointInPixels(newPoint, otherPointIndex);
        return true;
    }
    else
        return false;
}
String EditablePolygon::TryDeletePoint(const Point<double>& hitPoint)
{
    if (bcontourPointsInPixels.outer().size() > 3)
    {
        // - same code as in "TryCreatePoint" -
        int closestPointIndex = -1;
        double closestDistance = parentCanvas->getWidth()+parentCanvas->getHeight();
        // At first, we look for the closest point
        for (size_t i=0 ; i<bcontourPointsInPixels.outer().size(); i++)
        {
            double currentDistance = boost::geometry::distance(bcontourPointsInPixels.outer().at(i),bpt(hitPoint.x,hitPoint.y));
            if (currentDistance < closestDistance)
            {
                closestPointIndex = (int)i;
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
bool EditablePolygon::isCenterValidWithoutContourPoint(size_t contourPointId)
{
    // We add all point but the one to delete
    Path testContour;
	if (contourPointId > 0)
		testContour.startNewSubPath(juce::Point<float>((float)bcontourPointsInPixels.outer().at(0).get<0>(), (float)bcontourPointsInPixels.outer().at(0).get<1>()));
    else
        testContour.startNewSubPath((float)bcontourPointsInPixels.outer().at(1).get<0>(), (float)bcontourPointsInPixels.outer().at(1).get<1>());
    for (size_t i=1; i<bcontourPointsInPixels.outer().size() ; i++)
    {
        if (i != contourPointId)
            testContour.lineTo((float)bcontourPointsInPixels.outer().at(i).get<0>(),(float) bcontourPointsInPixels.outer().at(i).get<1>());
    }
    testContour.closeSubPath();
    // Then we check wether the center is still inside or not
    return testContour.contains((float)bcenterInPixels.get<0>(), (float)bcenterInPixels.get<1>());
}






// ===== EDITING HELPERS =====

bool EditablePolygon::isNewContourPointValid(const Point<double>& newLocation)
{
    // Init : we save indexes of adjacent points (that will help build the borders)
    int pointBefore = Math::Modulo(pointDraggedId-1, (int)bcontourPointsInPixels.outer().size());
    int pointAfter = Math::Modulo(pointDraggedId+1, (int)bcontourPointsInPixels.outer().size());
    /* Étape 1, on construit les équations des droites suivantes :
     * - droite 1 entre le centre et le point d'avant
     * - droite 2 entre le centre et le point d'après
     */
    CartesianLine droite1 = CartesianLine(bcenterInPixels,
                                          bcontourPointsInPixels.outer().at(pointBefore));
    CartesianLine droite2 = CartesianLine(bcenterInPixels,
                                          bcontourPointsInPixels.outer().at(pointAfter));
    /* Étape 2
     * on vérifie ne pas avoir changé de côté par rapport aux 2 lignes considérées
     */
    if ( droite1.PointWentThrough(bcontourPointsInPixels.outer().at(pointDraggedId), bpt(newLocation.x, newLocation.y))
        || droite2.PointWentThrough(bcontourPointsInPixels.outer().at(pointDraggedId), bpt(newLocation.x, newLocation.y)))
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
    for (size_t i=0 ; (i<bcontourPointsInPixels.outer().size() && !hasCrossed) ; i++)
    {
        CartesianLine cartesianLine = CartesianLine(bcontourPointsInPixels.outer().at(i),
                bcontourPointsInPixels.outer().at(Math::Modulo((int)i+1, (int)bcontourPointsInPixels.outer().size())));
        hasCrossed = cartesianLine.PointWentThrough(bcenterInPixels, bpt(newLocation.x, newLocation.y));
    }
    return !hasCrossed;
}


void EditablePolygon::insertPointInPixels(const bpt& newContourPoint, int position)
{
    bcontourPointsInPixels.outer().insert(bcontourPointsInPixels.outer().begin()+position,
                                 newContourPoint);
    
    recreateNormalizedPoints();
}
void EditablePolygon::deletePoint(int position)
{
	bcontourPointsInPixels.outer().erase(bcontourPointsInPixels.outer().begin() + position);
    
    recreateNormalizedPoints();
	this->CanvasResized(parentCanvas);
}

void EditablePolygon::recreateNormalizedPoints()
{
	bcontourPoints.clear();
    for (size_t i=0 ; i<bcontourPointsInPixels.outer().size() ; i++)
    {
        bcontourPoints.outer().push_back(bpt(bcontourPointsInPixels.outer().at(i).get<0>()/parentCanvas->getWidth(),
                                              bcontourPointsInPixels.outer().at(i).get<1>() /parentCanvas->getHeight()));
    }
	//bcontourPoints.outer().push_back(bpt(bcontourPointsInPixels.outer().at(0).get<0>() / parentCanvas->getWidth(),
	//	bcontourPointsInPixels.outer().at(0).get<1>() / parentCanvas->getHeight()));
    bcenter = bpt(bcenterInPixels.get<0>()/parentCanvas->getWidth(),
                           bcenterInPixels.get<1>()/parentCanvas->getHeight());
}


