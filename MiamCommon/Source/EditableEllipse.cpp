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
		DBG("contour dragging");
		DBG("isNewContourPointValid = " + (String)isNewContourPointValid(newLocation));
		if (parentCanvas->getLocalBounds().contains(newLocation.toInt())
			&& isNewContourPointValid(newLocation))
		{
			DBG("(a,b) = (" + (String)a + " , " + (String)b + ")");
			/*DBG("centerInPixels" + (String)centerInPixels.get<0>() + " " + (String)centerInPixels.get<1>());
			DBG("center" + (String)center.get<0>() + " " + (String)center.get<1>());*/
			DBG("0 : " + (String)contourPointsInPixels.outer().at(0).get<0>() + " " + (String)contourPointsInPixels.outer().at(0).get<1>());
			DBG("1 : " + (String)contourPointsInPixels.outer().at(1).get<0>() + " " + (String)contourPointsInPixels.outer().at(1).get<1>());
			DBG("2 : " + (String)contourPointsInPixels.outer().at(2).get<0>() + " " + (String)contourPointsInPixels.outer().at(2).get<1>());
			DBG("3 : " + (String)contourPointsInPixels.outer().at(3).get<0>() + " " + (String)contourPointsInPixels.outer().at(3).get<1>());
			double sa,sb;
			double resizeX, resizeY;
			switch (pointDraggedId)
			{
			case 1 : // (Cx +a/2, Cy     )
			case 3 : // (Cx -a/2, Cy     )
				sa = bnewLocation.get<0>() - centerInPixels.get<0>();
				resizeX = (2*abs(sa)/ (double)parentCanvas->getWidth()) / a;
				resizeY = 1;
				
				
				a = 2 * abs(sa) / (double)parentCanvas->getWidth();
				
				/*contourPoints.outer().at(1) = bpt(center.get<0>() + (a / 2)*xScale, center.get<1>());
				contourPoints.outer().at(3) = bpt(center.get<0>() - (a / 2)*xScale, center.get<1>());*/

				
				

				/*contourPointsInPixels.outer().at(1).set<0>(contourPoints.outer().at(1).get<0>()*(double)parentCanvas->getWidth());
				contourPointsInPixels.outer().at(3).set<0>(contourPoints.outer().at(3).get<0>()*(double)parentCanvas->getWidth());*/

				

				break;
			case 0 : // (Cx     , Cy +b/2)
			case 2 : // (Cx     , Cy -b/2)
				sb = bnewLocation.get<1>() - centerInPixels.get<1>();
				resizeX = 1;
				resizeY = (2*abs(sb)/ (double)parentCanvas->getHeight()) / b;
				
				b = 2 * abs(sb) / (double)parentCanvas->getHeight();


				/*contourPoints.outer().at(0).set<1>(center.get<1>() - (b / 2)*yScale);
				contourPoints.outer().at(2).set<1>(center.get<1>() + (b / 2)*yScale);
				contourPointsInPixels.outer().at(0).set<1>(contourPoints.outer().at(0).get<1>()*(double)parentCanvas->getHeight());
				contourPointsInPixels.outer().at(2).set<1>(contourPoints.outer().at(2).get<1>()*(double)parentCanvas->getHeight());*/
				break;
			default:
				break;
			}
			DBG("new (a,b) = (" + (String)a + " , " + (String)b + ")");
			
			bpolygon newContourPoints;
			boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(resizeX,resizeY);
			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> trans(-(double)center.get<0>(), -(double)center.get<1>());
			boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invtrans((double)center.get<0>(), (double)center.get<1>());
			boost::geometry::transform(contourPoints, newContourPoints, trans);
			contourPoints.clear();
			boost::geometry::transform(newContourPoints, contourPoints, scaler);
			newContourPoints.clear();
			boost::geometry::transform(contourPoints, newContourPoints, invtrans);
			contourPoints.clear();
			contourPoints = newContourPoints;
			contourPointsInPixels.clear();

			
			areaEventType = AreaEventType::ShapeChanged;
		}
		
	}
	else if (pointDraggedId == EditableAreaPointId::ManipulationPoint)
	{
		DBG("manipulation point dragged");
		DBG("0 : " + (String)contourPointsInPixels.outer().at(0).get<0>() + " " + (String)contourPointsInPixels.outer().at(0).get<1>());
		DBG("1 : " + (String)contourPointsInPixels.outer().at(1).get<0>() + " " + (String)contourPointsInPixels.outer().at(1).get<1>());
		DBG("2 : " + (String)contourPointsInPixels.outer().at(2).get<0>() + " " + (String)contourPointsInPixels.outer().at(2).get<1>());
		DBG("3 : " + (String)contourPointsInPixels.outer().at(3).get<0>() + " " + (String)contourPointsInPixels.outer().at(3).get<1>());
		// Just resize
		// Security needed for point to stay within the canvas ?
		areaEventType = AreaEventType::RotScale;

		double r1 = boost::geometry::distance(centerInPixels, bmanipulationPointInPixels);
		double r2 = boost::geometry::distance(centerInPixels, bnewLocation);
		// ----- size -----
		double size = r2 / r1;
		if (size > 2)
			DBG("size : " + (String)size + ", r1 =  " +(String)r1 + " r2 = " + (String)r2);
		else
			DBG("size : " + (String)size);
		if (SizeChanged(size))
		{
			bmanipulationPointInPixels.set<0>(bnewLocation.get<0>());
			bmanipulationPointInPixels.set<1>(bnewLocation.get<1>());
		}
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

			/*center = bpt(bnewLocation.get<0>() / ((double)parentCanvas->getWidth()),
				bnewLocation.get<1>() / ((double)parentCanvas->getHeight()));

			contourPoints.outer().at(0) = bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale);
			contourPoints.outer().at(1) = bpt(center.get<0>() + (a / 2)*xScale, center.get<1>());
			contourPoints.outer().at(2) = bpt(center.get<0>(), center.get<1>() + (b / 2)*yScale);
			contourPoints.outer().at(3) = bpt(center.get<0>() - (a / 2)*xScale, center.get<1>());*/


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
		DBG("0 : " + (String)contourPointsInPixels.outer().at(0).get<0>() + " " + (String)contourPointsInPixels.outer().at(0).get<1>());
		DBG("1 : " + (String)contourPointsInPixels.outer().at(1).get<0>() + " " + (String)contourPointsInPixels.outer().at(1).get<1>());
		DBG("2 : " + (String)contourPointsInPixels.outer().at(2).get<0>() + " " + (String)contourPointsInPixels.outer().at(2).get<1>());
		DBG("3 : " + (String)contourPointsInPixels.outer().at(3).get<0>() + " " + (String)contourPointsInPixels.outer().at(3).get<1>());
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

bool EditableEllipse::SizeChanged(double size)
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

	//bpt newManipulationPt, newManipulationPt2;
	//boost::geometry::transform(bmanipulationPointInPixels, newManipulationPt, trans);
	//boost::geometry::transform(newManipulationPt, newManipulationPt2, resizer);
	//boost::geometry::transform(newManipulationPt2, newManipulationPt, invtrans);

	// si 
	//for (size_t i = 0; i<contourPointsInPixels.outer().size(); i++)
	//{
	//	/*std::vector<bpt> result;
	//	boost::geometry::difference(contourPointsInPixels.outer().at(i), centerInPixels, result);
	//	bnewContourPoints.outer().push_back(result.front());*/

	//	

	//	bnewContourPoints.outer().push_back(bpt(contourPointsInPixels.outer().at(i).get<0>() - centerInPixels.get<0>(),
	//		contourPointsInPixels.outer().at(i).get<1>() - centerInPixels.get<1>()));


	//	bnewContourPoints.outer().at(i) = bpt(size * bnewContourPoints.outer().at(i).get<0>(),
	//		size * bnewContourPoints.outer().at(i).get<1>());
	//	/*if (boost::geometry::distance(bnewContourPoints.outer().at(i), bpt(0, 0)) > minDistanceFromCenter)
	//		minDistanceFromCenter = boost::geometry::distance(bnewContourPoints.outer().at(i), bpt(0, 0));

	//	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> tr(centerInPixels.get<0>(), centerInPixels.get<1>());
	//	boost::geometry::transform(bnewContourPoints.outer().at(i), bnewContourPoints.outer().at(i), tr);*/
	//	if (boost::geometry::distance(bnewContourPoints.outer().at(i), bpt(0, 0)) > minDistanceFromCenter)
	//		minDistanceFromCenter = boost::geometry::distance(bnewContourPoints.outer().at(i), bpt(0, 0));

	//	bnewContourPoints.outer().at(i).set<0>(bnewContourPoints.outer().at(i).get<0>() + centerInPixels.get<0>());
	//	bnewContourPoints.outer().at(i).set<1>(bnewContourPoints.outer().at(i).get<1>() + centerInPixels.get<1>());

	//}
	/*boost::geometry::detail::distance::point_to_polygon<bpt, bpolygon, boost::geometry::closure<, boost::geometry::strategy::distance::pythagoras<>>::apply(centerInPixels, testBoost, boost::geometry::strategy::distance::pythagoras<>());
	DBG("autre test : " + (String)boost::geometry::distance(testBoost.outer(),centerInPixels));
	DBG("test = " + (String)boost::geometry::comparable_distance(testBoost, centerInPixels, boost::geometry::strategy::distance::pythagoras<double>()));
	DBG("maxToCenterTest = " + (String)boost::geometry::distance(testBoost, centerInPixels));
	DBG("maxToCenter = " + (String)boost::geometry::distance(centerInPixels, testBoost));
	double test = boost::geometry::comparable_distance(testBoost, centerInPixels, boost::geometry::strategy::distance::pythagoras<double>());
	double maxToCenterTest = boost::geometry::distance(testBoost, centerInPixels);*/
	
	/*double maxToCenter = boost::geometry::distance(centerInPixels,testBoost);
	if (maxToCenter > minDistanceFromCenter)
		minDistanceFromCenter = maxToCenter;*/

	for (size_t i = 0; i < testBoost.outer().size(); i++)
	{
		//if (testBoost.outer().at(i).get<0>() < 0 || testBoost.outer().at(i).get<1>() < 0)
		//	DBG("probleme");
		if (boost::geometry::distance(testBoost.outer().at(i), centerInPixels) > minDistanceFromCenter)
			minDistanceFromCenter = boost::geometry::distance(testBoost.outer().at(i), centerInPixels);
	}
	//std::vector<bpolygon> comparaison;
	//boost::geometry::difference(bnewContourPoints, testBoost, comparaison);
	
	if (minDistanceFromCenter >=
		minimumSizePercentage*(parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0)
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

	// After manipulation computation : normalized coordinates update
	contourPoints.clear();
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(1/ ((double)parentCanvas->getWidth()), 1/ ((double)parentCanvas->getHeight()));
	boost::geometry::transform(contourPointsInPixels, contourPoints, scaler);
	return returnValue;
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
