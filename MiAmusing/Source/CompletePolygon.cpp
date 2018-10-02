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
#include <algorithm>

#include <list>
#include <vector>
#include <string>
#include <sstream>

#include "MultiAreaEvent.h"


#include "boost/geometry.hpp"
#include "boost/geometry/algorithms/transform.hpp"

using namespace Amusing;
using namespace Miam;

CompletePolygon::CompletePolygon(bptree::ptree & areaTree) : EditablePolygon(areaTree)
{
	isFilled = true;

	bptree::ptree completeParameterTree;

	completeParameterTree = areaTree.get_child("completeParameter");
	orientationAngle = completeParameterTree.get<double>("<xmlattr>.orientation");
	fillOpacity = completeParameterTree.get<float>("<xmlattr>.opacity");
	startRadius = completeParameterTree.get<double>("<xmlattr>.radius");
	interval = completeParameterTree.get<double>("<xmlattr>.interval");
	

	multiTouchActionBegun = false;
	currentTouchRotation = 0.0;

	showCursor = true;
	pc = 0;

	useBullsEye = true;
	showBullsEye = true;

	for (int i = 0; i < (int)contourPoints.outer().size(); ++i)
	{
		percentages.push_back(0);
		anglesPercentages.push_back(0);
	}

	SetNameVisible(false);

	CreateBullsEye();
	for (int i = 0; i < (int)contourPoints.outer().size(); i++)
	{
		OnCircles.push_back(0);
	}

	numAngles = 48;

	updateSubTriangles();

	onlyRotationAllowed = false;
	areaIsVisible = true;
	showAllCircles = false;

	//verticesBufferSize += 3 * Nradius * bullsEye[0].GetVerticesBufferSize();
	//indicesBufferSize += Nradius * bullsEye[0].GetIndicesBufferSize();
	//couloursBufferSize += Nradius * bullsEye[0].GetCouloursBufferSize();

	vertices_buffer.resize(verticesBufferSize);
	indices_buffer.resize(indicesBufferSize);
	coulours_buffer.resize(couloursBufferSize);

	previousSizeToShow = 5;
	deleteOldCircles = false;
}

CompletePolygon::CompletePolygon(int64_t _Id) : EditablePolygon(_Id)
{

	/*std::string s;
	std::ostringstream ost;
	ost << this;
	s = ost.str();
	DBG("Constructor : " + s);*/

	centerCircleRadius *= 2;
	centerContourWidth *= 2;
	multiTouchActionBegun = false;
	currentTouchRotation = 0.0;

	showCursor = false;
	pc = 0;
	orientationAngle = 0.0;
	
	//AddCursor();
	//percentages.reserve(contourPoints.size());
	for (int i = 0; i < (int)contourPoints.outer().size(); ++i)
		percentages.push_back(0);
	SetNameVisible(false);
	
	useBullsEye = true;
	showBullsEye = true;
	if (useBullsEye)
	{
		CreateBullsEye();
		for (int i = 0; i < (int)contourPoints.outer().size(); i++)
		{
			OnCircles.push_back(0);
		}
	}
	updateSubTriangles();
	onlyRotationAllowed = false;
	previousSizeToShow = 5;
}

CompletePolygon::CompletePolygon(int64_t _Id, bpt _center, int pointsCount, float radius,
	Colour _fillColour, float _canvasRatio) :
	EditablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
	/*std::string s;
	std::ostringstream ost;
	ost << this;
	s = ost.str();
	DBG("Constructor : " + s);*/
	isFilled = true;
	numAngles = 48;

	centerCircleRadius *= 2;
	centerContourWidth *= 2;
	multiTouchActionBegun = false;
	currentTouchRotation = 0.0;

	showCursor = false;
	pc = 0;
	
	//AddCursor(_canvasRatio);
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
		for (int i = 0; i < (int)contourPoints.outer().size(); i++)
		{
			OnCircles.push_back(0);
		}
	}
	chordFlag = std::vector<bool>(pointsCount, false);
	chordAreaForFlag = std::vector<std::shared_ptr<CompletePolygon>>(pointsCount, nullptr);
	showAllCircles = false;
	onlyRotationAllowed = false;

	//verticesBufferSize += 3 * Nradius * bullsEye[0].GetVerticesBufferSize();
	//indicesBufferSize += Nradius * bullsEye[0].GetIndicesBufferSize();
	//couloursBufferSize += Nradius * bullsEye[0].GetCouloursBufferSize();

	vertices_buffer.resize(verticesBufferSize);
	indices_buffer.resize(indicesBufferSize);
	coulours_buffer.resize(couloursBufferSize);

	previousSizeToShow = 5;
	deleteOldCircles = false;
}

CompletePolygon::CompletePolygon(int64_t _Id,
	bpt _center, bpolygon& _contourPoints, Colour _fillColour) :
	EditablePolygon(_Id, _center, _contourPoints, _fillColour)
{
	/*std::string s;
	std::ostringstream ost;
	ost << this;
	s = ost.str();
	DBG("Constructor : " + s);*/
	centerCircleRadius *= 2;
	centerContourWidth *= 2;
	multiTouchActionBegun = false;
	currentTouchRotation = 0.0;
	numAngles = 48;

	showCursor = true;
	pc = 0;
	
	//AddCursor();
	for (int i = 0; i < (int)contourPoints.outer().size(); ++i)
	{
		percentages.push_back(0);
		anglesPercentages.push_back(0);
	}
	percentages.reserve(contourPoints.outer().size());
	SetNameVisible(false);
	

	startRadius = 0.15;
	interval = 0.05;
	useBullsEye = true;
	showBullsEye = true;
	if (useBullsEye)
	{
		CreateBullsEye();
		for (int i = 0; i < (int)contourPoints.outer().size(); i++)
		{
			OnCircles.push_back(0);
		}
	}

	//updateSubTriangles();
	showAllCircles = false;
	onlyRotationAllowed = false;

	//verticesBufferSize += 3 * Nradius * bullsEye[0].GetVerticesBufferSize();
	//indicesBufferSize += Nradius * bullsEye[0].GetIndicesBufferSize();
	//couloursBufferSize += Nradius * bullsEye[0].GetCouloursBufferSize();

	vertices_buffer.resize(verticesBufferSize);
	indices_buffer.resize(indicesBufferSize);
	coulours_buffer.resize(couloursBufferSize);

	previousSizeToShow = 5;
	deleteOldCircles = false;
}

CompletePolygon::CompletePolygon(int64_t _Id,
	bpt _center, bpolygon& _contourPoints,
	std::vector<int> circles, std::vector<double> percent,
	Colour _fillColour) : 
	EditablePolygon(_Id, _center, _contourPoints, _fillColour)
{
	numAngles = 48;
	centerCircleRadius *= 2;
	centerContourWidth *= 2;
	multiTouchActionBegun = false;
	currentTouchRotation = 0.0;
	/*std::string s;
	std::ostringstrea
	m ost;
	ost << this;
	s = ost.str();
	DBG("Constructor : " + s);*/

	showCursor = true;
	pc = 0;
	
	//AddCursor();

	/*for (int i = 0; i < (int)contourPoints.outer().size(); ++i)
	{
		percentages.push_back(0);
		anglesPercentages.push_back(0);
	}
	percentages.reserve(contourPoints.outer().size());*/
	interval = 0.05;

	OnCircles = circles;
	anglesPercentages = percent;
	SetNameVisible(false);

	bpt A = contourPoints.outer().at(0);
	float _xScale = 1.0f / 1.47f;
	
	double midRadius = (A.get<0>() - center.get<0>()) / (_xScale * cos(anglesPercentages[0] * 2 * M_PI));
	startRadius = midRadius - OnCircles[0] * interval;
	
	//startRadius = 0.15;
	
	useBullsEye = true;
	showBullsEye = true;
	if (useBullsEye)
	{
		CreateBullsEye();
		
	}

	onlyRotationAllowed = false;
	areaIsVisible = true;
	// ajouter le calcul du rayon des cercles : on connait les coordonnees des points et a quels cercles ils appartienne -> possible de retrouver le rayon et le centre ! 

	//verticesBufferSize += 3 * Nradius * bullsEye[0].GetVerticesBufferSize();
	//indicesBufferSize += Nradius * bullsEye[0].GetIndicesBufferSize();
	//couloursBufferSize += Nradius * bullsEye[0].GetCouloursBufferSize();

	vertices_buffer.resize(verticesBufferSize);
	indices_buffer.resize(indicesBufferSize);
	coulours_buffer.resize(couloursBufferSize);

	previousSizeToShow = 5;
	deleteOldCircles = false;
}



CompletePolygon::~CompletePolygon()
{
	//DBG("CompletePolygon destructor");
	/*for (int i = 0; i < cursors.size(); i++)
		cursors[i]->LinkTo(nullptr);
	cursors.clear();*/
}

void CompletePolygon::Copy(std::shared_ptr<CompletePolygon> polygonToCopy)
{
	contourPoints.clear();
	contourPoints = polygonToCopy->getPolygon();
	
	OnCircles.clear();
	anglesPercentages.clear();
	int i = 0;
	int value = 0;
	double newPercentage = 0.0;
	while (polygonToCopy->getAllDistanceFromCenter(i, value) && polygonToCopy->getAllPercentages(i,newPercentage))
	{
		OnCircles.push_back(value);
		anglesPercentages.push_back(newPercentage);
		i++;
	}
	boost::geometry::centroid(contourPoints,center);
	// voir s'il faut recalculer le centre
	CanvasResized(parentCanvas);
	RefreshOpenGLBuffers();
	
}

//void CompletePolygon::AddCursor()
//{
//	cursorCenter = contourPoints.outer().at(0);
//	initCursorSize = 0.1f;
//	cursorSize = initCursorSize;
//	cursor = std::shared_ptr<Cursor>(new Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, 1.47f));
//	cursor->SetNameVisible(false);
//}
//
//void CompletePolygon::AddCursor(float _canvasRatio)
//{
//	cursorCenter = contourPoints.outer().at(0);
//	initCursorSize = 0.1f;
//	cursorSize = initCursorSize;
//	cursor = std::shared_ptr<Cursor>(new Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, _canvasRatio));
//	cursor->SetNameVisible(false);
//}
//
//void CompletePolygon::AddCursor(float _canvasRation, double p)
//{
//
//}

void CompletePolygon::linkTo(std::shared_ptr<Cursor> cursor)
{
	cursors.push_back(cursor);
	cursor->setSpeed(1);
}

int CompletePolygon::getCursorsCount()
{
	return (int)cursors.size();
}

std::shared_ptr<Cursor> CompletePolygon::getCursor(int idx)
{
	return cursors[idx];
}

void CompletePolygon::Paint(Graphics& g)
{
	if (areaVisible)
	{
		if (isActive && showBullsEye)
		{
			CanvasResizedBullsEye(parentCanvas);
			PaintBullsEye(g);
		}
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
			for (int i = 0; i < (int)cursors.size(); i++)
				cursors[i]->Paint(g);
		}
	}
}

void CompletePolygon::lengthToPercent()
{
	//calcul du perimetre
	perimeter = 0;
	perimeter = boost::geometry::perimeter(contourPointsInPixels);

	//calcul des poucentages correspondant � chaque point
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
		while (anglesPercentages[i] < anglesPercentages[0])
			anglesPercentages[i] += 1;

		if (i == (int)contourPointsInPixels.outer().size() - 1)
			anglesPercentages[i] = 1 + anglesPercentages[0];
	}
}

boost::geometry::model::segment<bpt> CompletePolygon::getSegment(bpt hitPoint) // rien en pixels
{
	// d'abord verifier si le point fait partie des points du contour
	int prev = 0;
	int suiv = 0;
	for (int i = 0; i < (int)contourPoints.outer().size(); i++)
	{
		if (boost::geometry::equals(hitPoint, contourPoints.outer().at(i)))
		{
			prev = i;
			suiv = i + 1;
			if (suiv == (int)contourPoints.outer().size())
				suiv = 1; // pas 0 car le point 0 et le dernier point sont les m�mes
			return boost::geometry::model::segment<bpt>(contourPoints.outer().at(prev), contourPoints.outer().at(suiv));
		}
	}

	// s'il ne fait pas partie des points du contour, chercher quand quel subTriangle se trouve le point
	bpt GT(hitPoint.get<0>() - center.get<0>(), hitPoint.get<1>() - center.get<1>());
	double angle = Miam::Math::ComputePositiveAngle(GT);
	int i = 0;
	while (!subTriangles[i].ContainsAngle(angle))
		i++;

	
	if (i == 0)
	{
		prev = (int)contourPoints.outer().size() - 1;
		suiv = 0;
	}
	else
	{
		prev = i - 1;
		suiv = i;
	}

	return boost::geometry::model::segment<bpt>(contourPoints.outer().at(prev), contourPoints.outer().at(suiv));
}

boost::geometry::model::segment<bpt> CompletePolygon::getSegmentInPixels(bpt hitPointInPixels) // tout en pixels
{
	// d'abord verifier si le point fait partie des points du contour
	int prev = 0;
	int suiv = 0;
	for (int i = 0; i < (int)contourPointsInPixels.outer().size(); i++)
	{
		if (boost::geometry::equals(hitPointInPixels, contourPointsInPixels.outer().at(i)))
		{
			prev = i;
			suiv = i + 1;
			if (suiv == (int)contourPointsInPixels.outer().size())
				suiv = 1; // pas 0 car le point 0 et le dernier point sont les m�mes
			return boost::geometry::model::segment<bpt>(contourPointsInPixels.outer().at(prev), contourPointsInPixels.outer().at(suiv));
		}
	}

	// s'il ne fait pas partie des points du contour, chercher quand quel subTriangle se trouve le point
	bpt GT(hitPointInPixels.get<0>() - centerInPixels.get<0>(), hitPointInPixels.get<1>() - centerInPixels.get<1>());
	double angle = Miam::Math::ComputePositiveAngle(GT);
	int i = 0;
	while (!subTriangles[i].ContainsAngle(angle))
		i++;


	if (i == 0)
	{
		prev = (int)contourPointsInPixels.outer().size() - 1;
		suiv = 0;
	}
	else
	{
		prev = i - 1;
		suiv = i;
	}

	return boost::geometry::model::segment<bpt>(contourPointsInPixels.outer().at(prev), contourPointsInPixels.outer().at(suiv));
}


bpt CompletePolygon::computeLinearCursorCenter(double p)
{
	while (p > 1)
		p -= 1;
	
	if (showCursor)
	{
		int prev = 0;
		int suiv = 0;
		if (useBullsEye)
		{
			pc = p;
			// conversion percent to radian
			angleToPercent();

			//determiner entre quels points va se trouver le curseur
			
			for (int i = 0; i < (int)anglesPercentages.size(); ++i)
			{
				//DBG((String)i);
				if (p < anglesPercentages[i])
				{
					if (i == 0)
					{
						//prev = anglesPercentages.size() - 2; // -2 pour ne pas prendre le point de fermeture
						//suiv = anglesPercentages.size() - 1; // point de fermeture, idem premier point
						p = 1 + p;
					}
					else
					{
						prev = i - 1;
						suiv = i;
						break;
					}
					//DBG("!!! " + (String)p + " < " + (String)percentages[i]);
					//DBG("stop");
					
				}
				else
				{
					prev = i;
					suiv = (i + 1) % anglesPercentages.size();
					//DBG((String)p + " > " + (String)percentages[i]);
				}
			}
			//DBG((String)prev + " " + (String)suiv);
			if (suiv == 0)
				suiv += 1;

			// calcul du point o� se trouve le curseur par interpolation lin�aire
			bpt P;
			if (suiv != 0)
			{
				P.set<0>(contourPoints.outer().at(prev).get<0>() + ((p - anglesPercentages[prev]) / (anglesPercentages[suiv] - anglesPercentages[prev])) * (contourPoints.outer().at(suiv).get<0>() - contourPoints.outer().at(prev).get<0>()));
				P.set<1>(contourPoints.outer().at(prev).get<1>() + ((p - anglesPercentages[prev]) / (anglesPercentages[suiv] - anglesPercentages[prev])) * (contourPoints.outer().at(suiv).get<1>() - contourPoints.outer().at(prev).get<1>()));
			}
			else
			{
				if (anglesPercentages[prev] == 1)
				{
					P.set<0>(contourPoints.outer().at(prev).get<0>());
					P.set<1>(contourPoints.outer().at(prev).get<1>());
				}
				else
				{
					P.set<0>(contourPoints.outer().at(prev).get<0>() + ((p - anglesPercentages[prev]) / (1 - anglesPercentages[prev])) * (contourPoints.outer().at(suiv).get<0>() - contourPoints.outer().at(prev).get<0>()));
					P.set<1>(contourPoints.outer().at(prev).get<1>() + ((p - anglesPercentages[prev]) / (1 - anglesPercentages[prev])) * (contourPoints.outer().at(suiv).get<1>() - contourPoints.outer().at(prev).get<1>()));
				}
			}
			//if (!boost::geometry::within(P, boost::geometry::model::segment<bpt>(contourPoints.outer().at(prev), contourPoints.outer().at(suiv))))
			//	DBG("outside the segment !");
			return P;

		}
		else
		{
			prev = 0;
			suiv = 0;
			//DBG("tailles setReadingPosition : bcontourPointsInPixels " + (String)bcontourPointsInPixels.outer().size());
			pc = p;
			lengthToPercent();

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




			return P;

		}



	}
	return bpt(0, 0);
}

bpt CompletePolygon::computeAngularCursorCenter(double p)
{
	while (p > 1)
		p -= 1;

	if (showCursor)
	{
		int prev = 0;
		int suiv = 0;
		if (useBullsEye)
		{
			pc = p;
			// conversion percent to radian
			angleToPercent();

			//determiner entre quels points va se trouver le curseur

			for (int i = 0; i < (int)anglesPercentages.size(); ++i)
			{
				//DBG((String)i);
				if (p < anglesPercentages[i])
				{
					if (i == 0)
					{
						//prev = anglesPercentages.size() - 2; // -2 pour ne pas prendre le point de fermeture
						//suiv = anglesPercentages.size() - 1; // point de fermeture, idem premier point
						p = 1 + p;
					}
					else
					{
						prev = i - 1;
						suiv = i;
						break;
					}
					//DBG("!!! " + (String)p + " < " + (String)percentages[i]);
					//DBG("stop");

				}
				else
				{
					prev = i;
					suiv = (i + 1) % anglesPercentages.size();
					//DBG((String)p + " > " + (String)percentages[i]);
				}
			}
			//DBG((String)prev + " " + (String)suiv);
			if (suiv == 0)
				suiv += 1;

			// calcul du point o� se trouve le curseur par interpolation lin�aire
			bpt P(0,0);
			bpt extr;
			//if( (0 <= p && p<0.25 ) || (0.75 <= p && p<1))
			extr = bpt(center.get<0>() + 100 * cos(2 * M_PI*p), center.get<1>() + 100 * sin(2 * M_PI*p));
			//else
			//extr = bpt(center.get<0>() + 1, center.get<1>() - tan(2 * M_PI*p));
			boost::geometry::model::segment<bpt> seg(center, extr);
			boost::geometry::model::linestring<bpt> line;//(center, extr);
			boost::geometry::append(line, center);
			boost::geometry::append(line, extr);
			std::vector<bpt> newP;
			boost::geometry::intersection(line, getPolygon(), newP);
			if (newP.size() == 1)
				P = newP[0];
			
			//if (!boost::geometry::within(P, boost::geometry::model::segment<bpt>(contourPoints.outer().at(prev), contourPoints.outer().at(suiv))))
			//	DBG("outside the segment !");
			return P;

		}
		else
		{
			//DBG("tailles setReadingPosition : bcontourPointsInPixels " + (String)bcontourPointsInPixels.outer().size());
			pc = p;
			lengthToPercent();

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




			return P;

		}



	}
	return bpt(0, 0);
}

float CompletePolygon::computeCursorAlpha(double p, bpt _center,float _maxAlpha)
{
	// conversion percent to radian
	angleToPercent();

	//determiner entre quels points va se trouver le curseur
	int prev = 0;
	int suiv = 0;
	for (int i = 0; i < (int)anglesPercentages.size(); ++i)
	{
		//DBG((String)i);
		if (p < anglesPercentages[i])
		{
			if (i == 0)
			{
				//prev = anglesPercentages.size() - 2; // -2 pour ne pas prendre le point de fermeture
				//suiv = anglesPercentages.size() - 1; // point de fermeture, idem premier point
				p = 1 + p;
			}
			else
			{
				prev = i - 1;
				suiv = i;
				break;
			}
			//DBG("!!! " + (String)p + " < " + (String)percentages[i]);
			//DBG("stop");

		}
		else
		{
			prev = i;
			suiv = (i + 1) % anglesPercentages.size();
			//DBG((String)p + " > " + (String)percentages[i]);
		}
	}

	double distPr = boost::geometry::distance(_center, contourPoints.outer().at(prev));
	double distSui = boost::geometry::distance(_center, contourPoints.outer().at(suiv));
	double D = 0.02; // distance � laquelle on commence � augmenter/diminuer l'opacit�
	double H = (double)_maxAlpha; //0.5;  // opacit� lorsqu'on est pas assez proche d"un sommet
	if (distPr < D )
	{
		if (1 - distPr * (1-H)/D < 0)
			DBG("negatif");
		return 1.0f - float(distPr * (1.0 - H) / D);//cursor->SetAlpha(1 - distPr * 10);
		//DBG((String)(1 - distPr * 10));
	}
	else if (distSui < D)
	{
		if (1 - distSui * (1 - H) / D < 0)
			DBG("negatif");
		return 1.0f - float(distSui * (1.0 - H) / D);
	}
	else
		return _maxAlpha;
	

}



void CompletePolygon::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
	//DBG("tailles setReadingPosition : bcontourPointsInPixels " + (String)bcontourPointsInPixels.outer().size());
	EditablePolygon::CanvasResized(_parentCanvas);
	//InteractivePolygon::CanvasResized(_parentCanvas);
	
	//cursor->CanvasResized(_parentCanvas);
	//lengthToPercent();
	if (showCursor)
	{
		for(int i=0;i<(int)cursors.size();i++)
			cursors[i]->CanvasResized(_parentCanvas);
	}
		
	//if (isActive)
	CanvasResizedBullsEye(_parentCanvas);
	//setReadingPosition(pc);
	//cursor->CanvasResized(_parentCanvas);
	//DBG("cursorCenter : " + (String)(cursorCenter.x) + " " + (String)(cursorCenter.y));
	//DBG("CompletePolygon::CanvasResized");
	pointDraggingRadius = 0.05f * (parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0f; // 5% => mieux pour le touch
}

AreaEventType CompletePolygon::TryBeginMultiTouchAction(const Point<double>& newLocation)
{
	bpt bnewLocation(newLocation.x, newLocation.y);
	if (pointDraggedId == EditableAreaPointId::Center) // le centre est déjà selectionné -> commencer une rotation classique (idem manipulationPoint)
	{
		DBG("rotation classique");
		multiTouchActionBegun = true;

		// calcul de l'angle initial
		bpt testPt(bnewLocation);
		boost::geometry::subtract_point(testPt, centerInPixels);
		currentTouchRotation = Math::ComputePositiveAngle(testPt);
		DBG("begin : " + (String)currentTouchRotation);

		currentTouchSize = boost::geometry::distance(centerInPixels, bnewLocation);

		return AreaEventType::RotScale;
	}
	else if(pointDraggedId == EditableAreaPointId::WholeArea) // le point était simplement à l'intérieur de l'aire
	{
		// vérifier si le nouveau point est, lui, sur le centre
		multiTouchActionBegun = true;
		if (boost::geometry::distance(centerInPixels, bpt(newLocation.x, newLocation.y))
			< (centerCircleRadius + centerContourWidth))
		{
			// on est sur le centre -> commencer une rotation classique (idem manipulationPoint)
			DBG("rotation classique");
			return AreaEventType::RotScale;
		}
		else
		{
			// aucun des points n'est sur le centre -> prendre un des deux points comme manipulationPoint
			DBG("rotation spéciale");
			multiTouchActionBegun = true;

			// calcul de l'angle initial
			bpt testPt(bnewLocation);
			boost::geometry::subtract_point(testPt, centerInPixels);
			currentTouchRotation = Math::ComputePositiveAngle(testPt);
			currentTouchSize = boost::geometry::distance(centerInPixels, bnewLocation);

			return AreaEventType::RotScale;
		}
	}
	else
	{
		// pas d'action
		return AreaEventType::NothingHappened;
	}
}

AreaEventType CompletePolygon::TryMoveMultiTouchPoint(const Point<double>& newLocation)
{
	bpt bnewLocation(newLocation.x, newLocation.y);
	if (pointDraggedId == EditableAreaPointId::Center) // le centre est déjà selectionné -> commencer une rotation classique (idem manipulationPoint)
	{
		DBG("rotation classique");

		bpt testPt(bnewLocation);
		boost::geometry::subtract_point(testPt, centerInPixels);
		double radAngle = Math::ComputePositiveAngle(testPt);

		double newTouchSize = boost::geometry::distance(centerInPixels, bnewLocation);
		double size = newTouchSize / currentTouchSize;
		EditablePolygon::SizeChanged(size, true);
		currentTouchSize = newTouchSize;

		DBG("radAngle : " + String(currentTouchRotation - radAngle));
		Rotate(currentTouchRotation - radAngle);
		currentTouchRotation = radAngle;

		updateContourPoints();

		double newStartRadius = startRadius * size;
		for (int i = 0; i < Nradius; ++i)
		{

			double newRadius = radius[i] * size;//newStartRadius + i* interval;
			double resize = size;//newRadius / radius[i];
			/*if (bullsEye[i].SizeChanged(resize, false))
			{*/
				startRadius = newStartRadius;
				radius[i] = newRadius; //startRadius + i*interval;
				/*if (radius[i] > 0.05)
					bullsEye[i].setVerticesCount(64);
				else
					bullsEye[i].setVerticesCount(32);
				bullsEye[i].updateContourPoints();*/
			//}
		}


		CanvasResized(parentCanvas);
		RefreshOpenGLBuffers();

		return AreaEventType::RotScale;
	}
	else if (pointDraggedId == EditableAreaPointId::WholeArea) // le point était simplement à l'intérieur de l'aire
	{
		// vérifier si le nouveau point est, lui, sur le centre
		multiTouchActionBegun = true;
		if (boost::geometry::distance(centerInPixels, bpt(newLocation.x, newLocation.y))
			< (centerCircleRadius + centerContourWidth))
		{
			// on est sur le centre -> commencer une rotation classique (idem manipulationPoint)
			DBG("rotation classique");
			return AreaEventType::RotScale;
		}
		else
		{
			// aucun des points n'est sur le centre -> calculer les pentes...
			DBG("rotation spéciale");
			bpt testPt(bnewLocation);
			boost::geometry::subtract_point(testPt, centerInPixels);
			double radAngle = Math::ComputePositiveAngle(testPt);

			double newTouchSize = boost::geometry::distance(centerInPixels, bnewLocation);
			double size = newTouchSize / currentTouchSize;
			EditablePolygon::SizeChanged(size, true);
			currentTouchSize = newTouchSize;

			DBG("radAngle : " + String(currentTouchRotation - radAngle));
			Rotate(currentTouchRotation - radAngle);
			currentTouchRotation = radAngle;
			updateContourPoints();


			double newStartRadius = startRadius * size;
			for (int i = 0; i < Nradius; ++i)
			{

				double newRadius = radius[i] * size;//newStartRadius + i* interval;
				double resize = size;//newRadius / radius[i];
				/*if (bullsEye[i].SizeChanged(resize, false))
				{*/
					startRadius = newStartRadius;
					radius[i] = newRadius; //startRadius + i*interval;
					/*bullsEye[i].updateContourPoints();
				}*/
			}


			CanvasResized(parentCanvas);
			RefreshOpenGLBuffers();
			return AreaEventType::RotScale;
		}
	}
	else
	{
		// pas d'action
		return AreaEventType::NothingHappened;
	}
}

AreaEventType CompletePolygon::EndMultiTouchPointMove()
{
	multiTouchActionBegun = false;

	/// verification de l'orientation !
	numAngles = 48;
	double e = 0.01;
	orientationAngle = rotationAngle;
	rotationAngle = 0;
	bool alreadyFound = false;
	double distanceMin = 2 * M_PI;
	double angleToReach = 0;
	for (int i = 0; i <= numAngles; ++i)
	{
		double currentAngle = (double)i * 2 * M_PI / (double)numAngles;

		double distance = currentAngle - orientationAngle;
		if (abs(distance)<e / 2.0) // vérifie si près de cet angle là
		{
			Rotate(orientationAngle - currentAngle);
			alreadyFound = true;
			orientationAngle = currentAngle;
			break;
		}
		else // sinon regarder la distance
		{
			if (abs(distance) < abs(distanceMin))
			{
				distanceMin = distance;
				angleToReach = currentAngle;
			}
		}

	}

	if (alreadyFound == false) // s'il ne correspondait à aucun angle parmi ceux-ci, on le remet sur le plus proche
	{
		Rotate(orientationAngle - angleToReach);
		orientationAngle = angleToReach;
	}

	updateContourPoints();
	CanvasResized(parentCanvas);
	RefreshOpenGLBuffers();


	currentTouchRotation = 0;

	AreaEventType eventType = AreaEventType::NothingHappened;

	// The point drag is always stopped without any check, for now
	pointDraggedId = EditableAreaPointId::None;
	eventType = AreaEventType::PointDragStops;


	return eventType;
}

AreaEventType CompletePolygon::TryBeginPointMove(const Point<double>& newLocation)
{
	AreaEventType areaEventType = EditablePolygon::TryBeginPointMove(newLocation);
	DBG("eventType : " + (String)((int)areaEventType));
	if (pointDraggedId == EditableAreaPointId::Center)
	{
		//pointDraggedId = EditableAreaPointId::WholeArea;
		lastLocation = newLocation;
	}

	if (onlyRotationAllowed && (pointDraggedId == EditableAreaPointId::WholeArea || pointDraggedId == EditableAreaPointId::Center))
	{
		pointDraggedId = EditableAreaPointId::None;
	}

	/*for (int i = 0; i < Nradius; ++i)
		bullsEye[i].TryBeginPointMove(newLocation);*/
	return areaEventType;
}

AreaEventType CompletePolygon::TryMovePoint(const Point<double>& newLocation)
{
	//DBG((String)newLocation.x + "   " + (String)newLocation.y);
	//DBG("radius 0  = " + (String)radius[0]);
	//DBG("radius 1  = " + (String)radius[1]);
	double r1 = boost::geometry::distance(centerInPixels, bmanipulationPointInPixels);
	int oldPointDraggedId = pointDraggedId;
	if (pointDraggedId == EditableAreaPointId::Center)
	{
		pointDraggedId = EditableAreaPointId::WholeArea; // pour que EditablePolygon::TryMovePoint déplace toute l'aire et pas seulement le centre quand on touche le centre
	}

	AreaEventType areaEventType;
	if (onlyRotationAllowed)
	{
		if (pointDraggedId == EditableAreaPointId::ManipulationPoint)
		{
			bpt location(newLocation.x, newLocation.y);
			boost::geometry::subtract_point(location, centerInPixels);
			boost::geometry::model::point<long double, 3, boost::geometry::cs::cartesian> pt3D(location.get<0>(), location.get<1>());
			boost::geometry::model::point<double, 3, boost::geometry::cs::spherical<boost::geometry::radian>> ptRad;
			boost::geometry::transform(pt3D, ptRad);
			ptRad.set<2>(boost::geometry::distance(centerInPixels, bmanipulationPointInPixels));
			boost::geometry::transform(ptRad, pt3D);
			boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian> pt3Dshort;
			boost::geometry::transform(pt3D, pt3Dshort);
			location = bpt(pt3Dshort.get<0>(), pt3Dshort.get<1>());
			boost::geometry::add_point(location, centerInPixels);
			const Point<double> correctLocation(location.get<0>(), location.get<1>());
			areaEventType = EditablePolygon::TryMovePoint(correctLocation);
		}
		else
			areaEventType = EditablePolygon::TryMovePoint(newLocation);
	}
	else
		areaEventType = EditablePolygon::TryMovePoint(newLocation);

	pointDraggedId = oldPointDraggedId;

	DBG("eventType : " + (String)((int)areaEventType));
	double r2 = boost::geometry::distance(centerInPixels, bmanipulationPointInPixels);
	double size = r2 / r1;

	//if (!boost::geometry::equals(center, bullsEyeCenter)) // at the creation, they're equal
	//{
	//	bpt translation(center.get<0>() - bullsEyeCenter.get<0>(), center.get<1>() - bullsEyeCenter.get<1>());
	//	
	//	translation.set<0>(translation.get<0>() * (double)parentCanvas->getWidth());
	//	translation.set<1>(translation.get<1>() * (double)parentCanvas->getHeight());
	//	

	//	for (int i = 0; i < Nradius; ++i)
	//		bullsEye[i].Translate(juce::Point<double>(translation.get<0>(), translation.get<1>()));
	//	
	//	bullsEyeCenter = center;
	//}

	if (areaEventType == AreaEventType::RotScale)
	{
		double newStartRadius = startRadius * size;
		for (int i = 0; i < Nradius; ++i)
		{

			double newRadius = radius[i] * size;//newStartRadius + i* interval;
			double resize = size;//newRadius / radius[i];
			/*if (bullsEye[i].SizeChanged(resize,false))
			{*/
				startRadius = newStartRadius;
				radius[i] = newRadius; //startRadius + i*interval;
				/*if (radius[i] > 0.0456)
					bullsEye[i].setVerticesCount(64);
				else
					bullsEye[i].setVerticesCount(32);
				bullsEye[i].updateContourPoints();*/
			//}
		}
	}
	//CanvasResized(this->parentCanvas);
	RefreshTargetOpenGLBuffers();

	return areaEventType;
}

AreaEventType CompletePolygon::EndPointMove()
{

	
	double e = 0.01;
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
				if (epsilon > abs(radiusInPixels[i]/*bullsEye[i].getRadius()*/ - Di))
				{
					epsilon = abs(radiusInPixels[i]/*bullsEye[i].getRadius()*/ - Di);
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
			while (angle < 0.0)
				angle += 2 * M_PI;

			bool alreadyFound = false;
			double distanceMin = 2 * M_PI;
			double angleToReach = 0;
			for (int i = 0; i <= numAngles; ++i)
			{
				double currentAngle = (double)i * 2 * M_PI / (double)numAngles;

				double distance = currentAngle - angle;
				if (abs(distance)<e / 2.0) // vérifie si près de cet angle là
				{
					//Rotate(orientationAngle - currentAngle);
					alreadyFound = true;
					angle = currentAngle;
					break;
				}
				else // sinon regarder la distance
				{
					if (abs(distance) < abs(distanceMin))
					{
						distanceMin = distance;
						angleToReach = currentAngle;
					}
				}

			}

			if (alreadyFound == false) // s'il ne correspondait à aucun angle parmi ceux-ci, on le remet sur le plus proche
			{
				//Rotate(orientationAngle - angleToReach);
				angle = angleToReach;
			}


			double R = radiusInPixels[nearest];//bullsEye[nearest].getRadius();//54;//bullsEye[nearest].getRadius();//radius[nearest];
			contourPointsInPixels.outer().at(pointDraggedId) = bpt(centerInPixels.get<0>() + R * std::cos(angle),
					centerInPixels.get<1>() + R * std::sin(angle));
			contourPoints.outer().at(pointDraggedId) = bpt(
					contourPointsInPixels.outer().at(pointDraggedId).get<0>() / parentCanvas->getWidth(),
					contourPointsInPixels.outer().at(pointDraggedId).get<1>() / parentCanvas->getHeight()
			);

				//DBG("difference = " + (String)boost::geometry::distance(contourPointsInPixels.outer().at(pointDraggedId), contourPointsInPixels.outer().at(2))); // = 53,4 = 267*0.2 -> -cos(angle)* R *Width
			
			

			if (pointDraggedId == 0) // first point = last point
			{
				contourPointsInPixels.outer().at(contourPointsInPixels.outer().size() - 1) = contourPointsInPixels.outer().at(0);
				contourPoints.outer().at(contourPoints.outer().size() - 1) = contourPoints.outer().at(0);
			}
			OnCircles.at(pointDraggedId) = nearest;
			if (pointDraggedId == 0)
				OnCircles.at(OnCircles.size() - 1) = nearest;
			
		}
#if !defined(OPENGLRENDERING) || OPENGLRENDERING == 0
		CanvasResized(parentCanvas);
#else
		RefreshOpenGLBuffers();
#endif
	}

	// si on a touché au manipulationPoit -> on a changé l'orientation : vérifié qu'elle soit autorisée et la repositionnée si besoin
	if (pointDraggedId == EditableAreaPointId::ManipulationPoint)
	{
		orientationAngle = rotationAngle;
		rotationAngle = 0;
		bool alreadyFound = false;
		double distanceMin = 2 * M_PI;
		double angleToReach = 0;
		for (int i = 0; i <= numAngles; ++i)
		{
			double currentAngle = (double)i * 2 * M_PI / (double)numAngles;

			double distance = currentAngle - orientationAngle;
			if (abs(distance)<e/2.0) // vérifie si près de cet angle là
			{
				Rotate(orientationAngle - currentAngle);
				alreadyFound = true;
				orientationAngle = currentAngle;
				break;
			}
			else // sinon regarder la distance
			{
				if (abs(distance) < abs(distanceMin))
				{
					distanceMin = distance;
					angleToReach = currentAngle;
				}
			}

		}

		if (alreadyFound == false) // s'il ne correspondait à aucun angle parmi ceux-ci, on le remet sur le plus proche
		{
			Rotate(orientationAngle - angleToReach);
			orientationAngle = angleToReach;
		}

		updateContourPoints();
		CanvasResized(parentCanvas);
		RefreshOpenGLBuffers();
	}
	
	AreaEventType eventType =  EditablePolygon::EndPointMove();
	
	
	return eventType;
}

void CompletePolygon::setCursorVisible(bool isVisible, SceneCanvasComponent* _parentCanvas)
{
	for (int i = 0; i < (int)cursors.size(); i++)
	{
		cursors[i]->CanvasResized(_parentCanvas);
		cursors[i]->RefreshOpenGLBuffers();
	}
	showCursor = isVisible;
	
	//CanvasResized(this->parentCanvas);

	//DBG("showCursor = " + (String)showCursor);
	//setReadingPosition(0);
}

bpolygon CompletePolygon::getPolygon()
{
	return contourPoints;
}

//std::shared_ptr<CompletePolygon> CompletePolygon::fusion(std::shared_ptr<CompletePolygon> polyToFusion, int m_Id)
//{
//
//	// structure to be able to sort the percentages and the circles the same way
//	struct Helper
//	{
//		double pc;
//		int circ;
//		Helper(double a, int b) : pc(a), circ(b) {};
//		//bool operator< (const Helper& b) { return (this->pc < b.pc); }
//		Helper() : pc(0), circ(0) {};
//	};
//
//	// get all the coordinates of the points (percentages and circles) of the current polygon
//	angleToPercent();
//	std::vector<Helper> test;
//	for (int j = 0; j < (int)anglesPercentages.size() - 1; ++j) // stop at size-1 because we don't need the closure point
//	{
//		double value = anglesPercentages[j];
//		if (value > 1)
//			value -= 1;
//		test.push_back(Helper(anglesPercentages[j], OnCircles[j]));
//	}
//
//	// add all the coordinates of the other polygon to the vector of coordinates
//	int i = 0;
//	double value;
//	int distance;
//	while (polyToFusion->getAllPercentages(i, value) && polyToFusion->getAllDistanceFromCenter(i, distance))
//	{
//		test.push_back(Helper(value, distance));
//		++i;
//	}
//	test.pop_back(); // delete last element = closure element of the second polygon
//
//	// sort to have all the points in clockwise order
//	std::sort(test.begin(), test.end(),[](Helper a, Helper b) {return (a.pc < b.pc); });
//
//	// close the polygon
//	test.push_back(Helper(test[0].pc+1,test[0].circ));
//	
//	// delete duplicates
//	std::vector<Helper>::iterator it;
//	it = std::unique(test.begin(), test.end(), [](Helper a, Helper b) {return ((a.circ == b.circ) && (a.pc == b.pc)); });
//	test.resize(std::distance(test.begin(), it));
//
//	// get the XY(pixels) coordinates from the previously sorted coordinates
//	// + separate percentages and circles in different vectors
//	bpolygon newContourPointsInPixels;
//	std::vector<int> newCircles;
//	std::vector<double> newAnglesPercentages;
//	for (int j = 0; j < (int)test.size(); j++)
//	{
//		double R = bullsEye[test[j].circ].getRadius();
//		double angle = test[j].pc * 2 * M_PI;
//		bpt newPoint(R * std::cos(angle), R * std::sin(angle));
//		boost::geometry::add_point(newPoint, centerInPixels);
//		boost::geometry::append(newContourPointsInPixels.outer(),newPoint);
//		newCircles.push_back(test[j].circ);
//		newAnglesPercentages.push_back(test[j].pc);
//	}
//	//boost::geometry::append(newContourPointsInPixels.outer(), newContourPointsInPixels.outer().at(0));
//
//	// get normalized coordinates
//	bpolygon newContourPoints;
//	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> rescaler(1.0 / (double)parentCanvas->getWidth(), 1.0 / (double)parentCanvas->getHeight());
//	boost::geometry::transform(newContourPointsInPixels, newContourPoints, rescaler);
//	
//	/*DBG("new size : " + (String)int(newContourPoints.outer().size()));
//	for (int k = 0; k < test.size(); k++)
//		DBG((String)test[k].circ + "   " + (String)test[k].pc);*/
//
//	// create polygon
//	return std::shared_ptr<CompletePolygon>(new CompletePolygon(m_Id,center,newContourPoints,newCircles,newAnglesPercentages,fillColour));
//
//}

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


double CompletePolygon::getAngularPercentage(bpt hitPoint)
{
	for (int i = 0; i < (int)contourPoints.outer().size(); i++)
	{
		if (boost::geometry::equals(hitPoint, contourPoints.outer().at(i)))
			return anglesPercentages[i];
	}
	
	bpt GT(hitPoint.get<0>() - center.get<0>(), hitPoint.get<1>() - center.get<1>());
	double angle = Miam::Math::ComputePositiveAngle(GT);
	int i = 0;
	while (!subTriangles[i].ContainsAngle(angle))
		i++;

	int prev = 0;
	int suiv = 0;
	if (i == 0)
	{
		prev = (int)contourPoints.outer().size() - 1;
		suiv = 0;
	}
	else
	{
		prev = i - 1;
		suiv = i;
	}

	double ans = angle / (2 * M_PI);
	while (ans < anglesPercentages[prev])
		ans += 1;

	return ans;
	
	//return 0.0;
}

double CompletePolygon::getLinearPercentage(bpt hitPoint)
{
	for (int i = 0; i < (int)contourPoints.outer().size(); i++)
	{
		if (boost::geometry::equals(hitPoint, contourPoints.outer().at(i)))
			return anglesPercentages[i];
	}

	bpt GT(hitPoint.get<0>() - center.get<0>(), hitPoint.get<1>() - center.get<1>());
	double angle = Miam::Math::ComputePositiveAngle(GT);
	int i = 0;
	while (!subTriangles[i].ContainsAngle(angle))
		i++;

	int prev = 0;
	int suiv = 0;
	if (i == 0)
	{
		prev = (int)contourPoints.outer().size() - 1;
		suiv = 0;
	}
	else
	{
		prev = i - 1;
		suiv = i;
	}
	
	double ans = 0;
	if (abs(contourPoints.outer().at(suiv).get<0>() - contourPoints.outer().at(prev).get<0>()) > abs(contourPoints.outer().at(suiv).get<1>() - contourPoints.outer().at(prev).get<1>()))
		ans = anglesPercentages[prev] + ((anglesPercentages[suiv] - anglesPercentages[prev]) / (contourPoints.outer().at(suiv).get<0>() - contourPoints.outer().at(prev).get<0>()))* (hitPoint.get<0>() - contourPoints.outer().at(prev).get<0>());
	else
		ans = anglesPercentages[prev] + ((anglesPercentages[suiv] - anglesPercentages[prev]) / (contourPoints.outer().at(suiv).get<1>() - contourPoints.outer().at(prev).get<1>()))* (hitPoint.get<1>() - contourPoints.outer().at(prev).get<1>());
	return ans;
}

bool CompletePolygon::getAllPercentages(int idx, double &value)
{
	if (useBullsEye)
	{
		angleToPercent();
		//DBG("percentage size : " + (String)percentages.size());
		if (idx < (int)anglesPercentages.size())
		{
			value = anglesPercentages[idx];
			while (value > 1)
				value -= 1; // the angle percentage can be negative to guarantee the cursor deplacement, but the audio needs [o;1[

			return true;
		}
		else
			return false;
	}
	else
	{
		lengthToPercent();
		//DBG("percentage size : " + (String)percentages.size());
		if (idx < (int)percentages.size())
		{
			value = percentages[idx];
			
			return true;
		}
		else
			return false;
	}
}

bool CompletePolygon::getAllDistanceFromCenter(int idx, int &value)
{
	if (idx < (int)OnCircles.size())
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
		//bullsEye.push_back( EditableEllipse(0, center, 2*radius[i], Colours::grey, 1.47f));
		//bullsEye.back().SetAlpha(1.0);
		//bullsEye.back().setIsFilled(false);
		//bullsEye.back().setVerticesCount(64/*6 * bullsEye.back().getVerticesCount()*/);
		
	}
	circlesToShow[0] = true;
	for (int i = 1; i < Nradius; ++i)
	{
		circlesToShow[i] = false;
	}
}

void CompletePolygon::PaintBullsEye(Graphics& g)
{
	if (showAllCircles)
	{
		//for (int i = 0; i < bullsEye.size(); ++i)
		//{
		//	bullsEye[i].SetOpacityMode(OpacityMode::Independent);
		//	bullsEye[i].SetAlpha(0.2f);
		//	//bullsEye[i].Paint(g);
		//}
		g.setOpacity(0.2f);
		int minDimension = parentCanvas->getHeight() > parentCanvas->getWidth() ? parentCanvas->getWidth() - 10 : parentCanvas->getHeight() - 10;
		double currentAngle = 0.0;
		for (int i = 0; i < numAngles; ++i)
		{
			currentAngle += 2 * M_PI / (double)numAngles;
			g.drawLine((float)centerInPixels.get<0>(), (float)centerInPixels.get<1>(), (float)centerInPixels.get<0>()+ (float)minDimension/2.0f * (float)std::cos(currentAngle), (float)centerInPixels.get<1>() + (float)minDimension/2.0f * (float)std::sin(currentAngle));
		}
	}
	else
	{
		for (int i = 0; i < (int)OnCircles.size(); ++i)
		{
			//bullsEye[OnCircles[i]].SetAlpha(1.0);
			//bullsEye[OnCircles[i]].Paint(g);
			//bullsEye[i].Paint(g);
		}
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
		/*bullsEye[i].CanvasResized(_parentCanvas);
		bullsEye[i].RefreshOpenGLBuffers();*/
		radiusInPixels[i] = radius[i] * xScale * _parentCanvas->getWidth();
	}

	
}

bpt CompletePolygon::getCenter()
{
	return centerInPixels;
}

bpt CompletePolygon::getCenterNormalized()
{
	return center;
}

float CompletePolygon::getNormalizedRadius()
{
	float distance = 0.0f;
	for (int i = 0; i < (int)contourPoints.outer().size(); ++i)
		if (boost::geometry::distance(contourPoints.outer().at(i), center) > distance)
			distance = (float)boost::geometry::distance(contourPoints.outer().at(i), center);
	return distance;
}

void CompletePolygon::setCursorsSpeed(int idx, double newSize)
{
	if (newSize == 0)
	{
		setCursorVisible(false,parentCanvas);
	}
	else
	{
		if (showCursor == false)
			setCursorVisible(true,parentCanvas);
		cursors[idx]->setSpeed(newSize);
		
	}
}

void CompletePolygon::setCursorsBaseNote(int idx, double newBaseNote)
{
		if (showCursor == false)
			setCursorVisible(true, parentCanvas);
		//cursors[idx]->SetAlpha(newBaseNote/127.0); //setAlpha(newBaseNote);//setSpeed(newSize);
		cursors[idx]->setMaxAlpha(0.05f + (0.65f - 0.05f) * (float)newBaseNote / 10.0f);/*(newBaseNote / 10.0)+1.0/20.0*/
}

void CompletePolygon::SetActive(bool activate)
{
	EditableArea::SetActive(activate);
}

void CompletePolygon::SetAlpha(float newAlpha)
{
	//if (newAlpha >= 0.3)
		DrawableArea::SetAlpha(newAlpha);
}

bool CompletePolygon::shouldShowOptions()
{
	return true;
}

bool CompletePolygon::contains(bpt point)
{
	return boost::geometry::within(point, contourPoints);
}

void CompletePolygon::deleteAllCursors()
{
	cursors.clear();
		
}

void CompletePolygon::addChordPoints(double m_anglepercentage, std::shared_ptr<CompletePolygon> areaForChord)
{
	chordsAnglePercentage.push_back(m_anglepercentage);
	chordAreaForPercentage.push_back(areaForChord);
}

void CompletePolygon::setChordFlag(bpt chordPt, bool isTrue, std::shared_ptr<CompletePolygon> areaForChord)
{
	for (int i = 0; i < (int)contourPoints.outer().size(); ++i)
	{
		if (boost::geometry::equals(chordPt, contourPoints.outer().at(i)))
		{
			chordFlag.at(i) = isTrue;
			chordAreaForFlag.at(i) = areaForChord;
			break;
		}
	}
}

void CompletePolygon::resetChords()
{
	chordFlag = std::vector<bool>(contourPoints.outer().size(), false);
	chordAreaForFlag = std::vector<std::shared_ptr<CompletePolygon>>(contourPoints.outer().size(), nullptr);
	chordAreaForPercentage.clear();
	chordsAnglePercentage.clear();
}

bool CompletePolygon::getChordParameters(int idx, std::shared_ptr<CompletePolygon>& chordArea, double &m_pC)
{
	if (idx < (int)chordAreaForPercentage.size())
	{
		m_pC = chordsAnglePercentage[idx];
		chordArea = chordAreaForPercentage[idx];
		return true;
	}
	else
	{
		idx -= (int)chordAreaForPercentage.size();
		int N = 0;
		for (int i = 0; i < (int)chordAreaForFlag.size(); ++i)
			if (chordAreaForFlag[i] != nullptr)
				N++;
		if (idx < N)
		{
			// idx = indice en terme d'elt non nuls !
			int count = 0;
			int realIdx = 0;
			for (int j = 0; j < (int)chordAreaForFlag.size(); ++j)
			{
				if (chordAreaForFlag[j] != nullptr)
				{
					count++;
					if (count == idx + 1)
					{
						realIdx = j;
						break;
					}
				}
			}
			m_pC = anglesPercentages[realIdx];
			chordArea = chordAreaForFlag[realIdx];
			return true;
		}
		else
			return false;

	}
	//return false;
}

std::shared_ptr<bptree::ptree> CompletePolygon::GetTree()
{
	auto inheritedTree = DrawablePolygon::GetTree();

	bptree::ptree completeParameterTree;

	bptree::ptree opacityTree;
	opacityTree.put("<xmlattr>.opacity", fillOpacity);
	bptree::ptree orientationTree;
	orientationTree.put("<xmlattr>.orientation", orientationAngle);

	completeParameterTree.put("<xmlattr>.opacity", fillOpacity);//.add_child("opacity", opacityTree);
	completeParameterTree.put("<xmlattr>.orientation", orientationAngle);//.add_child("orientation", orientationTree);
	completeParameterTree.put("<xmlattr>.radius", startRadius);
	completeParameterTree.put("<xmlattr>.interval", interval);
	//completeParameterTree.put("<xmlattr>.xScale", xScale);
	//completeParameterTree.put("<xmlattr>.yScale", yScale);

	inheritedTree->add_child("completeParameter", completeParameterTree);

	return inheritedTree;
}

void CompletePolygon::showAllTarget(bool shouldBeShowed)
{
	if (shouldBeShowed == false && showAllCircles == true)
		deleteOldCircles = true;
	showAllCircles = shouldBeShowed;
}

void CompletePolygon::Translate(const Point<double>& translation)
{
	EditablePolygon::Translate(translation);

	if (!boost::geometry::equals(center, bullsEyeCenter)) // at the creation, they're equal
	{
		bpt btranslation(center.get<0>() - bullsEyeCenter.get<0>(), center.get<1>() - bullsEyeCenter.get<1>());

		btranslation.set<0>(btranslation.get<0>() * (double)parentCanvas->getWidth());
		btranslation.set<1>(btranslation.get<1>() * (double)parentCanvas->getHeight());


		/*for (int i = 0; i < Nradius; ++i)
			bullsEye[i].Translate(juce::Point<double>(btranslation.get<0>(), btranslation.get<1>()));*/

		bullsEyeCenter = center;
	}

	
}

void CompletePolygon::DisableTranslation(bool shouldBeDisabled)
{
	onlyRotationAllowed = shouldBeDisabled;
}

double CompletePolygon::GetFullSceneRatio()
{
	if (parentCanvas->getHeight() < parentCanvas->getWidth())
		return (parentCanvas->getHeight() - 10) / (2 * radiusInPixels[4]/*radius[4] * parentCanvas->getHeight() * yScale*/);
	else
		return (parentCanvas->getWidth() - 10) / (2 * radiusInPixels[4]/*radius[4] * parentCanvas->getWidth() * xScale*/);
}

bool CompletePolygon::SizeChanged(double _size, bool minSize)
{
	bool ans = EditablePolygon::SizeChanged(_size,minSize);
	if (ans) // le changement de taille de la forme a été effectué -> changer la taille des cercles
	{
		double newStartRadius = startRadius * _size;
		for (int i = 0; i < Nradius; ++i)
		{

			double newRadius = radius[i] * _size;//newStartRadius + i * interval;
			double resize = _size;//newRadius / radius[i];
			/*if (bullsEye[i].SizeChanged(resize, false))
			{*/
				startRadius = newStartRadius;
				radius[i] = newRadius; //startRadius + i*interval;
			/*	bullsEye[i].updateContourPoints();
			}*/
		}
	}
	return ans;
}

void CompletePolygon::RefreshTargetOpenGLBuffers()
{
	//int decalage = EditablePolygon::GetVerticesBufferSize();
	//const int circlesSize = (int)bullsEye.size();
	//const int incDecalage = bullsEye[0].GetVerticesBufferSize();
	//const int circleVertexCount = 3 * bullsEye[0].GetVerticesBufferSize();
	//const int circleIndiceCount = bullsEye[0].GetIndicesBufferSize();
	//const int circleCoulourCount = bullsEye[0].GetCouloursBufferSize();


	//if (circlesSize > 0)
	//{
	//	if (showAllCircles)
	//	{
	//		/*for (int i = 0; i < circlesSize; ++i)
	//			bullsEye[i].RefreshOpenGLBuffers();*/

	//		// vertex
	//		for (int i = 0; i < circlesSize; ++i)
	//		{
	//			float *circleVertexPtr = bullsEye[i].GetVerticesBufferPtr();
	//			float *vertexPtr = &vertices_buffer[3 * decalage];
	//			for (int j = 0; j < circleVertexCount; ++j)
	//			{
	//				vertexPtr[j] = circleVertexPtr[j];
	//				//vertices_buffer[3 * decalage + j] = circleVertexPtr[j];
	//			}
	//			decalage += incDecalage;
	//		}

	//		// index
	//		decalage = EditablePolygon::GetIndicesBufferSize(); // pour decaler dans le buffer
	//		int begin = EditablePolygon::GetVerticesBufferSize(); // pour prendre en compte le nombre de vertex
	//		for (int i = 0; i < circlesSize; ++i)
	//		{
	//			unsigned int *circleIndicesPtr = bullsEye[i].GetIndicesBufferPtr();
	//			unsigned int *indicesPtr = &indices_buffer[decalage];
	//			for (int j = 0; j < circleIndiceCount; ++j)
	//			{
	//				indicesPtr[j] = circleIndicesPtr[j] + begin;
	//			}
	//			decalage += circleIndiceCount; // on se decale dans le buffer
	//			begin += incDecalage; // il faut ajouter aux indices le nombre de vertex déjà existant
	//		}

	//		// coulour
	//		decalage = EditablePolygon::GetCouloursBufferSize();
	//		for (int i = 0; i < circlesSize; ++i)
	//		{
	//			float *circleCoulourPtr = bullsEye[i].GetCoulourBufferPtr();
	//			float *coulourPtr = &coulours_buffer[decalage];
	//			for (int j = 0; j < circleCoulourCount; ++j)
	//			{
	//				coulourPtr[j] = circleCoulourPtr[j];
	//			}
	//			decalage += circleCoulourCount;
	//		}
	//	}
	//	else
	//	{
	//		std::vector<int> needToShow = OnCircles;
	//		auto end = std::unique(needToShow.begin(), needToShow.end()); // enleve juste les doublons consecutif -> il peut rester des doublons mais qui dessineront la mm chose :)
	//		int newSize = (int)std::distance(needToShow.begin(), end);
	//		while (newSize > Nradius)
	//		{
	//			end = std::unique(needToShow.begin(), needToShow.end());
	//			newSize = (int)std::distance(needToShow.begin(), end);
	//		}

	//		bool needToResetPart = false;
	//		if (newSize < previousSizeToShow)
	//			needToResetPart = true;
	//			
	//		previousSizeToShow = newSize;

	//		/*for (int i = 0; i < newSize; ++i)
	//			bullsEye[needToShow[i]].RefreshOpenGLBuffers();*/

	//		//vertex
	//		for (int i = 0; i < newSize; ++i)
	//		{

	//			float *circleVertexPtr = bullsEye[needToShow[i]].GetVerticesBufferPtr();
	//			float *vertexPtr = &vertices_buffer[3 * decalage];
	//			for (int j = 0; j < circleVertexCount; ++j)
	//			{
	//				vertexPtr[j] = circleVertexPtr[j];
	//				//vertices_buffer[3 * decalage + j] = circleVertexPtr[j];
	//			}
	//			decalage += incDecalage;
	//		}
	//		if (needToResetPart || deleteOldCircles)
	//		{
	//			int count = (Nradius - newSize) * incDecalage;
	//			float *vertexPtr = &vertices_buffer[3 * decalage];
	//			for (int i = 0; i < count; ++i)
	//			{
	//				vertexPtr[i] = 0.0f;
	//			}
	//		}

	//		// index
	//		decalage = EditablePolygon::GetIndicesBufferSize(); // pour decaler dans le buffer
	//		int begin = EditablePolygon::GetVerticesBufferSize(); // pour prendre en compte le nombre de vertex
	//		for (int i = 0; i < newSize; ++i)
	//		{
	//			unsigned int *circleIndicesPtr = bullsEye[needToShow[i]].GetIndicesBufferPtr();
	//			unsigned int *indicesPtr = &indices_buffer[decalage];
	//			for (int j = 0; j < circleIndiceCount; ++j)
	//			{
	//				indicesPtr[j] = circleIndicesPtr[j] + begin;
	//			}
	//			decalage += circleIndiceCount; // on se decale dans le buffer
	//			begin += incDecalage; // il faut ajouter aux indices le nombre de vertex déjà existant
	//		}
	//		if (needToResetPart || deleteOldCircles)
	//		{
	//			int count = (Nradius - newSize) * circleIndiceCount;
	//			unsigned int *indicesPtr = &indices_buffer[decalage];
	//			for (int i = 0; i < count; ++i)
	//			{
	//				indicesPtr[i] = 0;
	//			}
	//		}

	//		// coulour
	//		decalage = EditablePolygon::GetCouloursBufferSize();
	//		for (int i = 0; i < newSize; ++i)
	//		{
	//			float *circleCoulourPtr = bullsEye[needToShow[i]].GetCoulourBufferPtr();
	//			float *coulourPtr = &coulours_buffer[decalage];
	//			for (int j = 0; j < circleCoulourCount; ++j)
	//			{
	//				coulourPtr[j] = circleCoulourPtr[j];
	//			}
	//			decalage += circleCoulourCount;
	//		}
	//		if (needToResetPart || deleteOldCircles)
	//		{
	//			int count = (Nradius - newSize) * circleCoulourCount;
	//			float *coulourPtr = &coulours_buffer[decalage];
	//			for (int i = 0; i < count; ++i)
	//			{
	//				coulourPtr[i] = 0.0f;
	//			}
	//		}

	//	}
	//}
}

void CompletePolygon::RefreshOpenGLBuffers()
{
	EditablePolygon::RefreshOpenGLBuffers();
	RefreshTargetOpenGLBuffers();
}

void CompletePolygon::setOldVelocity(double value)
{
	oldVelocity = value;
}

double CompletePolygon::getOldVelocity()
{
	return oldVelocity;
}
