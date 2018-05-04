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
	rotationAngle = 0;
    contourColour = Colour(0xAAFFFFFF); // white, but not totally opaque
    
    editingElementsColour = Colours::white; // pure white (more visible)
    contourPointsRadius = 1.4f*contourWidth;
    manipulationPointRadius = centerContourWidth+centerCircleRadius;

	// ajout de la forme et du contour !
	opaque_vertex_buffer_size += (3 * (numPointsPolygon * numVerticesCircle) + 3 * dottedLineVertexes + 3 * numVerticesRing);
	opaque_index_buffer_size += (3 * (numPointsPolygon * numPointCircle) + dottedLineIndices + 3 * numVerticesRing);
	opaque_color_buffer_size += (4 * (numPointsPolygon * numVerticesCircle) + 4 * dottedLineVertexes + 4 * numVerticesRing);

	// resize des buffers
	opaque_vertex_buffer.resize(opaque_vertex_buffer_size);
	opaque_index_buffer.resize(opaque_index_buffer_size);
	opaque_color_buffer.resize(opaque_color_buffer_size);

	// calcul d'un disque de centre 0 et de rayon 5 pixels
	float radius = 5.0f;
	float width = 3.0f;
	int numPoints = numPointsRing;
	radius = 5.0f;
	double currentAngle = 0.0;
	double incAngle = 2 * M_PI / (double)numPoints;
	g_vertex_circle[0] = 0.0f;
	g_vertex_circle[1] = 0.0f;
	g_vertex_circle[2] = 0.0f;
	for (int i = 0; i < numPointCircle; ++i)
	{
		g_vertex_circle[(i + 1) * 3] = radius * (float)cos(currentAngle);
		g_vertex_circle[(i + 1) * 3 + 1] = radius * (float)sin(currentAngle);
		g_vertex_circle[(i + 1) * 3 + 2] = 0.0f;
		currentAngle += incAngle;
	}
	for (int i = 0; i < numPointCircle; ++i)
	{
		circleIndices[i * 3] = i + 1;
		circleIndices[i * 3 + 1] = 0;
		circleIndices[i * 3 + 2] = i + 2 > numPointCircle ? 1 : i + 2;
	}

	/// couleur
	// points
	int decalage = DrawablePolygon::GetOpaqueColourCount();
	for (int i = 0; i < (numPointsPolygon * numVerticesCircle); ++i)
	{
		opaque_color_buffer[decalage + 4 * i + 0] = contourColour.getRed() / 255.0f;
		opaque_color_buffer[decalage + 4 * i + 1] = contourColour.getGreen() / 255.0f;
		opaque_color_buffer[decalage + 4 * i + 2] = contourColour.getBlue() / 255.0f;
		opaque_color_buffer[decalage + 4 * i + 3] = contourColour.getAlpha() / 255.0f;
	}
	// manipulationLine
	decalage += 4 * (numPointsPolygon * numVerticesCircle);
	for (int i = 0; i < dottedLineVertexes; ++i)
	{
		opaque_color_buffer[decalage + 4 * i + 0] = contourColour.getRed() / 255.0f;
		opaque_color_buffer[decalage + 4 * i + 1] = contourColour.getGreen() / 255.0f;
		opaque_color_buffer[decalage + 4 * i + 2] = contourColour.getBlue() / 255.0f;
		opaque_color_buffer[decalage + 4 * i + 3] = contourColour.getAlpha() / 255.0f;
	}
	// manipulationPoint
	decalage += 4 * dottedLineVertexes;
	for (int i = 0; i < numVerticesRing; ++i)
	{
		opaque_color_buffer[decalage + 4 * i + 0] = contourColour.getRed() / 255.0f;
		opaque_color_buffer[decalage + 4 * i + 1] = contourColour.getGreen() / 255.0f;
		opaque_color_buffer[decalage + 4 * i + 2] = contourColour.getBlue() / 255.0f;
		opaque_color_buffer[decalage + 4 * i + 3] = contourColour.getAlpha() / 255.0f;
	}
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

	pointDraggingRadius = 0.01f * (parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0f; // 1%

}

void EditablePolygon::fillOpenGLBuffers()
{
	DrawablePolygon::fillOpenGLBuffers();
	int decalage = DrawablePolygon::GetOpaqueVerticesCount();
	int numApexes = contourPointsInPixels.outer().size() - 1;//isActive? contourPointsInPixels.outer().size() - 1 : 0;

	 /// vertex
	 // points
	for (int k = 0; k < numApexes; ++k)
	{

		for (int j = 0; j < 3 * numVerticesCircle; j += 3)
		{
			opaque_vertex_buffer[3 * decalage + j] = 1.0f* ((float)contourPointsInPixels.outer().at(k).get<0>() + g_vertex_circle[j]);
			opaque_vertex_buffer[3 * decalage + j + 1] = 1.0f*((float)contourPointsInPixels.outer().at(k).get<1>() + g_vertex_circle[j + 1]);
			opaque_vertex_buffer[3 * decalage + j + 2] = 0.1f + g_vertex_circle[j + 2];
		}
		decalage += numVerticesCircle;


	}
	for (int k = numApexes; k < numPointsPolygon; ++k)
	{
		for (int j = 0; j < 3 * numVerticesCircle; j++)
			opaque_vertex_buffer[3 * decalage + j] = 0;
		decalage += numVerticesCircle;
	}

	// manipulationLine + manipulationPoint
	if (isActive)
	{
		computeManipulationLine(centerInPixels.get<0>(), centerInPixels.get<1>(), bmanipulationPointInPixels.get<0>(), bmanipulationPointInPixels.get<1>(), 4.0f, 4.0f);
		for (int i = 0; i < 3 * dottedLineVertexes; ++i)
			opaque_vertex_buffer[3 * decalage + i] = g_vertex_dotted_line[i];
		decalage += dottedLineVertexes;

		for (int j = 0; j < 3 * numVerticesRing; j += 3)
		{

			opaque_vertex_buffer[3 * decalage + j] = 1.0f* (bmanipulationPointInPixels.get<0>() + g_vertex_ring[j]);
			opaque_vertex_buffer[3 * decalage + j + 1] = 1.0f*(bmanipulationPointInPixels.get<1>() + g_vertex_ring[j + 1]);
			opaque_vertex_buffer[3 * decalage + j + 2] = 0.1f + g_vertex_ring[j + 2];

		}
	}
	else
	{
		for (int i = 0; i < 3 * dottedLineVertexes; ++i)
			opaque_vertex_buffer[3 * decalage + i] = 0.0f;
		decalage += dottedLineVertexes;
		for (int j = 0; j < 3 * numVerticesRing; ++j)
			opaque_vertex_buffer[3 * decalage + j] = 0.0f;
	}

	/// index
	// points
	decalage = DrawablePolygon::GetOpaqueVerticesCount(); // decalage dans le buffer index
	int begin = DrawablePolygon::GetOpaqueVerticesCount(); // decalage dans le buffer vertex
	for (int k = 0; k < numApexes; ++k)
	{
		for (int j = 0; j < 3 * numPointCircle; ++j)
		{
			opaque_index_buffer[j + 3 * decalage/*+ numVerticesPolygon*/] = circleIndices[j] + begin + k * numVerticesCircle;
		}
		decalage += numPointCircle;
	}
	for (int k = numApexes; k < numPointsPolygon; ++k)
	{
		for (int j = 0; j < 3 * numPointCircle; ++j)
			opaque_index_buffer[j + 3 * decalage/*+ numVerticesPolygon*/] = 0;
		decalage += numPointCircle;
	}
	begin += numPointsPolygon * numVerticesCircle;

	// manipulationLine + manipulationPoint
	if (isActive)
	{
		for (int i = 0; i < dottedLineIndices; ++i)
			opaque_index_buffer[3 * decalage + i] = g_indices_dotted_line[i] + begin;
		decalage += 2 * dottedLineNparts;
		begin += dottedLineVertexes;
		for (int j = 0; j < 3 * numVerticesRing; ++j)
		{
			opaque_index_buffer[j + 3 * decalage] = ringIndices[j] + begin;
		}
		decalage += numVerticesRing;
	}
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
    if (centerInPixels.get<0>() + manipulationLineLengthRight + manipulationPointRadius
        <= parentCanvas->getWidth())
        bmanipulationPointInPixels = bpt(centerInPixels.get<0>()
                                                  + manipulationLineLengthRight,
                                                  centerInPixels.get<1>());
    else
        bmanipulationPointInPixels = bpt(centerInPixels.get<0>()
                                                  - manipulationLineLengthLeft,
                                                  centerInPixels.get<1>());
}







// ===== SETTERS AND GETTERS =====
void EditablePolygon::SetActive(bool activate)
{
    EditableArea::SetActive(activate);
}

bool EditablePolygon::IsActive()
{
	return isActive;
}

bool EditablePolygon::ShowCenter()
{
	return displayCenter;
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
			verticesChanged = true;
        }
        
        // Are we grabbing one of the contour points ?
        for (size_t i=0; (i < contourPointsInPixels.outer().size()-1 && (eventType!=AreaEventType::PointDragBegins)) ; i++)
        {
            if (boost::geometry::distance(contourPointsInPixels.outer().at(i), bpt(hitPoint.x, hitPoint.y)) < pointDraggingRadius)
            {
                pointDraggedId = (int)i;
				eventType = AreaEventType::PointDragBegins;
				verticesChanged = true;
            }
        }

        // Are we grabbing the center ?
        if (eventType != AreaEventType::PointDragBegins)
        {
			
            if (boost::geometry::distance(centerInPixels, bpt(hitPoint.x, hitPoint.y))
                < (centerCircleRadius+centerContourWidth))
            {
                pointDraggedId = EditableAreaPointId::Center;
				eventType = AreaEventType::PointDragBegins;
				verticesChanged = true;
            }
        }
    }

    // Finally, was the point inside the polygon ? (which starts a translation)
    if (eventType != AreaEventType::PointDragBegins)
    {
        if (hitTest(hitPoint.x,hitPoint.y))
        {
            pointDraggedId = EditableAreaPointId::WholeArea;
            lastLocation = hitPoint;
			eventType = AreaEventType::PointDragBegins;
			verticesChanged = true;
        }
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
        if ( parentCanvas->getLocalBounds().contains(newLocation.toInt())
            && isNewContourPointValid(newLocation))
        {
			contourPointsInPixels.outer().at(pointDraggedId) = bpt(newLocation.x, newLocation.y);
			contourPoints.outer().at(pointDraggedId) = bpt(newLocation.x / (double)parentCanvas->getWidth(),
															newLocation.y / (double)parentCanvas->getHeight());
			if (pointDraggedId == 0) // first point = last point
			{
				contourPointsInPixels.outer().at(contourPointsInPixels.outer().size() - 1) = contourPointsInPixels.outer().at(0);
				contourPoints.outer().at(contourPoints.outer().size() - 1) = contourPoints.outer().at(0);
			}
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
		double r1 = boost::geometry::distance(centerInPixels, bmanipulationPointInPixels);
        double r2 = boost::geometry::distance(centerInPixels, bnewLocation);
       /* double x1 = bmanipulationPointInPixels.get<0>() - centerInPixels.get<0>();
        double x2 = bnewLocation.get<0>() - centerInPixels.get<0>();
        double y1 = bmanipulationPointInPixels.get<1>() - centerInPixels.get<1>();
        double y2 = bnewLocation.get<1>() - centerInPixels.get<1>();


		double cos_a = (x2*x1 + y2*y1)/(r1*r2);
        double sin_a = (y2*x1 - x2*y1)/(r1*r2);*/

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
		// always apply the rotation
		//double radAngle = Math::ComputePositiveAngle(bnewLocation);//atan(sin_a / cos_a);

		Rotate(-radAngle + rotationAngle);
		rotationAngle = radAngle;

		
        if (!wasSizeApplied)
        {
            // If size wasn't applied, we need to rotate the manipulation point
			bpt newManipulationPoint;

			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> trans(-centerInPixels.get<0>(), -centerInPixels.get<1>());
			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invtrans(centerInPixels.get<0>(), centerInPixels.get<1>());
			boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> rot(radAngle - rotationAngle);


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
	verticesChanged = false;

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
    if ( closestDistance <= 10 * pointDraggingRadius )
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

bool EditablePolygon::isNewContourPointValid(const Point<double>& newLocation)
{
    // Init : we save indexes of adjacent points (that will help build the borders)
	int pointBefore;
	if(pointDraggedId != 0)
		pointBefore = Math::Modulo(pointDraggedId - 1, (int)contourPointsInPixels.outer().size());
	else
		pointBefore = Math::Modulo(pointDraggedId - 2, (int)contourPointsInPixels.outer().size());
    int pointAfter = Math::Modulo(pointDraggedId+1, (int)contourPointsInPixels.outer().size());
    /* Étape 1, on construit les équations des droites suivantes :
     * - droite 1 entre le centre et le point d'avant
     * - droite 2 entre le centre et le point d'après
     */
    CartesianLine droite1 = CartesianLine(centerInPixels,
                                          contourPointsInPixels.outer().at(pointBefore));
    CartesianLine droite2 = CartesianLine(centerInPixels,
                                          contourPointsInPixels.outer().at(pointAfter));
    /* Étape 2
     * on vérifie ne pas avoir changé de côté par rapport aux 2 lignes considérées
     */
    if ( droite1.PointWentThrough(contourPointsInPixels.outer().at(pointDraggedId), bpt(newLocation.x, newLocation.y))
        || droite2.PointWentThrough(contourPointsInPixels.outer().at(pointDraggedId), bpt(newLocation.x, newLocation.y)))
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

bpt EditablePolygon::GetManipulationPoint()
{
	return bmanipulationPointInPixels;
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

void EditablePolygon::computeManipulationLine(float Ox, float Oy, float Mx, float My, float width, float height)
{
	int N = 20;
	float length = boost::geometry::distance(bpt(Ox, Oy), bpt(Mx, My));//0.25 * (getWidth() + getHeight()) / 2.0;
	if (length / (2 * height) > 20.0f)
		height = (length / 20.0f) / 2.0f;
	else
		N = length / (2 * height);

	float sina = (My - Oy) / length;
	float cosa = (Mx - Ox) / length;

	for (int i = 0; i < dottedLineNparts; ++i)
	{
		if (i < N)
		{
			// up_left
			g_vertex_dotted_line[i * 3 * 4] = Ox + i * 2 * height * cosa - (width / 2.0) * sina;
			g_vertex_dotted_line[i * 3 * 4 + 1] = Oy + i * 2 * height * sina + (width / 2.0) * cosa;
			g_vertex_dotted_line[i * 3 * 4 + 2] = 0.1f;
			// down_left
			g_vertex_dotted_line[i * 3 * 4 + 3] = Ox + i * 2 * height * cosa + (width / 2.0) * sina;
			g_vertex_dotted_line[i * 3 * 4 + 4] = Oy + i * 2 * height * sina - (width / 2.0) * cosa;
			g_vertex_dotted_line[i * 3 * 4 + 5] = 0.1f;
			// up_right
			g_vertex_dotted_line[i * 3 * 4 + 6] = Ox + (2 * i + 1)  * height * cosa - (width / 2.0) * sina;
			g_vertex_dotted_line[i * 3 * 4 + 7] = Oy + (2 * i + 1) * height * sina + (width / 2.0) * cosa;
			g_vertex_dotted_line[i * 3 * 4 + 8] = 0.1f;
			// down_right
			g_vertex_dotted_line[i * 3 * 4 + 9] = Ox + (2 * i + 1) * height * cosa + (width / 2.0) * sina;
			g_vertex_dotted_line[i * 3 * 4 + 10] = Oy + (2 * i + 1) * height * sina - (width / 2.0) * cosa;
			g_vertex_dotted_line[i * 3 * 4 + 11] = 0.1f;

			g_indices_dotted_line[i * 6] = i * 4;
			g_indices_dotted_line[i * 6 + 1] = i * 4 + 1;
			g_indices_dotted_line[i * 6 + 2] = i * 4 + 2;
			g_indices_dotted_line[i * 6 + 3] = i * 4 + 1;
			g_indices_dotted_line[i * 6 + 4] = i * 4 + 2;
			g_indices_dotted_line[i * 6 + 5] = i * 4 + 3;
		}
		else
		{
			for (int j = 0; j < 12; ++j)
			{
				g_vertex_dotted_line[i * 12 + j] = 0.0f;
			}
			for (int j = 0; j < 6; ++j)
			{
				g_indices_dotted_line[i * 6 + j] = 0;
			}
		}
	}

}
