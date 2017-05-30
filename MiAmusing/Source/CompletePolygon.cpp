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
	
	showBullsEye = false;
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
		percentages.push_back(0);
	percentages.reserve(contourPoints.outer().size());
	SetNameVisible(false);
	
	showBullsEye = true;
	int Nradius = 1;
	int startRadius = radius;
	//updateSubTriangles();
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
		percentages.push_back(0);
	percentages.reserve(contourPoints.outer().size());
	SetNameVisible(false);
	
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
	if (showBullsEye)
	{
		
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
	setReadingPosition(pc);
	//cursor->CanvasResized(_parentCanvas);
	//DBG("cursorCenter : " + (String)(cursorCenter.x) + " " + (String)(cursorCenter.y));
	//DBG("CompletePolygon::CanvasResized");
	pointDraggingRadius = 0.05f * (parentCanvas->getWidth() + parentCanvas->getHeight()) / 2.0f; // 5% => mieux pour le touch
}

AreaEventType CompletePolygon::TryMovePoint(const Point<double>& newLocation)
{
	AreaEventType areaEventType = EditablePolygon::TryMovePoint(newLocation);
	//CanvasResized(this->parentCanvas);
	return areaEventType;
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

std::vector<bpt> CompletePolygon::intersection(bpolygon hitPolygon)
{
	std::vector<bpt> inter;
	boost::geometry::intersection(contourPoints, hitPolygon, inter);
	//DBG("------ A ------");
	//for (int i = 0; i < (int)bcontourPoints.outer().size(); ++i)
	//	DBG((String)bcontourPoints.outer().at(i).get<0>() + "  " + (String)bcontourPoints.outer().at(i).get<1>());
	//DBG("------ B ------");
	/*
	for (int i = 0; i < (int)hitPolygon.outer().size(); ++i)
		DBG((String)hitPolygon.outer().at(i).get<0>() + "  " + (String)hitPolygon.outer().at(i).get<1>());
	DBG("------ I ------");
	for (int j = 0; j < (int)inter.size(); ++j)
	{
		DBG("..." + (String)j + "...");
		//for (int i = 0; i < (int)inter.size(); ++i)
		int i = j;
			DBG((String)inter.at(i).get<0>() + " " + (String)inter.at(i).get<1>());
		//for (int i = 0; i < (int)inter.at(j).outer().size(); ++i)
		//	DBG((String)inter.at(j).outer().at(i).get<0>() + "  " + (String)inter.at(j).outer().at(i).get<1>());
	}
	*/
	return inter;
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
	return percentages[prev] + (boost::geometry::distance(hitPoint, contourPoints.outer().at(i - 1))) / perimeter;
	
	//return 0.0;
}

bool CompletePolygon::getAllPercentages(int idx, double &value)
{
	lengthToPercent();
	//DBG("percentage size : " + (String)percentages.size());
	if (idx < percentages.size())
	{
		value = percentages[idx];
		return true;
	}
	else
		return false;
}

void CompletePolygon::CreateBullsEye()
{
	bullsEye = new EditableEllipse*[N];
	for (int i = 0; i < N; ++i)
	{
		bullsEye[i] = new EditableEllipse(0, center, 0.15f, 0.15f, Colours::grey, 1.47f);
	}
}

void CompletePolygon::DeleteBullsEye()
{

}

void CompletePolygon::PaintBullsEye(Graphics& g)
{
	//for(int i=0;i<Nradius;i++)

}

void CompletePolygon::CanvasResizedBullsEye(SceneCanvasComponent* _parentCanvas)
{

}