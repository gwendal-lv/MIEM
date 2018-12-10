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

EditablePolygon::EditablePolygon(bptree::ptree& areaTree)
:
InteractivePolygon(areaTree)
{
    init();
}

EditablePolygon::EditablePolygon(int64_t _Id) :
    InteractivePolygon(_Id)
{
    init();
}

EditablePolygon::EditablePolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
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
	// rotationAngle = 0; // useless I think....
    contourColour = Colour(0xAAFFFFFF); // white, but not totally opaque
    
    editingElementsColour = Colours::white; // pure white (more visible)
    
    manipulationPointRadius = centerContourWidth+centerCircleRadius;

#if defined(__MIEM_VBO)
		// resize des buffers
    vertices_buffer.resize(GetVerticesBufferSize());
    indices_buffer.resize(GetIndicesBufferSize());
    coulours_buffer.resize(GetColoursBufferSize());
#endif
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
            for (size_t i=0 ; i<contourPointsInPixels.outer().size() ; i++)
            {
                g.fillEllipse((float)contourPointsInPixels.outer().at(i).get<0>()-contourPointsRadius,
							  (float)contourPointsInPixels.outer().at(i).get<1>() -contourPointsRadius,
                              contourPointsRadius*2.0f,
                              contourPointsRadius*2.0f);
            }
            
            // And finally, the manipulation (rotation & scale) main control
            Line<float> manipulationLine = Line<float>((float)centerInPixels.get<0>(),
                                                       (float)centerInPixels.get<1>(),
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

    // vérifier fonctionnement après fusion
	//pointDraggingRadius = 0.01f * (parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0f; // 1%
    
    // UN PETIT REFRESH GL LA NON ???
    // UN PETIT REFRESH GL LA NON ???
    // UN PETIT REFRESH GL LA NON ???
    // UN PETIT REFRESH GL LA NON ???
    // UN PETIT REFRESH GL LA NON ???
    // UN PETIT REFRESH GL LA NON ???
    // UN PETIT REFRESH GL LA NON ???

}

static bool __MIEM__editablePrintDone = false;
void EditablePolygon::RefreshOpenGLBuffers()
{
#ifdef __MIAM_DEBUG
    if (!__MIEM__editablePrintDone)
    {
        __MIEM__editablePrintDone = true;
        std::cout << "[Refresh GL buffers] DrawableArea : " << DrawableArea::GetVerticesBufferElementsCount() << " points. ";
        std::cout << "DrawablePolygon : " << DrawablePolygon::GetVerticesBufferElementsCount() << " points. ";
        std::cout << "EditablePolygon : " << EditablePolygon::GetVerticesBufferElementsCount() << " points." << std::endl;
        std::cout << "[Refresh GL buffers] DrawableArea : " << DrawableArea::GetIndicesBufferElementsCount() << " indices. ";
        std::cout << "DrawablePolygon : " << DrawablePolygon::GetIndicesBufferElementsCount() << " indices. ";
        std::cout << "EditablePolygon : " << EditablePolygon::GetIndicesBufferElementsCount() << " indices." << std::endl;
    }
#endif
    
#if defined(__MIEM_VBO)
	InteractivePolygon::RefreshOpenGLBuffers();
    EditableArea::refreshOpenGLSubBuffers(InteractivePolygon::GetVerticesBufferElementsCount(),
                                          InteractivePolygon::GetIndicesBufferElementsCount());
#endif
}




// ===== SETTERS AND GETTERS =====
void EditablePolygon::SetActive(bool activate)
{
    EditableArea::SetActive(activate);
}






AreaEventType EditablePolygon::TryBeginPointMove(const Point<double>& hitPoint)
{
	AreaEventType eventType = AreaEventType::NothingHappened;
    auto hitBpt = bpt(hitPoint.x, hitPoint.y);

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
        if (boost::geometry::distance(bmanipulationPointInPixels, hitBpt)
            < (centerCircleRadius+centerContourWidth)) // same radius than the center
        {
            pointDraggedId = EditableAreaPointId::ManipulationPoint;
			eventType = AreaEventType::PointDragBegins;
        }
        
        // Are we grabbing one of the contour points ?
        for (size_t i=0; (i < contourPointsInPixels.outer().size()-1 && (eventType!=AreaEventType::PointDragBegins)) ; i++)
        {
            if (boost::geometry::distance(contourPointsInPixels.outer().at(i), hitBpt) < elementInteractionRadius)
            {
                pointDraggedId = (int)i;
				eventType = AreaEventType::PointDragBegins;
            }
        }

        // Are we grabbing the center ?
        if (eventType != AreaEventType::PointDragBegins)
        {
            if (boost::geometry::distance(centerInPixels, hitBpt)
                < (centerCircleRadius+centerContourWidth))
            {
                pointDraggedId = EditableAreaPointId::Center;
				eventType = AreaEventType::PointDragBegins;
            }
        }
        
        // Are we grabbing an entire side ? If the used clicked close enough to a painted side line
        for (size_t i=0; i < edgesHitBoxes.size() && (eventType != AreaEventType::PointDragBegins) ; i++)
        {
            if (boost::geometry::within(hitBpt, edgesHitBoxes[i]))
            {
                std::cout << "collision avec arête #" << i << std::endl;
                eventType = AreaEventType::PointDragBegins;
                pointDraggedId = EditableAreaPointId::TwoNeighbourPoints;
                sideDraggedId = (int)i;
            }
        }
    }

    // Finally, was the point inside the polygon ? (which starts a translation)
    if (eventType != AreaEventType::PointDragBegins)
    {
        if (hitTest(hitPoint.x,hitPoint.y))
        {
            pointDraggedId = EditableAreaPointId::WholeArea;
			eventType = AreaEventType::PointDragBegins;
        }
    }
    
    // If something starts : data backups
    if (eventType == AreaEventType::PointDragBegins)
    {
        lastLocation = hitPoint;
    }

	return eventType;
}

AreaEventType EditablePolygon::TryMovePoint(const Point<double>& newLocation)
{
    AreaEventType areaEventType = AreaEventType::NothingHappened;
	bpt bnewLocation(newLocation.x, newLocation.y);
    // Simple contour point dragging
    if (pointDraggedId >= 0)
    {
        if ( isNewContourPointValid(pointDraggedId, newLocation) )
        {
            moveContourPoint(pointDraggedId, newLocation);
            areaEventType = AreaEventType::ShapeChanged;
        }
    }
    // arête du polygone : 2 points déplacés ensemble
    else if (pointDraggedId == EditableAreaPointId::TwoNeighbourPoints)
    {
        // On essaie de faire la translation un point après l'autre
        // si la seconde n'est pas valide, on devra annuler la première...
        Point<double> translation = newLocation - lastLocation;
        Point<double> newLocation1(contourPointsInPixels.outer()[sideDraggedId].get<0>(),
                                   contourPointsInPixels.outer()[sideDraggedId].get<1>());
        newLocation1 += translation;
        if (isNewContourPointValid(sideDraggedId, newLocation1))
        {
            moveContourPoint(sideDraggedId, newLocation1);
            Point<double> newLocation2(contourPointsInPixels.outer()[sideDraggedId+1].get<0>(),
                                       contourPointsInPixels.outer()[sideDraggedId+1].get<1>());
            newLocation2 += translation;
            // déplacement effectif de l'arête entière + évènement
            if (isNewContourPointValid(sideDraggedId+1, newLocation2))
            {
                moveContourPoint(sideDraggedId+1, newLocation2);
                areaEventType = AreaEventType::ShapeChanged;
            }
            // sinon, annulation du premier déplacement
            else
            {
                newLocation1 -= translation;
                moveContourPoint(sideDraggedId, newLocation1);
            }
        }
    }
    // manipulation point
    else if (pointDraggedId == EditableAreaPointId::ManipulationPoint)
    {
        // Rotation will be applied anyway...
        // Security needed for point to stay within the canvas ?
		
        areaEventType = AreaEventType::RotScale;
        
        // Computation of the RotScale transformation needed to move the manipulation
        // point to this new location (RotScale relative to the center)
        // ----- rotation -----
		double r1 = boost::geometry::distance(centerInPixels, bmanipulationPointInPixels);
        double r2 = boost::geometry::distance(centerInPixels, bnewLocation);
        
		bpt testPt(bnewLocation);
		boost::geometry::subtract_point(testPt, centerInPixels);
		double radAngle = Math::ComputePositiveAngle(testPt);

        // ----- size -----
		double size = r2 / r1;
        
        // === Application of this transformation to the whole polygon ===
        // --- size if polygon is still big enough only ---
       // double minDistanceFromCenter = 0.0;
        bool wasSizeApplied = false;
		if (SizeChanged(size, true))
		{
			bmanipulationPointInPixels.set<0>(bnewLocation.get<0>());
			bmanipulationPointInPixels.set<1>(bnewLocation.get<1>());
			wasSizeApplied = true;
		}

		Rotate(-radAngle + lastManipRotAngleRad);
		lastManipRotAngleRad = radAngle;

		
        if (!wasSizeApplied)
        {
            // If size wasn't applied, we need to rotate the manipulation point
			bpt newManipulationPoint;

			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> trans(-centerInPixels.get<0>(), -centerInPixels.get<1>());
			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invtrans(centerInPixels.get<0>(), centerInPixels.get<1>());
			boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> rot(radAngle - lastManipRotAngleRad);


			boost::geometry::transform(bmanipulationPointInPixels, newManipulationPoint, trans);
			boost::geometry::transform(newManipulationPoint, bmanipulationPointInPixels, rot);
			boost::geometry::transform(bmanipulationPointInPixels, newManipulationPoint, invtrans);

			bmanipulationPointInPixels = newManipulationPoint;
        }
        
        // After manipulation computation : normalized coordinates update
		updateContourPoints();
    }
    
    else if (pointDraggedId == EditableAreaPointId::Center)
    {
        if (isNewCenterValid(newLocation))
        {
			centerInPixels = bnewLocation;
			center = bpt(bnewLocation.get<0>() / ((double)parentCanvas->getWidth()),
						  bnewLocation.get<1>() / ((double)parentCanvas->getHeight()));
            
            computeManipulationPoint();
            areaEventType = AreaEventType::ShapeChanged;
        }
    }
    
    // aire entière
    else if (pointDraggedId == EditableAreaPointId::WholeArea)
    {
        // test de validité de la translation en passant par la bouding box
        Rectangle<double> boundingBoxContour = contour.getBounds().toDouble();
        Point<double> translation = newLocation - lastLocation;
        boundingBoxContour.translate(translation.getX(), translation.getY());
         // does new contour would be inside the canvas ?
        if (parentCanvas->getLocalBounds().contains(boundingBoxContour.toNearestInt()))
        {
            Translate(translation);
            areaEventType = AreaEventType::Translation;
        }
    }
    
    
    // Actualisation en prévision du prochain petit déplacement
    lastLocation = newLocation;
    
    // Graphic updates to all base attributes inherited
	if (areaEventType != AreaEventType::NothingHappened)
	{
		InteractivePolygon::CanvasResized(this->parentCanvas);
#if defined(__MIEM_VBO)
        RefreshOpenGLBuffers();
#endif
	}
    return areaEventType;
}


AreaEventType EditablePolygon::EndPointMove()
{
	// Always initialized, if some more complex code is written below, later
	AreaEventType eventType = AreaEventType::NothingHappened;

	// The point drag is always stopped without any check, for now
    pointDraggedId = EditableAreaPointId::None;
    sideDraggedId = -1;
	eventType = AreaEventType::PointDragStops;
    
    // Updates the manip line, back to origin position ?
    lastManipRotAngleRad = 0.0;
    computeManipulationPoint();
    
#if defined(__MIEM_VBO)
    RefreshOpenGLBuffers();
#endif

	return eventType;
}

bool EditablePolygon::SizeChanged(double size, bool minSize)
{

	bool returnValue = false;
	//// --- size if polygon is still big enough only ---
	double minDistanceFromCenter = 0.0;
	bool wasSizeApplied = false;
	bpolygon testBoost2, testBoost;
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> trans(-centerInPixels.get<0>(), -centerInPixels.get<1>());
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invtrans(centerInPixels.get<0>(), centerInPixels.get<1>());
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> resizer(size, size);

	boost::geometry::transform(contourPointsInPixels, testBoost, trans);
	boost::geometry::transform(testBoost, testBoost2, resizer);
	boost::geometry::transform(testBoost2, testBoost, invtrans);


	for (size_t i = 0; i < testBoost.outer().size(); i++)
	{
		//if (testBoost.outer().at(i).get<0>() < 0 || testBoost.outer().at(i).get<1>() < 0)
		//	DBG("probleme");
		if (boost::geometry::distance(testBoost.outer().at(i), centerInPixels) > minDistanceFromCenter)
			minDistanceFromCenter = boost::geometry::distance(testBoost.outer().at(i), centerInPixels);
	}
	//std::vector<bpolygon> comparaison;
	//boost::geometry::difference(bnewContourPoints, testBoost, comparaison);

	if ((!minSize) || (minDistanceFromCenter >=
		minimumSizePercentage*(parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0))
	{
		wasSizeApplied = true;
		contourPointsInPixels.clear(); // test;
		contourPointsInPixels = testBoost;//bnewContourPoints;
										  //bmanipulationPointInPixels.set<0>(centerInPixels.get<0>() + manipulationPointRadius*size); //= bnewLocation;
										  //bmanipulationPointInPixels.set<1>(centerInPixels.get<1>() + manipulationPointRadius*size);
										  //bmanipulationPointInPixels = newManipulationPt;
		returnValue = true;
		//a *= size; // faire diviser pas Heght et width?
		//b *= size;
	}


	return returnValue;
}

void EditablePolygon::Rotate(double Radian)
{
	bpolygon newPolygon;

	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> trans(-centerInPixels.get<0>(), -centerInPixels.get<1>());
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invtrans(centerInPixels.get<0>(), centerInPixels.get<1>());
	boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> rot(Radian);


	boost::geometry::transform(contourPointsInPixels, newPolygon, trans);
	boost::geometry::transform(newPolygon, contourPointsInPixels, rot);
	boost::geometry::transform(contourPointsInPixels, newPolygon, invtrans);

	contourPointsInPixels.clear();
	contourPointsInPixels = newPolygon;

}

void EditablePolygon::updateContourPoints()
{
	contourPoints.clear();
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(1 / ((double)parentCanvas->getWidth()), 1 / ((double)parentCanvas->getHeight()));
	boost::geometry::transform(contourPointsInPixels, contourPoints, scaler);
}


void EditablePolygon::Translate(const Point<double>& translation) // remplacer par la translation de boost
{
	centerInPixels.set<0>(centerInPixels.get<0>() + translation.x);
	centerInPixels.set<1>(centerInPixels.get<1>() + translation.y);
    center = bpt(centerInPixels.get<0>() / ((double)parentCanvas->getWidth()),
                           centerInPixels.get<1>() / ((double)parentCanvas->getHeight()) );
    for (size_t i=0; i < contourPointsInPixels.outer().size() ; i++)
    {
		contourPointsInPixels.outer().at(i).set<0>(contourPointsInPixels.outer().at(i).get<0>() + translation.x);
		contourPointsInPixels.outer().at(i).set<1>(contourPointsInPixels.outer().at(i).get<1>() + translation.y);
        contourPoints.outer().at(i) = bpt(
											contourPointsInPixels.outer().at(i).get<0>() / ((double)parentCanvas->getWidth()) ,
											contourPointsInPixels.outer().at(i).get<1>() / ((double)parentCanvas->getHeight()));
    }
    
    // Manipulation point (+ line...)
    computeManipulationPoint();
}
void EditablePolygon::moveContourPoint(size_t pointIndex, const Point<double>& newLocation)
{
    // Translation du point
    contourPointsInPixels.outer()[pointIndex] = bpt(newLocation.x, newLocation.y);
    contourPoints.outer()[pointIndex] = bpt(newLocation.x / ((double)parentCanvas->getWidth()),
                                            newLocation.y / ((double)parentCanvas->getHeight()));
    // Translation éventuelle du duplicat (si point de fermeture du polygone)
    const size_t closingPointIndex = contourPointsInPixels.outer().size() - 1;
    if (pointIndex == 0)
    {
        contourPointsInPixels.outer()[closingPointIndex] = contourPointsInPixels.outer()[0];
        contourPoints.outer()[closingPointIndex] = contourPoints.outer()[0];
    }
    else if (pointIndex == closingPointIndex)
    {
        contourPointsInPixels.outer()[0] = contourPointsInPixels.outer()[closingPointIndex];
        contourPoints.outer()[0] = contourPoints.outer()[closingPointIndex];
    }
}


bool EditablePolygon::TryCreatePoint(const Point<double>& hitPoint)
{
    int closestPointIndex = -1;
    double closestDistance = parentCanvas->getWidth()+parentCanvas->getHeight();
    // At first, we look for the closest point
    for (size_t i=0 ; i<contourPointsInPixels.outer().size(); i++)
    {
		double currentDistance = boost::geometry::distance(contourPointsInPixels.outer().at(i),bpt(hitPoint.x,hitPoint.y));//hitPoint.getDistanceFrom(contourPointsInPixels[i]);
        if (currentDistance < closestDistance)
        {
            closestPointIndex = (int)i;
            closestDistance = currentDistance;
        }
    }
    // Then, the other point is always the next
    int otherPointIndex = Math::Modulo(closestPointIndex+1, (int)contourPointsInPixels.outer().size());
    
    // Is the hitPoint to far from the two current points ?
    if ( closestDistance <= 10 * elementInteractionRadius )
    {
        bpt newPoint = bpt((contourPointsInPixels.outer().at(closestPointIndex).get<0>()+contourPointsInPixels.outer().at(otherPointIndex).get<0>())/2.0,(contourPointsInPixels.outer().at(closestPointIndex).get<1>()+contourPointsInPixels.outer().at(otherPointIndex).get<1>())/2.0);
        
        insertPointInPixels(newPoint, otherPointIndex);
        return true;
    }
    else
        return false;
}
String EditablePolygon::TryDeletePoint(const Point<double>& hitPoint)
{
    if (contourPointsInPixels.outer().size() > 3)
    {
        // - same code as in "TryCreatePoint" -
        int closestPointIndex = -1;
        double closestDistance = parentCanvas->getWidth()+parentCanvas->getHeight();
        // At first, we look for the closest point
        for (size_t i=0 ; i<contourPointsInPixels.outer().size(); i++)
        {
            double currentDistance = boost::geometry::distance(contourPointsInPixels.outer().at(i),bpt(hitPoint.x,hitPoint.y));
            if (currentDistance < closestDistance)
            {
                closestPointIndex = (int)i;
                closestDistance = currentDistance;
            }
        }
        // - same code as in "TryCreatePoint" -
        // Is the hitPoint to far from the closest point ?
        if ( closestDistance <= elementInteractionRadius )
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
		testContour.startNewSubPath(juce::Point<float>((float)contourPointsInPixels.outer().at(0).get<0>(), (float)contourPointsInPixels.outer().at(0).get<1>()));
    else
        testContour.startNewSubPath((float)contourPointsInPixels.outer().at(1).get<0>(), (float)contourPointsInPixels.outer().at(1).get<1>());
    for (size_t i=1; i<contourPointsInPixels.outer().size() ; i++)
    {
        if (i != contourPointId)
            testContour.lineTo((float)contourPointsInPixels.outer().at(i).get<0>(),(float) contourPointsInPixels.outer().at(i).get<1>());
    }
    testContour.closeSubPath();
    // Then we check wether the center is still inside or not
    return testContour.contains((float)centerInPixels.get<0>(), (float)centerInPixels.get<1>());
}






// ===== EDITING HELPERS =====

bool EditablePolygon::isNewContourPointValid(size_t pointIndex, const Point<double>& newLocation)
{
    // Si le point sort du canevas -> non-valide
    if (!(parentCanvas->getLocalBounds().contains(newLocation.toInt())))
        return false;
    
    // Dans l'algorithme qui suit : on fait tout par rapport au point original [0]
    // et pas son duplicat le [.size()-1]
    int uniquePointIndex = Math::Modulo((int)pointIndex, (int)contourPointsInPixels.outer().size()-1);
    
    // Init : we save indexes of adjacent points (that will help build the borders)
	int previousIndex = Math::Modulo((int)uniquePointIndex - 1, (int)contourPointsInPixels.outer().size() - 1);
    int nextIndex = Math::Modulo((int)uniquePointIndex + 1, (int)contourPointsInPixels.outer().size() - 1);
    /* Étape 1, on construit les équations des droites suivantes :
     * - droite 1 entre le centre et le point d'avant
     * - droite 2 entre le centre et le point d'après
     */
    CartesianLine droite1 = CartesianLine(centerInPixels,
                                          contourPointsInPixels.outer().at(previousIndex));
    CartesianLine droite2 = CartesianLine(centerInPixels,
                                          contourPointsInPixels.outer().at(nextIndex));
    /* Étape 2
     * on vérifie ne pas avoir changé de côté par rapport aux 2 lignes considérées
     */
    if ( droite1.PointWentThrough(contourPointsInPixels.outer()[uniquePointIndex], bpt(newLocation.x, newLocation.y))
        || droite2.PointWentThrough(contourPointsInPixels.outer()[uniquePointIndex], bpt(newLocation.x, newLocation.y)))
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
    for (size_t i=0 ; (i<contourPointsInPixels.outer().size() && !hasCrossed) ; i++)
    {
        CartesianLine cartesianLine =
            CartesianLine(contourPointsInPixels.outer().at(i),
            contourPointsInPixels.outer().at(Math::Modulo((int)i+1,                                                             (int)contourPointsInPixels.outer().size())));
        
        hasCrossed = cartesianLine.PointWentThrough(centerInPixels,
                                                    bpt(newLocation.x, newLocation.y));
    }
    return !hasCrossed;
}


void EditablePolygon::insertPointInPixels(const bpt& newContourPoint, int position)
{
    // Attention : comportement spécial à l'insertion du point en 0 (fermeture poly)
    if (position > 0)
        contourPointsInPixels.outer().insert(contourPointsInPixels.outer().begin()
                                             +position,
                                             newContourPoint);
    else if (position == 0)
    {
        contourPointsInPixels.outer().insert(contourPointsInPixels.outer().begin(),
                                             newContourPoint);
        contourPointsInPixels.outer().back() = contourPointsInPixels.outer().front();
    }
    
    recreateNormalizedPoints();
}
void EditablePolygon::deletePoint(int position)
{
    if (position > 0)
        contourPointsInPixels.outer().erase(contourPointsInPixels.outer().begin()
                                            + position);
    // Attention : si on supprime le point 0, il pas mal changer le
    // polygone boost, le rendre fermé, etc.
    // On considère que c'est le point d'après qui devient le point de fermeture
    else if (position == 0)
    {
        contourPointsInPixels.outer().erase(contourPointsInPixels.outer().begin());
        auto& closingPoint = contourPointsInPixels.outer().back();
        auto& firstPoint = contourPointsInPixels.outer().at(1);
        closingPoint = firstPoint;
    }
        
    
    recreateNormalizedPoints();
	this->CanvasResized(parentCanvas);
}


void EditablePolygon::recreateNormalizedPoints()
{
	contourPoints.clear();
    for (size_t i=0 ; i<contourPointsInPixels.outer().size() ; i++)
    {
        contourPoints.outer().push_back(bpt(contourPointsInPixels.outer().at(i).get<0>()/parentCanvas->getWidth(),
                                              contourPointsInPixels.outer().at(i).get<1>() /parentCanvas->getHeight()));
    }
    center = bpt(centerInPixels.get<0>()/parentCanvas->getWidth(),
                           centerInPixels.get<1>()/parentCanvas->getHeight());
}
