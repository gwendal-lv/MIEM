/*
  ==============================================================================

    CompletePolygon.cpp
    Created: 5 May 2017 11:39:08am
    Author:  ayup1

  ==============================================================================
*/

#include "CompletePolygon.h"
#include "SceneCanvasComponent.h"

#include "MiamMath.h"
#include <cmath>

#include "boost\geometry.hpp"
#include "boost\geometry\algorithms\transform.hpp"

using namespace Amusing;
using namespace Miam;

CompletePolygon::CompletePolygon(int64_t _Id) : EditablePolygon(_Id)
{
	showCursor = false;
	pc = 0;
	cursorCenter = contourPoints.outer().at(0);
	cursor = std::shared_ptr<EditableEllipse>(new EditableEllipse(0, cursorCenter, 0.1f, 0.1f, Colours::grey, 1.47f));
	cursor->SetNameVisible(false);
	//percentages.reserve(contourPoints.size());
	for (int i = 0; i < (int)contourPoints.outer().size(); ++i)
		percentages.push_back(0);
	SetNameVisible(false);
	
	useBullsEye = true;
	showBullsEye = true;
	if (useBullsEye)
	{
		CreateBullsEye();
		for (int i = 0; i < contourPoints.outer().size(); i++)
		{
			OnCircles.push_back(1);
		}
	}
	updateSubTriangles();
}

CompletePolygon::CompletePolygon(int64_t _Id, bpt _center, int pointsCount, float radius,
	Colour _fillColour, float _canvasRatio) :
	EditablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
	showCursor = false;
	pc = 0;
	cursorCenter = contourPoints.outer().at(0);
	cursor = std::shared_ptr<EditableEllipse>(new EditableEllipse(0, cursorCenter, 0.1f, 0.1f, Colours::grey, _canvasRatio));
	cursor->SetNameVisible(false);
	for (int i = 0; i < (int)contourPoints.outer().size(); ++i)
	{
		percentages.push_back(0);
		anglesPercentages.push_back(0);
	}
	percentages.reserve(contourPoints.outer().size());
	SetNameVisible(false);
	
	useBullsEye = true;
	showBullsEye = true;
	startRadius = radius;
	interval = 0.05;
	if (useBullsEye)
	{
		CreateBullsEye();
		for (int i = 0; i < contourPoints.outer().size(); i++)
		{
			OnCircles.push_back(1);
		}
	}
}

CompletePolygon::CompletePolygon(int64_t _Id,
	bpt _center, bpolygon& _contourPoints, Colour _fillColour) :
	EditablePolygon(_Id, _center, _contourPoints, _fillColour)
{
	showCursor = false;
	pc = 0;
	cursorCenter = contourPoints.outer().at(0);
	cursor = std::shared_ptr<EditableEllipse>(new EditableEllipse(0, cursorCenter, 0.1f, 0.1f, Colours::grey, 1.47f));
	cursor->SetNameVisible(false);
	for (int i = 0; i < (int)contourPoints.outer().size(); ++i)
	{
		percentages.push_back(0);
		anglesPercentages.push_back(0);
	}
	percentages.reserve(contourPoints.outer().size());
	SetNameVisible(false);
	

	startRadius = 0.30;
	interval = 0.05;
	useBullsEye = true;
	showBullsEye = true;
	if (useBullsEye)
	{
		CreateBullsEye();
		for (int i = 0; i < contourPoints.outer().size(); i++)
		{
			OnCircles.push_back(1);
		}
	}

	updateSubTriangles();
}



CompletePolygon::~CompletePolygon()
{
	
}

void CompletePolygon::Paint(Graphics& g)
{
	//DBG("CompletePolygon::Paint");
	EditablePolygon::Paint(g);
	g.setColour(Colours::white);
	//DBG((String)contourPoints[0].getX());
	//Point<double> diff = contourPointsInPixels[fromPt] - contourPointsInPixels[fromPt + 1];
	//double dist = contourPointsInPixels[fromPt].getDistanceFrom(contourPointsInPixels[fromPt + 1]);
	//double angle = atan(diff.getY() / diff.getX());
	//point.addXY(dist * 24 / speed*cos(angle), dist * 24 / speed*sin(angle));
	//Point<double> diff =  contourPointsInPixels[fromPt] - contourPointsInPixels[fromPt + 1];
	//Point<double> diff;
	//DBG((String)(contourPointsInPixels[1] - contourPointsInPixels[0]).getX());
	//DBG((String)contourPointsInPixels[1].getAngleToPoint(contourPointsInPixels[0]));
	//diff.setX(contourPointsInPixels[fromPt].getX() - contourPointsInPixels[fromPt + 1].getX());
	//g.fillEllipse(point.getX(), point.getY(), 20, 20);
	//g.fillEllipse(contourPointsInPixels[0].getX() - contourPointsRadius,
	//	contourPointsInPixels[0].getY() - contourPointsRadius, 20, 20);

	//DBG("showCursorPaint = " + (String)showCursor);
	if (showCursor)
	{
		//DBG("paint cursor");
		cursor->Paint(g);
	}
	if (isActive && showBullsEye)
	{
		CanvasResizedBullsEye(parentCanvas);
		PaintBullsEye(g);
	}
}

void CompletePolygon::lengthToPercent()
{
	//calcul du perimetre
	perimeter = 0;
	/*for (int i = 0; i < bcontourPoints.outer().size()) - 1; ++i)
	{
		
		perimeter += contourPoints[i + 1].getDistanceFrom(contourPoints[i]);
		percentages[i + 1] = perimeter;
	}
	perimeter += contourPoints[contourPoints.size() - 1].getDistanceFrom(contourPoints[0]);*/
	perimeter = boost::geometry::perimeter(contourPointsInPixels);

	//calcul des poucentages correspondant à chaque point
	percentages[0] = 0;
	for (int i = 1; i < (int)contourPoints.outer().size(); ++i)
	{
		percentages[i] = percentages[i-1] + (boost::geometry::distance(contourPointsInPixels.outer().at(i),contourPointsInPixels.outer().at(i-1)))/ perimeter;
	}
}

void CompletePolygon::angleToPercent()
{
	for (int i = 0; i < (int)contourPointsInPixels.outer().size(); ++i)
	{
		boost::geometry::model::point<long double, 3, boost::geometry::cs::cartesian> pt3D(contourPointsInPixels.outer().at(i).get<0>()-centerInPixels.get<0>(), contourPointsInPixels.outer().at(i).get<1>() - centerInPixels.get<1>());
		boost::geometry::model::point<double, 3, boost::geometry::cs::spherical<boost::geometry::radian>> ptRad;
		boost::geometry::transform(pt3D, ptRad);
		if(ptRad.get<0>() >= 0)
			anglesPercentages[i] = ptRad.get<0>() / (2 * M_PI);
		else
			anglesPercentages[i] = 1 + (ptRad.get<0>() / (2 * M_PI));
	}
}

void CompletePolygon::setReadingPosition(double p)
{
	//DBG("position : " + (String)p);
	if (p >= 1)
		p -= 1;
	//DBG("position after: " + (String)p);
	if (showCursor)
	{
		//DBG("tailles setReadingPosition : bcontourPointsInPixels " + (String)bcontourPointsInPixels.outer().size());
		pc = p;
		lengthToPercent();
		//determiner entre quels points va se trouver le curseur
		int prev = 0;
		int suiv = 0;
		//DBG("percentages.size()" + (String)(percentages.size()));
		for (int i = 0; i < (int)percentages.size(); ++i)
		{
			//DBG((String)i);
			if (p < percentages[i])
			{
				prev = i - 1;
				suiv = i;
				//DBG("!!! " + (String)p + " < " + (String)percentages[i]);
				//DBG("stop");
				break;
			}
			else
			{
				prev = i;
				suiv = (i + 1) % percentages.size();
				//DBG((String)p + " > " + (String)percentages[i]);
			}
		}

		//DBG("prev = " + (String)prev);
		//DBG("suiv = " + (String)suiv);
		//calculer la position du curseur entre ces points
		bpt P;
		if (suiv != 0)
		{
			P.set<0>(contourPoints.outer().at(prev).get<0>() + ((p - percentages[prev]) / (percentages[suiv] - percentages[prev])) * (contourPoints.outer().at(suiv).get<0>() - contourPoints.outer().at(prev).get<0>()));
			P.set<1>(contourPoints.outer().at(prev).get<1>() + ((p - percentages[prev]) / (percentages[suiv] - percentages[prev])) * (contourPoints.outer().at(suiv).get<1>() - contourPoints.outer().at(prev).get<1>()));
		}
		else
		{
			P.set<0>(contourPoints.outer().at(prev).get<0>() + ((p - percentages[prev]) / (1 - percentages[prev])) * (contourPoints.outer().at(suiv).get<0>() - contourPoints.outer().at(prev).get<0>()));
			P.set<1>(contourPoints.outer().at(prev).get<1>() + ((p - percentages[prev]) / (1 - percentages[prev])) * (contourPoints.outer().at(suiv).get<1>() - contourPoints.outer().at(prev).get<1>()));
		}

		//placer le curseur à ce point
		bpt translation(P.get<0>() - cursorCenter.get<0>(), P.get<1>() - cursorCenter.get<1>());
		
		translation.set<0>(translation.get<0>() * (double)parentCanvas->getWidth());
		translation.set<1>(translation.get<1>() * (double)parentCanvas->getHeight());


		
		cursor->Translate(juce::Point<double>(translation.get<0>(), translation.get<1>()));
		cursorCenter = P;
		//}
		//redessiner
		cursor->CanvasResized(this->parentCanvas);
	}
	
}

void CompletePolygon::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
	//DBG("tailles setReadingPosition : bcontourPointsInPixels " + (String)bcontourPointsInPixels.outer().size());
	EditablePolygon::CanvasResized(_parentCanvas);
	//InteractivePolygon::CanvasResized(_parentCanvas);
	
	//cursor->CanvasResized(_parentCanvas);
	//lengthToPercent();
	if(showCursor)
		cursor->CanvasResized(_parentCanvas);
	if (isActive && showBullsEye)
		CanvasResizedBullsEye(_parentCanvas);
	setReadingPosition(pc);
	//cursor->CanvasResized(_parentCanvas);
	//DBG("cursorCenter : " + (String)(cursorCenter.x) + " " + (String)(cursorCenter.y));
	//DBG("CompletePolygon::CanvasResized");
	pointDraggingRadius = 0.05f * (parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0f; // 5% => mieux pour le touch
}

AreaEventType CompletePolygon::TryBeginPointMove(const Point<double>& newLocation)
{
	AreaEventType areaEventType = EditablePolygon::TryBeginPointMove(newLocation);
	for (int i = 0; i < Nradius; ++i)
		bullsEye[i].TryBeginPointMove(newLocation);
	return areaEventType;
}

AreaEventType CompletePolygon::TryMovePoint(const Point<double>& newLocation)
{
	DBG((String)newLocation.x + "   " + (String)newLocation.y);
	DBG("radius 0  = " + (String)radius[0]);
	DBG("radius 1  = " + (String)radius[1]);
	double r1 = boost::geometry::distance(centerInPixels, bmanipulationPointInPixels);
	AreaEventType areaEventType = EditablePolygon::TryMovePoint(newLocation);
	double r2 = boost::geometry::distance(centerInPixels, bmanipulationPointInPixels);
	double size = r2 / r1;

	if (!boost::geometry::equals(center, bullsEyeCenter)) // at the creation, they're equal
	{
		bpt translation(center.get<0>() - bullsEyeCenter.get<0>(), center.get<1>() - bullsEyeCenter.get<1>());
		
		translation.set<0>(translation.get<0>() * (double)parentCanvas->getWidth());
		translation.set<1>(translation.get<1>() * (double)parentCanvas->getHeight());
		

		for (int i = 0; i < Nradius; ++i)
			bullsEye[i].Translate(juce::Point<double>(translation.get<0>(), translation.get<1>()));
		
		bullsEyeCenter = center;
	}

	if (areaEventType == AreaEventType::RotScale)
	{
		double newStartRadius = startRadius * size;
		for (int i = 0; i < Nradius; ++i)
		{

			double newRadius = newStartRadius + i* interval;
			double resize = newRadius / radius[i];
			if (bullsEye[i].SizeChanged(resize))
			{
				startRadius = newStartRadius;
				radius[i] = newRadius; //startRadius + i*interval;
			}
		}
	}
	//CanvasResized(this->parentCanvas);
	return areaEventType;
}

AreaEventType CompletePolygon::EndPointMove()
{
	DBG("EndPointMove");

	if (useBullsEye)
	{

		if (pointDraggedId >= 0)
		{
			int nearest = 0;
			double epsilon = 1000000;
			double Di = boost::geometry::distance(contourPointsInPixels.outer().at(pointDraggedId), centerInPixels);
			double Df;
			for (int i = 0; i < Nradius; ++i)
			{
				if (epsilon > abs(bullsEye[i].getRadius() - Di))
				{
					epsilon = abs(bullsEye[i].getRadius() - Di);
					Df = radius[i];
					nearest = i;
					//circlesToShow[i] = true;
				}
				
			}
			
			circlesToShow[nearest] = true;

			
			boost::geometry::subtract_point(contourPointsInPixels.outer().at(pointDraggedId), centerInPixels);
			boost::geometry::model::point<long double, 3, boost::geometry::cs::cartesian> pt3D(contourPointsInPixels.outer().at(pointDraggedId).get<0>(), contourPointsInPixels.outer().at(pointDraggedId).get<1>());
			boost::geometry::model::point<double, 3, boost::geometry::cs::spherical<boost::geometry::radian>> ptRad;
			boost::geometry::transform(pt3D, ptRad);
			double angle = ptRad.get<0>();//M_PI;//M_PI/2;//ptRad.get<0>(); // --> cos = 1, sin = 0, contourPointsInPixels.outer().at(pointDraggedId) = bpt(centerInPixels.get<0>() + radius[i] * parent.getWidth(), 0)

			double R = bullsEye[nearest].getRadius();//54;//bullsEye[nearest].getRadius();//radius[nearest];
			contourPointsInPixels.outer().at(pointDraggedId) = bpt(centerInPixels.get<0>() + R * std::cos(angle),
					centerInPixels.get<1>() + R * std::sin(angle));
			contourPoints.outer().at(pointDraggedId) = bpt(
					contourPointsInPixels.outer().at(pointDraggedId).get<0>() / parentCanvas->getWidth(),
					contourPointsInPixels.outer().at(pointDraggedId).get<1>() / parentCanvas->getHeight()
			);

				DBG("difference = " + (String)boost::geometry::distance(contourPointsInPixels.outer().at(pointDraggedId), contourPointsInPixels.outer().at(2))); // = 53,4 = 267*0.2 -> -cos(angle)* R *Width
			
			

			if (pointDraggedId == 0) // first point = last point
			{
				contourPointsInPixels.outer().at(contourPointsInPixels.outer().size() - 1) = contourPointsInPixels.outer().at(0);
				contourPoints.outer().at(contourPoints.outer().size() - 1) = contourPoints.outer().at(0);
			}
			OnCircles.at(pointDraggedId) = nearest + 1; // +1 pcq notes midi commencent a 1;
			
		}
		CanvasResized(parentCanvas);
	}


	AreaEventType eventType =  EditablePolygon::EndPointMove();
	DBG("EndPointMove : " + (String)(int)eventType);
	
	return eventType;
}

void CompletePolygon::setCursorVisible(bool isVisible, SceneCanvasComponent* _parentCanvas)
{
	cursor->CanvasResized(_parentCanvas);
	showCursor = isVisible;
	
	//CanvasResized(this->parentCanvas);

	//DBG("showCursor = " + (String)showCursor);
	//setReadingPosition(0);
}

bpolygon CompletePolygon::getPolygon()
{
	return contourPoints;
}

bool CompletePolygon::intersection(bpolygon hitPolygon, bpt &I)
{
	std::vector<bpt> inter;
	boost::geometry::intersection(contourPoints, hitPolygon, inter);

	bpolygon test;
	for(int i = 0; i<inter.size();i++)
		boost::geometry::append(test,inter[i]);
	
	double areaTest = abs(boost::geometry::area(test));
	double hitArea = abs(boost::geometry::area(hitPolygon));
	double area = abs(boost::geometry::area(contourPoints));
	if (areaTest >= 0.75 * hitArea || areaTest >= 0.75 * area)
		DBG("need To fusion !");

	if (inter.size() > 0)
	{
		int idx = 0;
		double minAngle = 2 * M_PI;
		for (int i = 0; i < inter.size(); i++)
		{
			bpt currentPt = inter[i];
			boost::geometry::subtract_point(currentPt, center);
			double angle = Math::ComputePositiveAngle(currentPt);
			if (angle < minAngle)
			{
				minAngle = angle;
				idx = i;
			}
		}

		I = inter[idx];
		return true;
	}
	else
		return false;
}

std::shared_ptr<CompletePolygon> CompletePolygon::fusion(std::shared_ptr<CompletePolygon> polyToFusion)
{
	// structure to be able to sort the percentages and the circles the same way
	struct Helper
	{
		double pc;
		int circ;
		Helper(double a, int b) : pc(a), circ(b) {};
		bool operator< (Helper b) { return (this->pc < b.pc); }
	};

	// get all the coordinates of the points (percentages and circles) of the current polygon
	std::vector<Helper> test;
	for (int j = 0; j < anglesPercentages.size(); ++j)
		test.push_back(Helper(anglesPercentages[j], OnCircles[j]));

	// add all the coordinates of the other polygon to the vector of coordinates
	int i = 0;
	double value;
	int distance;
	while (polyToFusion->getAllPercentages(i, value) && polyToFusion->getAllDistanceFromCenter(i,distance))
	{
		test.push_back(Helper(value, distance));
		++i;
	}

	// sort to have all the points in clockwise order
	std::sort(test.begin(), test.end());

	// get the XY(pixels) coordinates from the previously sorted coordinates
	bpolygon newContourPointsInPixels;
	for (int j = 0; j < test.size(); j++)
	{
		double R = bullsEye[test[j].circ].getRadius();
		double angle = test[j].pc * 2 * M_PI;
		bpt newPoint(R * std::cos(angle), R * std::sin(angle));
		boost::geometry::add_point(newPoint, centerInPixels);
		boost::geometry::append(newContourPointsInPixels.outer(),newPoint);
	}

	// get normalized coordinates
	bpolygon newContourPoints;
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> rescaler(1 / parentCanvas->getWidth(), 1 / parentCanvas->getHeight());
	boost::geometry::transform(newContourPointsInPixels, newContourPoints, rescaler);
	
	// create polygon
	return std::shared_ptr<CompletePolygon>(new CompletePolygon(Id,center,newContourPoints,fillColour));

}

bool CompletePolygon::getUnion(bpolygon hitPolygon, bpolygon &output)
{
	std::vector<bpolygon> result;
	boost::geometry::union_(contourPoints, hitPolygon, result);
	if (result.size() == 1)
	{
		output = result[1];
	}
	else if (result.size() == 0)
		return false;
	return true;
}


double CompletePolygon::getPercentage(bpt hitPoint)
{
	
	bpt GT(hitPoint.get<0>() - center.get<0>(), hitPoint.get<1>() - center.get<1>());
	double angle = Miam::Math::ComputePositiveAngle(GT);
	int i = 0;
	while (!subTriangles[i].ContainsAngle(angle))
		i++;

	int prev = 0;
	int suiv = 0;
	if (i == 0)
	{
		prev = contourPoints.outer().size() - 1;
		suiv = 0;
	}
	else
	{
		prev = i - 1;
		suiv = i;
	}
	return percentages[prev] + (boost::geometry::distance(hitPoint, contourPoints.outer().at(prev))) / perimeter;
	
	//return 0.0;
}

bool CompletePolygon::getAllPercentages(int idx, double &value)
{
	lengthToPercent();
	angleToPercent();
	//DBG("percentage size : " + (String)percentages.size());
	if (idx < percentages.size())
	{
		value = anglesPercentages[idx];
		return true;
	}
	else
		return false;
}

bool CompletePolygon::getAllDistanceFromCenter(int idx, int &value)
{
	if (idx < OnCircles.size())
	{
		value = OnCircles[idx];
		return true;
	}
	else
		return false;
}

void CompletePolygon::CreateBullsEye()
{
	bullsEyeCenter = center;
	for (int i = 0; i < Nradius; ++i)
	{
		radius[i] = startRadius + i*interval;//(i + 1)*0.15f / 2;
		bullsEye.push_back( EditableEllipse(0, center, 2*radius[i], 2*radius[i], Colours::grey, 1.47f));
		bullsEye.back().SetAlpha(0.0);
	}
	circlesToShow[0] = true;
	for (int i = 1; i < Nradius; ++i)
	{
		circlesToShow[i] = false;
	}
}

void CompletePolygon::PaintBullsEye(Graphics& g)
{
	for (int i = 0; i < OnCircles.size(); ++i)
	{
		bullsEye[OnCircles[i]-1].Paint(g);
			//bullsEye[i].Paint(g);
	}
}

void CompletePolygon::CanvasResizedBullsEye(SceneCanvasComponent* _parentCanvas)
{
	//if (!boost::geometry::equals(center,bullsEyeCenter)) // at the creation, they're equal
	//{
	//	bpt translation(center.get<0>() - bullsEyeCenter.get<0>(), center.get<1>() - bullsEyeCenter.get<1>());
	//	DBG("center = (" + (String)center.get<0>() + " , " + (String)center.get<1>() + ")");
	//	DBG("centerInPixels = (" + (String)centerInPixels.get<0>() + " , " + (String)centerInPixels.get<1>() + ")");
	//	DBG("bullsEyeCenter = (" + (String)bullsEyeCenter.get<0>() + " , " + (String)bullsEyeCenter.get<1>() + ")");
	//	DBG("translation = (" + (String)translation.get<0>() + " , " + (String)translation.get<1>() + ")");
	//	translation.set<0>(translation.get<0>() * (double)parentCanvas->getWidth());
	//	translation.set<1>(translation.get<1>() * (double)parentCanvas->getHeight());
	//	//DBG("parentSize = (" + (String)parentCanvas->getWidth() + " , " + (String)parentCanvas->getHeight() + ")");
	//	//DBG("translation resized = (" + (String)translation.get<0>() +" , " + (String)translation.get<1>() +")");
	//	for (int i = 0; i < Nradius; ++i)
	//	{
	//		bullsEye[i]->Translate(juce::Point<double>(translation.get<0>(), translation.get<1>()));
	//	}
	//	bullsEyeCenter = center;
	//	DBG(" verif center = (" + (String)center.get<0>() + " , " + (String)center.get<1>() + ")");
	//	DBG(" verifcenterInPixels = (" + (String)centerInPixels.get<0>() + " , " + (String)centerInPixels.get<1>() + ")");
	//	DBG(" verifbullsEyeCenter = (" + (String)bullsEyeCenter.get<0>() + " , " + (String)bullsEyeCenter.get<1>() + ")");
	//	
	//}

	for (int i = 0; i < Nradius; ++i)
	{
		bullsEye[i].CanvasResized(_parentCanvas);
	}

	
}




