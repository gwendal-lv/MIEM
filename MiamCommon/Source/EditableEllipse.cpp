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
#include "boost/geometry.hpp"
#include "boost/geometry/strategies/strategy_transform.hpp"

#include "MiamMath.h"
#include "CartesianLine.h"


using namespace Miam;

EditableEllipse::EditableEllipse(bptree::ptree & areaTree)
:
InteractiveEllipse(areaTree)
{
    init();
}

EditableEllipse::EditableEllipse(int64_t _Id) :
	InteractiveEllipse(_Id)
{
	init();
	SetIsRound(false);
}

EditableEllipse::EditableEllipse(int64_t _Id, bpt _center, double _r, Colour _fillColour, float _canvasRatio) :
	InteractiveEllipse(_Id, _center, _r, _r, _fillColour, _canvasRatio)
{
	init();
	SetIsRound(true);
}


EditableEllipse::EditableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
	InteractiveEllipse(_Id, _center, _a, _b, _fillColour, _canvasRatio)
{
	init();
	SetIsRound(false);
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
		if (hitTest(hitPoint.x,hitPoint.y))
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

			// need to bring back contourPointInPixels centered to (0,0) and with rotation = 0
			bpt normalizeNewLocation;
			bpolygon newPolygonInPixels;
			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invTrPixels(-centerInPixels.get<0>(), -centerInPixels.get<1>());
			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> TrPixels(centerInPixels.get<0>(), centerInPixels.get<1>());
			boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> invRot(-rotationAngle);
			boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> Rot(rotationAngle);
			/*boost::geometry::strategy::transform::scale_transformer<double, 2, 2> invScale(1.0 / (double)parentCanvas->getWidth(), 1.0 / (double)parentCanvas->getHeight());
			boost::geometry::strategy::transform::scale_transformer<double, 2, 2> Scale((double)parentCanvas->getWidth(), (double)parentCanvas->getHeight());*/

			boost::geometry::transform(contourPointsInPixels, newPolygonInPixels, invTrPixels);
			boost::geometry::transform(bnewLocation, normalizeNewLocation, invTrPixels);
			boost::geometry::transform(newPolygonInPixels, contourPointsInPixels, Rot);
			boost::geometry::transform(normalizeNewLocation, bnewLocation, Rot);

			double resizeX, resizeY;
			switch (pointDraggedId)
			{
			case 1:
			case 3:
				resizeX = bnewLocation.get<0>() / contourPointsInPixels.outer().at(pointDraggedId).get<0>();
				resizeY = isRound? resizeX : 1;
				a = 2 * abs(bnewLocation.get<0>()) / ((double)parentCanvas->getWidth() * xScale);
				if(isRound)
					b = 2 * abs(bnewLocation.get<0>()) / ((double)parentCanvas->getHeight() * yScale);
				break;
			case 0:
			case 2:
				
				resizeY = bnewLocation.get<1>() / contourPointsInPixels.outer().at(pointDraggedId).get<1>();
				resizeX = isRound? resizeY : 1;
				b = 2 * abs(bnewLocation.get<1>()) / ((double)parentCanvas->getHeight() * yScale);
				if(isRound)
					a = 2 * abs(bnewLocation.get<1>()) / ((double)parentCanvas->getWidth() * xScale);
				break;
			default:
				break;
			}

			boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(resizeX, resizeY);
			boost::geometry::transform(contourPointsInPixels, newPolygonInPixels, scaler);

			boost::geometry::transform(newPolygonInPixels, contourPointsInPixels, invRot);
			boost::geometry::transform(contourPointsInPixels, newPolygonInPixels, TrPixels);
			contourPointsInPixels.clear();
			contourPointsInPixels = newPolygonInPixels;

			contourPoints.clear();
			boost::geometry::strategy::transform::scale_transformer<double, 2, 2> rescaler(1 / ((double)parentCanvas->getWidth()), 1 / ((double)parentCanvas->getHeight()));
			boost::geometry::transform(contourPointsInPixels, contourPoints, rescaler);





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

        // Causera un problème à la fusion GIT
        // Causera un problème à la fusion GIT
        // Causera un problème à la fusion GIT
        /*
		double x1 = bmanipulationPointInPixels.get<0>() - centerInPixels.get<0>();
		double x2 = bnewLocation.get<0>() - centerInPixels.get<0>();
		double y1 = bmanipulationPointInPixels.get<1>() - centerInPixels.get<1>();
		double y2 = bnewLocation.get<1>() - centerInPixels.get<1>();
         */
        // Causera un problème à la fusion GIT
        // Causera un problème à la fusion GIT
        // Causera un problème à la fusion GIT

		//DBG("rotationAngle = " + (String)rotationAngle);

        // Causera un problème à la fusion GIT
        // Causera un problème à la fusion GIT
        // Causera un problème à la fusion GIT
		//double cos_a = (x2*x1 + y2*y1) / (r1*r2);
		//double sin_a = (y2*x1 - x2*y1) / (r1*r2);
        // Causera un problème à la fusion GIT
        // Causera un problème à la fusion GIT
        // Causera un problème à la fusion GIT
		bpt testPt(bnewLocation);
		boost::geometry::subtract_point(testPt, centerInPixels);
		double radAngle = Math::ComputePositiveAngle(testPt);//atan(sin_a / cos_a);//Math::ComputePositiveAngle(bnewLocation);//atan(sin_a / cos_a);
															 // ----- size -----
		double size = r2 / r1;

		bool wasSizeApplied(false);
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
			bpt newManipulationPoint;

			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> trans(-centerInPixels.get<0>(), -centerInPixels.get<1>());
			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invtrans(centerInPixels.get<0>(), centerInPixels.get<1>());
			boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> rot(radAngle - rotationAngle);


			boost::geometry::transform(bmanipulationPointInPixels, newManipulationPoint, trans);
			boost::geometry::transform(newManipulationPoint, bmanipulationPointInPixels, rot);
			boost::geometry::transform(bmanipulationPointInPixels, newManipulationPoint, invtrans);

			bmanipulationPointInPixels = newManipulationPoint;
		}

		updateContourPoints();
	}
	else if (pointDraggedId == EditableAreaPointId::Center)
	{
		if (isNewCenterValid(newLocation))
		{
			centerInPixels = bnewLocation;
			boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(1.0 / ((double)parentCanvas->getWidth()), 1.0 / ((double)parentCanvas->getWidth()));
			bpt newCenter;
			boost::geometry::transform(bnewLocation, newCenter, scaler);

			bpolygon newContoursPoint;
			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> tr(newCenter.get<0>()- center.get<0>(), newCenter.get<1>() - center.get<1>());
			boost::geometry::transform(contourPoints, newContoursPoint, tr);

			center = newCenter;
			contourPoints.clear();
			contourPoints = newContoursPoint;

			


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
	{
		InteractiveEllipse::CanvasResized(this->parentCanvas);
       
	}


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

bool EditableEllipse::SizeChanged(double size, bool minSize)
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
		a *= size; // faire diviser pas Heght et width?
		b *= size;
	}


	return returnValue;
}

void EditableEllipse::Rotate(double Radian)
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

void EditableEllipse::updateContourPoints()
{
	contourPoints.clear();
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(1 / ((double)parentCanvas->getWidth()), 1 / ((double)parentCanvas->getHeight()));
	boost::geometry::transform(contourPointsInPixels, contourPoints, scaler);
}

void EditableEllipse::Translate(const Point<double>& translation)
{
	
	

	//// Manipulation point (+ line...)
	//computeManipulationPoint();
	bpt newCenterInPixels;
	bpolygon newContourPointsInPixels;
	newContourPointsInPixels.clear();
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> btranslation(translation.x, translation.y);
	boost::geometry::transform(centerInPixels, newCenterInPixels,btranslation);
	boost::geometry::transform(contourPointsInPixels, newContourPointsInPixels,btranslation);

	bpt newCenter;
	bpolygon newContourPoints;
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> bscale(1.0 / (double)parentCanvas->getWidth(), 1.0 / (double)parentCanvas->getHeight());
	boost::geometry::transform(newCenterInPixels, newCenter, bscale);
	boost::geometry::transform(newContourPointsInPixels, newContourPoints, bscale);

	centerInPixels = newCenterInPixels;
	contourPointsInPixels = newContourPointsInPixels;
	center = newCenter;
	contourPoints = newContourPoints;

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
	int pointBefore;
	if (pointDraggedId != 0)
		pointBefore = Math::Modulo(pointDraggedId - 1, (int)contourPointsInPixels.outer().size());
	else
		pointBefore = Math::Modulo(pointDraggedId - 2, (int)contourPointsInPixels.outer().size());
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
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(1.0 / (double)parentCanvas->getWidth(), 1.0 / (double)parentCanvas->getHeight());
	boost::geometry::transform(contourPointsInPixels, contourPoints,scaler);
	boost::geometry::transform(centerInPixels, center, scaler);
}

double EditableEllipse::getRadius()
{
	return boost::geometry::distance(centerInPixels, contourPointsInPixels.outer().at(0));
}
