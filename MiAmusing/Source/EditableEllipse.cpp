/*
  ==============================================================================

    EditableEllipse.cpp
    Created: 2 Feb 2017 2:17:32pm
    Author:  ayup1

  ==============================================================================
*/

#include "EditableEllipse.h"
#include "SceneCanvasComponent.h"

#include <cmath>

#include "MiamMath.h"
#include "CartesianLine.h"

using namespace Amusing;
using namespace Miam;

EditableEllipse::EditableEllipse(int64_t _Id) :
	InteractiveEllipse(_Id)
{
	DBG("EditableEllipse contructor");
	init();
}

EditableEllipse::EditableEllipse(int64_t _Id, Point<double> _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
	InteractiveEllipse(_Id,_center,_a,_b,_fillColour,_canvasRatio)
{
	DBG("EditableEllipse contructor");
	init();
}

void EditableEllipse::init()
{
	DBG("EditableEllipse::init");
	graphicalInit();
	behaviorInit();
}

void EditableEllipse::graphicalInit()
{
	contourColour = Colour(0xAAFFFFFF); // white, but not totally opaque

	editingElementsColour = Colours::white; // pure white (more visible)
	contourPointsRadius = 1.4f*contourWidth;
	manipulationPointRadius = centerContourWidth + centerCircleRadius;
}

void EditableEllipse::behaviorInit()
{
	SetActive(false);
	pointDraggedId = EditableAreaPointId::None;
}

// ========== DRAWING ==========

void EditableEllipse::Paint(Graphics& g)
{
	// At first, we draw the normal surface
	//DBG("EditableEllipse::paint");
	DrawableEllipse::Paint(g);

	//DBG("contourPointRadius = " + (String)contourPointsRadius);
	//DBG("nbr contourPoints = " + (String)contourPointsInPixels.size());
	if (isActive)
	{
		g.setColour(editingElementsColour);

		//DBG("contourPointRadius = " + (String)contourPointsRadius);
		//DBG("nbr contourPoints = " + (String)contourPointsInPixels.size());
		if (!enableTranslationOnly)
		{
			//DBG("contourPointRadius = " + (String)contourPointsRadius);
			//DBG("nbr contourPoints = " + (String)contourPointsInPixels.size());
			// Then, we draw the coutour draggable points
			for (size_t i = 0; i<contourPointsInPixels.size(); i++)
			{
				g.fillEllipse((float)contourPointsInPixels[i].x - contourPointsRadius,
					(float)contourPointsInPixels[i].y - contourPointsRadius,
					contourPointsRadius*2.0f,
					contourPointsRadius*2.0f);
			}

			// And finally, the manipulation (rotation & scale) main control
			Line<float> manipulationLine = Line<float>((float)centerInPixels.x,
													   (float)centerInPixels.y,
													   (float)manipulationPointInPixels.x,
													   (float)manipulationPointInPixels.y);
			float dashedLineParameters[] = { 4.0f, 4.0f };
			g.drawDashedLine(manipulationLine, dashedLineParameters, 2, centerContourWidth);
			g.fillEllipse((float)manipulationPointInPixels.x - manipulationPointRadius,
				(float)manipulationPointInPixels.y - manipulationPointRadius,
				(float)manipulationPointRadius*2.0f,
				(float)manipulationPointRadius*2.0f);
		}
	}
}






// =========== GRAPHICAL UPDATES ==========

// Whole actualization of pixel-coordinates graphical objects
void EditableEllipse::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
	InteractiveEllipse::CanvasResized(_parentCanvas);

	// Manipulation point (+ line...)
	computeManipulationPoint();

	pointDraggingRadius = 0.01f * (parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0f; // 1%
}

void EditableEllipse::computeManipulationPoint()
{
	float manipulationLineLengthLeft = 0.25f*(parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0f,
		manipulationLineLengthRight = manipulationLineLengthLeft; //px
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
void EditableEllipse::SetActive(bool activate)
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

AreaEventType EditableEllipse::TryBeginPointMove(const Point<double>& hitPoint)
{
	DBG("TryBeginPointMove");
	AreaEventType eventType = AreaEventType::NothingHappened;
	if (pointDraggedId != EditableAreaPointId::None)
	{
		eventType = AreaEventType::AnotherMonoTouchPointDragAlreadyBegun;
		return eventType;
	}

	// If any manipulation is authorized (not only the translation)
	if (!enableTranslationOnly)
	{
		DBG("hit point = (" + (String)hitPoint.getX() + " , " + (String)hitPoint.getY() + ")");

		// Are we grabbing the manipulation dot ?
		if (manipulationPointInPixels.getDistanceFrom(hitPoint)
			< (centerCircleRadius + centerContourWidth)) // same radius than the center
		{
			pointDraggedId = EditableAreaPointId::ManipulationPoint;
			eventType = AreaEventType::PointDragBegins;
		}

		// Are we grabbing one of the contour points ?
		for (size_t i = 0; (i < contourPointsInPixels.size() && (eventType != AreaEventType::PointDragBegins)); i++)
		{
			if (contourPointsInPixels[i].getDistanceFrom(hitPoint) < pointDraggingRadius)
			{
				pointDraggedId = (int)i;
				eventType = AreaEventType::PointDragBegins;
			}
		}

		// Are we grabbing the center ?
		if (eventType != AreaEventType::PointDragBegins)
		{
			if (centerInPixels.getDistanceFrom(hitPoint.toDouble())
				< (centerCircleRadius + centerContourWidth))
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

AreaEventType EditableEllipse::TryMovePoint(const Point<double>& newLocation)
{
	AreaEventType areaEventType = AreaEventType::NothingHappened;

	//DBG("[0] = " + (String)contourPointsInPixels[0].getX());
	//DBG("[0] = " + (String)contourPoints[0].getX());

	//DBG("manip = " + (String)manipulationPointInPixels.getX());

	// Simple contour point dragging
	if (pointDraggedId >= 0)
	{
		if (parentCanvas->getLocalBounds().contains(newLocation.toInt())
			&& isNewContourPointValid(newLocation))
		{
			switch (pointDraggedId)
			{
			case 0 : // (Cx +a/2, Cy     )
			case 1 : // (Cx -a/2, Cy     )
				contourPointsInPixels[pointDraggedId].setX(newLocation.x);
				contourPoints[pointDraggedId].setX(newLocation.x / (double)parentCanvas->getWidth());
				a = abs(contourPointsInPixels[pointDraggedId].getDistanceFrom(centerInPixels));
				break;
			case 2 : // (Cx     , Cy +b/2)
			case 3 : // (Cx     , Cy -b/2)
				contourPointsInPixels[pointDraggedId].setY(newLocation.y);
				contourPoints[pointDraggedId].setY(newLocation.y / (double)parentCanvas->getHeight());
				a = abs(contourPointsInPixels[pointDraggedId].getDistanceFrom(centerInPixels));
				break;
			default:
				break;
			}
			areaEventType = AreaEventType::ShapeChanged;
		}
		
	}
	else if (pointDraggedId == EditableAreaPointId::ManipulationPoint)
	{
		// Just resize
		// Security needed for point to stay within the canvas ?
		areaEventType = AreaEventType::RotScale;

		double r1 = centerInPixels.getDistanceFrom(manipulationPointInPixels);
		double r2 = centerInPixels.getDistanceFrom(newLocation);
		// ----- size -----
		double size = r2 / r1;

		// --- size if polygon is still big enough only ---
		double minDistanceFromCenter = 0.0;
		bool wasSizeApplied = false;
		std::vector<Point<double>> newContourPoints;
		for (size_t i = 0; i<contourPointsInPixels.size(); i++)
		{
			newContourPoints.push_back(contourPointsInPixels[i] - centerInPixels);
			newContourPoints[i] = Point<double>(size * newContourPoints[i].x,
				size * newContourPoints[i].y);
			if (newContourPoints[i].getDistanceFromOrigin() > minDistanceFromCenter)
				minDistanceFromCenter = newContourPoints[i].getDistanceFromOrigin();
			newContourPoints[i] += centerInPixels;
		}
		if (minDistanceFromCenter >=
			minimumSizePercentage*(parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0)
		{
			wasSizeApplied = true;
			contourPointsInPixels = newContourPoints;
			manipulationPointInPixels = newLocation;
			a *= size;
			b *= size;
		}
		// After manipulation computation : normalized coordinates update
		for (size_t i = 0; i < contourPointsInPixels.size(); i++)
		{
			contourPoints[i] = Point<double>(
				contourPointsInPixels[i].x / ((double)parentCanvas->getWidth()),
				contourPointsInPixels[i].y / ((double)parentCanvas->getHeight()));
		}
	}

	else if (pointDraggedId == EditableAreaPointId::Center)
	{
		if (isNewCenterValid(newLocation))
		{
			centerInPixels = newLocation;
			center = Point<double>(newLocation.x / ((double)parentCanvas->getWidth()),
				newLocation.y / ((double)parentCanvas->getHeight()));

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
		InteractiveEllipse::CanvasResized(this->parentCanvas);

	return areaEventType;
}

AreaEventType EditableEllipse::EndPointMove()
{
	// Always initialized, if some more complex code is written below, later
	AreaEventType eventType = AreaEventType::NothingHappened;

	// The point drag is always stopped without any check, for now
	computeManipulationPoint();
	pointDraggedId = EditableAreaPointId::None;
	eventType = AreaEventType::PointDragStops;

	return eventType;
}

void EditableEllipse::Translate(const Point<double>& translation)
{
	centerInPixels += translation;
	center = Point<double>(centerInPixels.x / ((double)parentCanvas->getWidth()),
		centerInPixels.y / ((double)parentCanvas->getHeight()));
	for (size_t i = 0; i < contourPointsInPixels.size(); i++)
	{
		contourPointsInPixels[i] += translation;
		contourPoints[i] = Point<double>(
			contourPointsInPixels[i].x / ((double)parentCanvas->getWidth()),
			contourPointsInPixels[i].y / ((double)parentCanvas->getHeight()));
	}

	// Manipulation point (+ line...)
	computeManipulationPoint();
}




// ===== EDITING HELPERS =====

bool EditableEllipse::isNewContourPointValid(const Point<double>& newLocation)
{
	// Init : we save indexes of adjacent points (that will help build the borders)
	int pointBefore = Math::Modulo(pointDraggedId - 1, (int)contourPointsInPixels.size());
	int pointAfter = Math::Modulo(pointDraggedId + 1, (int)contourPointsInPixels.size());
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
	if (droite1.PointWentThrough(contourPointsInPixels[pointDraggedId], newLocation)
		|| droite2.PointWentThrough(contourPointsInPixels[pointDraggedId], newLocation))
	{
		return false;
	}
	else
		return true;
}

bool EditableEllipse::isNewCenterValid(const Point<double>& newLocation)
{
	// We check if the center did not cross all the lines, one-by-one
	bool hasCrossed = false;
	for (size_t i = 0; (i<contourPointsInPixels.size() && !hasCrossed); i++)
	{
		CartesianLine cartesianLine = CartesianLine(contourPointsInPixels[i],
			contourPointsInPixels[Math::Modulo((int)i + 1, (int)contourPointsInPixels.size())]);
		hasCrossed = cartesianLine.PointWentThrough(centerInPixels, newLocation);
	}
	return !hasCrossed;
}

void EditableEllipse::recreateNormalizedPoints()
{
	contourPoints.clear();
	for (size_t i = 0; i<contourPointsInPixels.size(); i++)
	{
		contourPoints.push_back(Point<double>(contourPointsInPixels[i].x / parentCanvas->getWidth(),
			contourPointsInPixels[i].y / parentCanvas->getHeight()));
	}
	center = Point<double>(centerInPixels.x / parentCanvas->getWidth(),
		centerInPixels.y / parentCanvas->getHeight());
}