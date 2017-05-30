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


using namespace Miam;

EditableEllipse::EditableEllipse(int64_t _Id) :
	InteractiveEllipse(_Id)
{
	init();
}


EditableEllipse::EditableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
	InteractiveEllipse(_Id, _center, _a, _b, _fillColour, _canvasRatio)
{
	init();
}

void EditableEllipse::init()
{
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
	DrawableEllipse::Paint(g);

	
	if (isActive)
	{
		g.setColour(editingElementsColour);

		
		if (!enableTranslationOnly)
		{
			
			// Then, we draw the coutour draggable points
			for (size_t i = 0; i<contourPointsInPixels.outer().size(); i++)
			{
				g.fillEllipse((float)contourPointsInPixels.outer().at(i).get<0>() - contourPointsRadius,
					(float)contourPointsInPixels.outer().at(i).get<1>() - contourPointsRadius,
					contourPointsRadius*2.0f,
					contourPointsRadius*2.0f);
			}

			// And finally, the manipulation (rotation & scale) main control
			Line<float> manipulationLine = Line<float>((float)centerInPixels.get<0>(),
													   (float)centerInPixels.get<1>(),
													   (float)bmanipulationPointInPixels.get<0>(),
													   (float)bmanipulationPointInPixels.get<1>());
			float dashedLineParameters[] = { 4.0f, 4.0f };
			g.drawDashedLine(manipulationLine, dashedLineParameters, 2, centerContourWidth);
			g.fillEllipse((float)bmanipulationPointInPixels.get<0>() - manipulationPointRadius,
				(float)bmanipulationPointInPixels.get<1>() - manipulationPointRadius,
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
	
	AreaEventType eventType = AreaEventType::NothingHappened;
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
			< (centerCircleRadius + centerContourWidth)) // same radius than the center
		{
			pointDraggedId = EditableAreaPointId::ManipulationPoint;
			eventType = AreaEventType::PointDragBegins;
		}
		
		// Are we grabbing one of the contour points ?
		for (size_t i = 0; (i < contourPointsInPixels.outer().size() && (eventType != AreaEventType::PointDragBegins)); i++)
		{
			
			if (boost::geometry::distance(contourPointsInPixels.outer().at(i), bpt(hitPoint.x, hitPoint.y)) < pointDraggingRadius)
			{
				pointDraggedId = (int)i;
				eventType = AreaEventType::PointDragBegins;
			}
		}

		// Are we grabbing the center ?
		if (eventType != AreaEventType::PointDragBegins)
		{
			if (boost::geometry::distance(centerInPixels, bpt(hitPoint.x, hitPoint.y))
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
		if (HitTest(hitPoint.x,hitPoint.y))
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
	bpt bnewLocation(newLocation.x, newLocation.y);
	AreaEventType areaEventType = AreaEventType::NothingHappened;

	// Simple contour point dragging
	if (pointDraggedId >= 0)
	{
		
		if (parentCanvas->getLocalBounds().contains(newLocation.toInt())
			&& isNewContourPointValid(newLocation))
		{
			
			double sa,sb;
			switch (pointDraggedId)
			{
			case 1 : // (Cx +a/2, Cy     )
			case 3 : // (Cx -a/2, Cy     )
				sa = bnewLocation.get<0>() - centerInPixels.get<0>();
				a = 2 * abs(sa) / (double)parentCanvas->getWidth();

				contourPoints.outer().at(1) = bpt(center.get<0>() + (a / 2)*xScale, center.get<1>());
				contourPoints.outer().at(3) = bpt(center.get<0>() - (a / 2)*xScale, center.get<1>());
				contourPointsInPixels.outer().at(1).set<0>(contourPoints.outer().at(1).get<0>()*(double)parentCanvas->getWidth());
				contourPointsInPixels.outer().at(3).set<0>(contourPoints.outer().at(3).get<0>()*(double)parentCanvas->getWidth());

				break;
			case 0 : // (Cx     , Cy +b/2)
			case 2 : // (Cx     , Cy -b/2)
				sb = bnewLocation.get<1>() - centerInPixels.get<1>();
				b = 2 * abs(sb) / (double)parentCanvas->getHeight();

				contourPoints.outer().at(0).set<1>(center.get<1>() - (b / 2)*yScale);
				contourPoints.outer().at(2).set<1>(center.get<1>() + (b / 2)*yScale);
				contourPointsInPixels.outer().at(0).set<1>(contourPoints.outer().at(0).get<1>()*(double)parentCanvas->getHeight());
				contourPointsInPixels.outer().at(2).set<1>(contourPoints.outer().at(2).get<1>()*(double)parentCanvas->getHeight());
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

		double r1 = boost::geometry::distance(centerInPixels, bmanipulationPointInPixels);
		double r2 = boost::geometry::distance(centerInPixels, bnewLocation);
		// ----- size -----
		double size = r2 / r1;

		// --- size if polygon is still big enough only ---
		double minDistanceFromCenter = 0.0;
		bool wasSizeApplied = false;
		bpolygon bnewContourPoints;
		for (size_t i = 0; i<contourPointsInPixels.outer().size(); i++)
		{
			std::vector<bpt> result;
			boost::geometry::difference(contourPointsInPixels.outer().at(i), centerInPixels, result);
			bnewContourPoints.outer().push_back(result.front());
			bnewContourPoints.outer().at(i) = bpt(size * bnewContourPoints.outer().at(i).get<0>(),
				size * bnewContourPoints.outer().at(i).get<1>());
			if (boost::geometry::distance(bnewContourPoints.outer().at(i), bpt(0, 0)) > minDistanceFromCenter)
				minDistanceFromCenter = boost::geometry::distance(bnewContourPoints.outer().at(i), bpt(0, 0));

			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> tr(centerInPixels.get<0>(), centerInPixels.get<1>());
			boost::geometry::transform(bnewContourPoints.outer().at(i), bnewContourPoints.outer().at(i), tr);

		}
		if (minDistanceFromCenter >=
			minimumSizePercentage*(parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0)
		{
			wasSizeApplied = true;
			contourPointsInPixels = bnewContourPoints;
			bmanipulationPointInPixels = bnewLocation;
			a *= size;
			b *= size;
		}
		// After manipulation computation : normalized coordinates update
		for (size_t i = 0; i < contourPointsInPixels.outer().size(); i++)
		{
			contourPoints.outer().at(i) = bpt(
				contourPointsInPixels.outer().at(i).get<0>() / ((double)parentCanvas->getWidth()),
				contourPointsInPixels.outer().at(i).get<1>() / ((double)parentCanvas->getHeight()));
		}
	}

	else if (pointDraggedId == EditableAreaPointId::Center)
	{
		if (isNewCenterValid(newLocation))
		{
			centerInPixels = bnewLocation;
			center = bpt(bnewLocation.get<0>() / ((double)parentCanvas->getWidth()),
				bnewLocation.get<1>() / ((double)parentCanvas->getHeight()));

			contourPoints.outer().at(0) = bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale);
			contourPoints.outer().at(1) = bpt(center.get<0>() + (a / 2)*xScale, center.get<1>());
			contourPoints.outer().at(2) = bpt(center.get<0>(), center.get<1>() + (b / 2)*yScale);
			contourPoints.outer().at(3) = bpt(center.get<0>() - (a / 2)*xScale, center.get<1>());


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
	centerInPixels.set<0>(centerInPixels.get<0>() + translation.x);
	centerInPixels.set<1>(centerInPixels.get<1>() + translation.y);
	center = bpt(centerInPixels.get<0>() / ((double)parentCanvas->getWidth()),
		centerInPixels.get<1>() / ((double)parentCanvas->getHeight()));
	for (size_t i = 0; i < contourPointsInPixels.outer().size(); i++)
	{
		contourPointsInPixels.outer().at(i).set<0>(contourPointsInPixels.outer().at(i).get<0>() + translation.x);
		contourPointsInPixels.outer().at(i).set<1>(contourPointsInPixels.outer().at(i).get<1>() + translation.y);
		contourPoints.outer().at(i) = bpt(
			contourPointsInPixels.outer().at(i).get<0>() / ((double)parentCanvas->getWidth()),
			contourPointsInPixels.outer().at(i).get<1>() / ((double)parentCanvas->getHeight()));
	}

	// Manipulation point (+ line...)
	computeManipulationPoint();
}

void EditableEllipse::setCenterPosition(bpt newCenter) // pixels
{
	
	Point<double> translation(newCenter.get<0>() - centerInPixels.get<0>(), newCenter.get<0>() - centerInPixels.get<0>()); // pixels
	Translate(translation);
	
}






// ===== EDITING HELPERS =====

bool EditableEllipse::isNewContourPointValid(const Point<double>& newLocation)
{
	
	// Init : we save indexes of adjacent points (that will help build the borders)
	int pointBefore = Math::Modulo(pointDraggedId - 1, (int)contourPointsInPixels.outer().size());
	int pointAfter = Math::Modulo(pointDraggedId + 1, (int)contourPointsInPixels.outer().size());
	
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
	
	if (droite1.PointWentThrough(contourPointsInPixels.outer().at(pointDraggedId), bpt(newLocation.x,newLocation.y))
		|| droite2.PointWentThrough(contourPointsInPixels.outer().at(pointDraggedId), bpt(newLocation.x,newLocation.y)))
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
	for (size_t i = 0; (i<contourPointsInPixels.outer().size() && !hasCrossed); i++)
	{
		CartesianLine cartesianLine = CartesianLine(contourPointsInPixels.outer().at(i),
			contourPointsInPixels.outer().at(Math::Modulo((int)i + 1, (int)contourPointsInPixels.outer().size())));
		hasCrossed = cartesianLine.PointWentThrough(centerInPixels, bpt(newLocation.x,newLocation.y));
	}
	return !hasCrossed;
}

void EditableEllipse::recreateNormalizedPoints()
{
	contourPoints.clear();
	for (size_t i = 0; i<contourPointsInPixels.outer().size(); i++)
	{
		contourPoints.outer().push_back(bpt(contourPointsInPixels.outer().at(i).get<0>() / parentCanvas->getWidth(),
			contourPointsInPixels.outer().at(i).get<1>() / parentCanvas->getHeight()));
	}
	center = bpt(centerInPixels.get<0>() / parentCanvas->getWidth(),
		centerInPixels.get<1>() / parentCanvas->getHeight());
}
