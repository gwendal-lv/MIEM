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
	showCursor = true;
	pc = 0;
	cursorCenter = contourPoints[0];
	cursor = std::shared_ptr<EditableEllipse>(new EditableEllipse(0, cursorCenter, 0.1f, 0.1f, Colours::grey, 1.47));
	cursor->SetNameVisible(false);
	//percentages.reserve(contourPoints.size());
	for (int i = 0; i < contourPoints.size(); ++i)
		percentages.push_back(0);
	DBG("percentages.size() : " + (String)percentages.size());
	SetNameVisible(false);
	

}

CompletePolygon::CompletePolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius,
	Colour _fillColour, float _canvasRatio) :
	EditablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
	showCursor = true;
	pc = 0;
	cursorCenter = contourPoints[0];
	cursor = std::shared_ptr<EditableEllipse>(new EditableEllipse(0, cursorCenter, 0.1f, 0.1f, Colours::grey, _canvasRatio));
	cursor->SetNameVisible(false);
	for (int i = 0; i < contourPoints.size(); ++i)
		percentages.push_back(0);
	percentages.reserve(contourPoints.size());
	SetNameVisible(false);
}

CompletePolygon::CompletePolygon(int64_t _Id,
	Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour) :
	EditablePolygon(_Id, _center, _contourPoints, _fillColour)
{
	showCursor = true;
	pc = 0;
	cursorCenter = contourPoints[0];
	cursor = std::shared_ptr<EditableEllipse>(new EditableEllipse(0, cursorCenter, 0.1f, 0.1f, Colours::grey, 1.47));
	cursor->SetNameVisible(false);
	for (int i = 0; i < contourPoints.size(); ++i)
		percentages.push_back(0);
	percentages.reserve(contourPoints.size());
	SetNameVisible(false);
}



CompletePolygon::~CompletePolygon()
{

}

void CompletePolygon::Paint(Graphics& g)
{
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
	g.fillEllipse(contourPointsInPixels[0].getX() - contourPointsRadius,
		contourPointsInPixels[0].getY() - contourPointsRadius, 20, 20);
	cursor->Paint(g);
}

void CompletePolygon::lengthToPercent()
{
	//calcul du perimetre
	perimeter = 0;
	for (int i = 0; i < contourPoints.size() - 1; ++i)
	{
		
		perimeter += contourPoints[i + 1].getDistanceFrom(contourPoints[i]);
		percentages[i + 1] = perimeter;
	}
	perimeter += contourPoints[contourPoints.size() - 1].getDistanceFrom(contourPoints[0]);

	//calcul des poucentages correspondant à chaque point
	//DBG("-----------------");
	for (int i = 0; i < percentages.size(); ++i)
	{
		percentages[i] = percentages[i] / perimeter;
		//DBG((String)percentages[i]);
	}
	//DBG("-----------------");
}

void CompletePolygon::setReadingPosition(double p)
{
	pc = p;
	lengthToPercent();
	//determiner entre quels points va se trouver le curseur
	int prev = 0;
	int suiv = 0;
	//DBG("percentages.size()" + (String)(percentages.size()));
	for (int i = 0; i < percentages.size(); ++i)
	{
		//DBG((String)i);
		if (p < percentages[i])
		{
			prev = i-1;
			suiv = i;
			//DBG("!!! " + (String)p + " > " + (String)percentages[i]);
			//DBG("stop");
			break;
		}
		else
		{
			prev = i;
			suiv = (i + 1) % percentages.size();
			//DBG((String)p + " < " + (String)percentages[i]);
		}
	}

	//DBG("prev = " + (String)prev);
	//DBG("suiv = " + (String)suiv);
	//calculer la position du curseur entre ces points
	Point<double> P;
	if(suiv != 0)
		P = contourPoints[prev] + ((p - percentages[prev]) / (percentages[suiv] - percentages[prev])) * (contourPoints[suiv] - contourPoints[prev]);
	else
		P = contourPoints[prev] + ((p - percentages[prev]) / (1 - percentages[prev])) * (contourPoints[suiv] - contourPoints[prev]);

	//placer le curseur à ce point
	Point<double> translation = (P - cursorCenter);
	translation.setXY(translation.getX() * (double)parentCanvas->getWidth(), translation.getY() * (double)parentCanvas->getHeight());
	//DBG("cursorCenter = " + (String)cursorCenter.x + " " + (String)cursorCenter.y);
	//DBG("P = " + (String)P.x + " " + (String)P.y);
	//DBG("translation = " + (String)translation.x + " " + (String)translation.y);
	cursor->Translate(translation);
	cursorCenter = P;
	//redessiner
	cursor->CanvasResized(this->parentCanvas);
	
}

void CompletePolygon::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
	InteractivePolygon::CanvasResized(_parentCanvas);
	//cursor->CanvasResized(_parentCanvas);
	//lengthToPercent();
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
