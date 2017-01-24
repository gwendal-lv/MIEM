/*
  ==============================================================================

    AnimatedPolygon.cpp
    Created: 23 Jan 2017 9:09:50am
    Author:  ayup1

  ==============================================================================
*/

#include "AnimatedPolygon.h"

using namespace Amusing;
using namespace Miam;

AnimatedPolygon::AnimatedPolygon(int64_t _Id) : EditablePolygon(_Id)
{
	speed = 5; // 5 pixels/sec
	point.setX(contourPointsInPixels[0].getX());
	point.setY(contourPointsInPixels[0].getY());
	fromPt = 0;
	//setFramesPerSecond(24);
	startTimer(500);
	
}

AnimatedPolygon::AnimatedPolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius,
	Colour _fillColour, float _canvasRatio) :
	EditablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
	//setFramesPerSecond(24);
	startTimer(500);
}

AnimatedPolygon::AnimatedPolygon(int64_t _Id,
	Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour) :
	EditablePolygon(_Id, _center, _contourPoints, _fillColour)
{
	point.setX(contourPointsInPixels[0].getX());
	point.setY(contourPointsInPixels[0].getY());
	fromPt = 0;
	//setFramesPerSecond(24);
	startTimer(500);
}



AnimatedPolygon::~AnimatedPolygon()
{

}

void AnimatedPolygon::Paint(Graphics& g)
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
	
}

/*void AnimatedPolygon::update()
{
	repaint();
	//if(isActive)
	DBG("salut");
}*/

void AnimatedPolygon::timerCallback()
{
	//Point<double> diff = contourPoints[fromPt + 1] - contourPoints[fromPt];
}

