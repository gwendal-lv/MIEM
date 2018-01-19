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


#include "boost\geometry.hpp"
#include "boost\geometry\algorithms\transform.hpp"

using namespace Amusing;
using namespace Miam;

CompletePolygon::CompletePolygon(int64_t _Id) : EditablePolygon(_Id)
{

	/*std::string s;
	std::ostringstream ost;
	ost << this;
	s = ost.str();
	DBG("Constructor : " + s);*/

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
	
}

CompletePolygon::CompletePolygon(int64_t _Id,
	bpt _center, bpolygon& _contourPoints,
	std::vector<int> circles, std::vector<double> percent,
	Colour _fillColour) : 
	EditablePolygon(_Id, _center, _contourPoints, _fillColour)
{
	/*std::string s;
	std::ostringstream ost;
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


	// ajouter le calcul du rayon des cercles : on connait les coordonnees des points et a quels cercles ils appartienne -> possible de retrouver le rayon et le centre ! 
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
		for(int i=0;i<(int)cursors.size();i++)
			cursors[i]->Paint(g);
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
		prev = contourPoints.outer().size() - 1;
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
		prev = contourPointsInPixels.outer().size() - 1;
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

float CompletePolygon::computeCursorAlpha(double p, bpt _center)
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
	double H = 0.5;  // opacit� lorsqu'on est pas assez proche d"un sommet
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
		return 0.5f;
	

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

AreaEventType CompletePolygon::TryBeginPointMove(const Point<double>& newLocation)
{
	AreaEventType areaEventType = EditablePolygon::TryBeginPointMove(newLocation);
	DBG("eventType : " + (String)((int)areaEventType));
	if (pointDraggedId == EditableAreaPointId::Center)
	{
		pointDraggedId = EditableAreaPointId::WholeArea;
		lastLocation = newLocation;
	}
	for (int i = 0; i < Nradius; ++i)
		bullsEye[i].TryBeginPointMove(newLocation);
	return areaEventType;
}

AreaEventType CompletePolygon::TryMovePoint(const Point<double>& newLocation)
{
	//DBG((String)newLocation.x + "   " + (String)newLocation.y);
	//DBG("radius 0  = " + (String)radius[0]);
	//DBG("radius 1  = " + (String)radius[1]);
	double r1 = boost::geometry::distance(centerInPixels, bmanipulationPointInPixels);
	AreaEventType areaEventType = EditablePolygon::TryMovePoint(newLocation);
	DBG("eventType : " + (String)((int)areaEventType));
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
			if (bullsEye[i].SizeChanged(resize,false))
			{
				startRadius = newStartRadius;
				radius[i] = newRadius; //startRadius + i*interval;
				bullsEye[i].updateContourPoints();
			}
		}
	}
	//CanvasResized(this->parentCanvas);
	return areaEventType;
}

AreaEventType CompletePolygon::EndPointMove()
{

	int numAngles = 32;
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
		CanvasResized(parentCanvas);
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
	}
	
	AreaEventType eventType =  EditablePolygon::EndPointMove();
	
	
	return eventType;
}

void CompletePolygon::setCursorVisible(bool isVisible, SceneCanvasComponent* _parentCanvas)
{
	for(int i=0;i<(int)cursors.size();i++)
		cursors[i]->CanvasResized(_parentCanvas);
	showCursor = isVisible;
	
	//CanvasResized(this->parentCanvas);

	//DBG("showCursor = " + (String)showCursor);
	//setReadingPosition(0);
}

bpolygon CompletePolygon::getPolygon()
{
	return contourPoints;
}

std::shared_ptr<AreaEvent> CompletePolygon::intersection(std::shared_ptr<CompletePolygon> hitPolygon, int m_Id, int N)
{
	

	// compute the intersection
	bpolygon poly1, poly2;
	poly1 = contourPoints;
	poly2 = hitPolygon->getPolygon();
	boost::geometry::correct(poly1);
	boost::geometry::correct(poly2);
	std::deque<bpolygon> inter;
	boost::geometry::intersection(poly1, poly2, inter);

	
	// compute the area of the 2 polygons and the intersection
	double area = abs(boost::geometry::area(poly1));
	double hitArea = abs(boost::geometry::area(poly2));

	double areaInter = 0;
	for(int i = 0; i<(int)inter.size();i++)
		areaInter += abs(boost::geometry::area(inter[0]));


	//DBG((String)areaTest + " >= " + (String)double(0.75 * hitArea) + " ou " + (String)double(0.75 * area));
	std::shared_ptr<CompletePolygon> completeP;
	std::shared_ptr<AreaEvent> areaE;
	/*
	if (areaInter >= 0.75 * hitArea || areaInter >= 0.75 * area)
	{
		//DBG("need To fusion ! : " + (String)areaInter + " >= " + (String)double(0.75 * hitArea) + " ou " + (String)double(0.75 * area));
		completeP = fusion(hitPolygon, m_Id);
		//DBG("fusionned");
		areaE = std::shared_ptr<AreaEvent>(new AreaEvent(completeP, AreaEventType::Added));
	}*/
	if(inter.size() >= 0)
	{
		//DBG("number of polygon by intersection : " + (String)inter.size());
		std::shared_ptr<MultiAreaEvent> multiE(new MultiAreaEvent());
		for (int i = 0; i < (int)inter.size(); i++)
		{
			//if (boost::geometry::area(inter[i]) > minimumSizePercentage)//*(parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0)
			//{
				// compute the center coordinate
			bpt interCenter;
			boost::geometry::reverse(inter[i]);
			boost::geometry::centroid(inter[i], interCenter);

			// compute the notes // verifier ordre des points si le son est bizarre
			std::vector<int> newCircles;
			std::vector<double> newAnglesPercentages;
			for (int j = 0; j < (int)inter[i].outer().size(); j++)
			{
				//newCircles.push_back(0);

				if (boost::geometry::within(inter[i].outer().at(j), poly1))
					newCircles.push_back(OnCircles[0]);
				else if (boost::geometry::within(inter[i].outer().at(j), poly2))
				{
					int val;
					hitPolygon->getAllDistanceFromCenter(0, val);
					newCircles.push_back(val);
				}
				else
				{
					int val;
					hitPolygon->getAllDistanceFromCenter(0, val);
					newCircles.push_back(val + OnCircles[0]);
				}


				bpt centeredPt(inter[i].outer().at(j));
				boost::geometry::subtract_point(centeredPt, interCenter);
				newAnglesPercentages.push_back(Math::ComputePositiveAngle(centeredPt)/ (2* M_PI));
				while (j > 0 && newAnglesPercentages[j] < newAnglesPercentages[j - 1])
					newAnglesPercentages[j] += 1;
			}

			// create the polygon and the event
			completeP = std::shared_ptr<CompletePolygon>(new CompletePolygon(N + i, interCenter, inter[i], newCircles, newAnglesPercentages, juce::Colours::red));

			// intersection's polygon must not be clickable and won't show the circles
			completeP->SetActive(false);

			// add the cration of the polygon in the MultiAreaEvent
			multiE->AddAreaEvent(std::shared_ptr<AreaEvent>(new AreaEvent(completeP, AreaEventType::NothingHappened)));
			//}
		}
		return multiE;
		//areaE = std::shared_ptr<AreaEvent>(new AreaEvent(completeP, AreaEventType::NothingHappened));
	}
	else
	{
		std::shared_ptr<MultiAreaEvent> multiE(new MultiAreaEvent());
		return multiE;
	}
	
}

std::shared_ptr<CompletePolygon> CompletePolygon::fusion(std::shared_ptr<CompletePolygon> polyToFusion, int m_Id)
{

	// structure to be able to sort the percentages and the circles the same way
	struct Helper
	{
		double pc;
		int circ;
		Helper(double a, int b) : pc(a), circ(b) {};
		//bool operator< (const Helper& b) { return (this->pc < b.pc); }
		Helper() : pc(0), circ(0) {};
	};

	// get all the coordinates of the points (percentages and circles) of the current polygon
	angleToPercent();
	std::vector<Helper> test;
	for (int j = 0; j < (int)anglesPercentages.size() - 1; ++j) // stop at size-1 because we don't need the closure point
	{
		double value = anglesPercentages[j];
		if (value > 1)
			value -= 1;
		test.push_back(Helper(anglesPercentages[j], OnCircles[j]));
	}

	// add all the coordinates of the other polygon to the vector of coordinates
	int i = 0;
	double value;
	int distance;
	while (polyToFusion->getAllPercentages(i, value) && polyToFusion->getAllDistanceFromCenter(i, distance))
	{
		test.push_back(Helper(value, distance));
		++i;
	}
	test.pop_back(); // delete last element = closure element of the second polygon

	// sort to have all the points in clockwise order
	std::sort(test.begin(), test.end(),[](Helper a, Helper b) {return (a.pc < b.pc); });

	// close the polygon
	test.push_back(Helper(test[0].pc+1,test[0].circ));
	
	// delete duplicates
	std::vector<Helper>::iterator it;
	it = std::unique(test.begin(), test.end(), [](Helper a, Helper b) {return ((a.circ == b.circ) && (a.pc == b.pc)); });
	test.resize(std::distance(test.begin(), it));

	// get the XY(pixels) coordinates from the previously sorted coordinates
	// + separate percentages and circles in different vectors
	bpolygon newContourPointsInPixels;
	std::vector<int> newCircles;
	std::vector<double> newAnglesPercentages;
	for (int j = 0; j < (int)test.size(); j++)
	{
		double R = bullsEye[test[j].circ].getRadius();
		double angle = test[j].pc * 2 * M_PI;
		bpt newPoint(R * std::cos(angle), R * std::sin(angle));
		boost::geometry::add_point(newPoint, centerInPixels);
		boost::geometry::append(newContourPointsInPixels.outer(),newPoint);
		newCircles.push_back(test[j].circ);
		newAnglesPercentages.push_back(test[j].pc);
	}
	//boost::geometry::append(newContourPointsInPixels.outer(), newContourPointsInPixels.outer().at(0));

	// get normalized coordinates
	bpolygon newContourPoints;
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> rescaler(1.0 / (double)parentCanvas->getWidth(), 1.0 / (double)parentCanvas->getHeight());
	boost::geometry::transform(newContourPointsInPixels, newContourPoints, rescaler);
	
	/*DBG("new size : " + (String)int(newContourPoints.outer().size()));
	for (int k = 0; k < test.size(); k++)
		DBG((String)test[k].circ + "   " + (String)test[k].pc);*/

	// create polygon
	return std::shared_ptr<CompletePolygon>(new CompletePolygon(m_Id,center,newContourPoints,newCircles,newAnglesPercentages,fillColour));

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
		prev = contourPoints.outer().size() - 1;
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
		prev = contourPoints.outer().size() - 1;
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
		bullsEye.push_back( EditableEllipse(0, center, 2*radius[i], Colours::grey, 1.47f));
		bullsEye.back().SetAlpha(1.0);
		//bullsEye.back().Set
		bullsEye.back().setIsFilled(false);
		
	}
	circlesToShow[0] = true;
	for (int i = 1; i < Nradius; ++i)
	{
		circlesToShow[i] = false;
	}
}

void CompletePolygon::PaintBullsEye(Graphics& g)
{
	for (int i = 0; i < (int)OnCircles.size(); ++i)
	{
		//bullsEye[OnCircles[i]].SetAlpha(1.0);
		bullsEye[OnCircles[i]].Paint(g);
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

void CompletePolygon::SetActive(bool activate)
{
	EditableArea::SetActive(activate);
}

void CompletePolygon::SetAlpha(float newAlpha)
{
	if (newAlpha >= 0.3)
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
		idx -= chordAreaForPercentage.size();
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
