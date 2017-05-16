/*
  ==============================================================================

    CompletePolygon.cpp
    Created: 5 May 2017 11:39:08am
    Author:  ayup1

  ==============================================================================
*/

#include "CompletePolygon.h"
#include "SceneCanvasComponent.h"

using namespace Amusing;
using namespace Miam;

CompletePolygon::CompletePolygon(int64_t _Id) : EditablePolygon(_Id)
{
	showCursor = false;
	pc = 0;
	cursorCenter = bcontourPoints.outer().at(0);
	cursor = std::shared_ptr<EditableEllipse>(new EditableEllipse(0, cursorCenter, 0.1f, 0.1f, Colours::grey, 1.47f));
	cursor->SetNameVisible(false);
	//percentages.reserve(contourPoints.size());
	for (int i = 0; i < (int)bcontourPoints.outer().size(); ++i)
		percentages.push_back(0);
	SetNameVisible(false);
	

}

CompletePolygon::CompletePolygon(int64_t _Id, bpt _center, int pointsCount, float radius,
	Colour _fillColour, float _canvasRatio) :
	EditablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
	showCursor = false;
	pc = 0;
	cursorCenter = bcontourPoints.outer().at(0);
	cursor = std::shared_ptr<EditableEllipse>(new EditableEllipse(0, cursorCenter, 0.1f, 0.1f, Colours::grey, _canvasRatio));
	cursor->SetNameVisible(false);
	for (int i = 0; i < (int)bcontourPoints.outer().size(); ++i)
		percentages.push_back(0);
	percentages.reserve(bcontourPoints.outer().size());
	SetNameVisible(false);
}

CompletePolygon::CompletePolygon(int64_t _Id,
	bpt _center, bpolygon& _contourPoints, Colour _fillColour) :
	EditablePolygon(_Id, _center, _contourPoints, _fillColour)
{
	showCursor = false;
	pc = 0;
	cursorCenter = bcontourPoints.outer().at(0);
	cursor = std::shared_ptr<EditableEllipse>(new EditableEllipse(0, cursorCenter, 0.1f, 0.1f, Colours::grey, 1.47f));
	cursor->SetNameVisible(false);
	for (int i = 0; i < (int)bcontourPoints.outer().size(); ++i)
		percentages.push_back(0);
	percentages.reserve(bcontourPoints.outer().size());
	SetNameVisible(false);
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

	DBG("showCursorPaint = " + (String)showCursor);
	if (showCursor)
	{
		//DBG("paint cursor");
		cursor->Paint(g);
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
	perimeter = boost::geometry::perimeter(bcontourPoints);

	//calcul des poucentages correspondant à chaque point
	//DBG("-----------------");
	percentages[0] = 0;
	for (int i = 1; i < (int)bcontourPoints.outer().size(); ++i)
	{
		percentages[i] = percentages[i-1] + (boost::geometry::distance(bcontourPoints.outer().at(i),bcontourPoints.outer().at(i-1)))/ perimeter;
		//DBG((String)percentages[i]);
	}
	//DBG("-----------------");
}

void CompletePolygon::setReadingPosition(double p)
{
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
			P.set<0>(bcontourPoints.outer().at(prev).get<0>() + ((p - percentages[prev]) / (percentages[suiv] - percentages[prev])) * (bcontourPoints.outer().at(suiv).get<0>() - bcontourPoints.outer().at(prev).get<0>()));
			P.set<1>(bcontourPoints.outer().at(prev).get<1>() + ((p - percentages[prev]) / (percentages[suiv] - percentages[prev])) * (bcontourPoints.outer().at(suiv).get<1>() - bcontourPoints.outer().at(prev).get<1>()));
		}
		else
		{
			P.set<0>(bcontourPoints.outer().at(prev).get<0>() + ((p - percentages[prev]) / (1 - percentages[prev])) * (bcontourPoints.outer().at(suiv).get<0>() - bcontourPoints.outer().at(prev).get<0>()));
			P.set<1>(bcontourPoints.outer().at(prev).get<1>() + ((p - percentages[prev]) / (1 - percentages[prev])) * (bcontourPoints.outer().at(suiv).get<1>() - bcontourPoints.outer().at(prev).get<1>()));
		}

		//placer le curseur à ce point
		bpt translation(P.get<0>() - cursorCenter.get<0>(), P.get<1>() - cursorCenter.get<1>());
		//boost::geometry::difference (P , cursorCenter, translation);
		//DBG("cursorCenter = " + (String)cursorCenter.get<0>() + " " + (String)cursorCenter.get<1>());
		//DBG("P = " + (String)P.get<0>() + " " + (String)P.get<1>());
		//if (translation.size() > 0)
		//{
			//DBG("translation = " + (String)translation.get<0>() + " " + (String)translation.get<1>());
		translation.set<0>(translation.get<0>() * (double)parentCanvas->getWidth());
		translation.set<1>(translation.get<1>() * (double)parentCanvas->getHeight());


		//DBG("translation in pixels = " + (String)translation.get<0>() + " " + (String)translation.get<1>());
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
	InteractivePolygon::CanvasResized(_parentCanvas);
	//cursor->CanvasResized(_parentCanvas);
	//lengthToPercent();
	if(showCursor)
		cursor->CanvasResized(_parentCanvas);
	setReadingPosition(pc);
	//cursor->CanvasResized(_parentCanvas);
	//DBG("cursorCenter : " + (String)(cursorCenter.x) + " " + (String)(cursorCenter.y));
	//DBG("CompletePolygon::CanvasResized");
}

AreaEventType CompletePolygon::TryMovePoint(const Point<double>& newLocation)
{
	AreaEventType areaEventType = EditablePolygon::TryMovePoint(newLocation);
	CanvasResized(this->parentCanvas);
	return areaEventType;
}

void CompletePolygon::setCursorVisible(bool isVisible)
{
	//cursor->CanvasResized(this->parentCanvas);
	showCursor = isVisible;
	
	CanvasResized(this->parentCanvas);
	DBG("showCursor = " + (String)showCursor);
	//setReadingPosition(0);
}