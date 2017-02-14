/*
  ==============================================================================

    AnimatedPolygon.cpp
    Created: 23 Jan 2017 9:09:50am
    Author:  ayup1

  ==============================================================================
*/

#include "AnimatedPolygon.h"
#include "SceneCanvasComponent.h"

using namespace Amusing;
using namespace Miam;

AnimatedPolygon::AnimatedPolygon(int64_t _Id) : EditablePolygon(_Id), first(true)
{
	speed = 5; // 5 pixels/sec
	point.setX(contourPointsInPixels[0].getX());
	point.setY(contourPointsInPixels[0].getY());
	fromPt = 0;
	
	currentSommet = 0;
	
}

AnimatedPolygon::AnimatedPolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius,
	Colour _fillColour, float _canvasRatio) :
	EditablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
	currentSommet = 0;
	
}

AnimatedPolygon::AnimatedPolygon(int64_t _Id,
	Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour) :
	EditablePolygon(_Id, _center, _contourPoints, _fillColour)
{
	point.setX(contourPointsInPixels[0].getX());
	point.setY(contourPointsInPixels[0].getY());
	fromPt = 0;
	
	currentSommet = 0;
	
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

double AnimatedPolygon::GetNextAreaLength()
{
	double distance = 0;
	if((currentSommet+1)>=contourPoints.size())
		distance = contourPoints[currentSommet].getDistanceFrom(contourPoints[0]) * 100;
	else
		distance = contourPoints[currentSommet].getDistanceFrom(contourPoints[currentSommet+1]) * 100;

	currentSommet = (currentSommet + 1) % contourPoints.size();

	return distance;
}

void AnimatedPolygon::associateFollower(std::shared_ptr<Follower> newFollower)
{
	follower = newFollower;
}

std::shared_ptr<Follower> AnimatedPolygon::GetFollower()
{
	return follower;
}

Point<double> AnimatedPolygon::initiateFollower()
{
	return contourPoints[0];
}

Point<double> AnimatedPolygon::getCenter()
{
	return centerInPixels;
}


Point<double> AnimatedPolygon::initializePolygone(Point<double> currentCenter)
{
	currentPoint = 0;
	oldPositionPC = -1; // initialise en negatif pour etre sur qu'on ne passe pas un segment

	Point<double> initialTranslation;
	oldCenter = currentCenter;

	initialTranslation = contourPoints[0] - currentCenter ;
	oldCenter = center;

	oldCenterInPixels = centerInPixels;

	initT.setXY(initialTranslation.getX()*parentCanvas->getWidth(), initialTranslation.getY()*parentCanvas->getHeight());

	return initialTranslation;
}

Point<double> AnimatedPolygon::getPosition(double positionPC)
{
	

	if (oldPositionPC > positionPC) // on a recommence le son
	{
		oldPositionPC = 0;
		currentPoint = (currentPoint + 1) % GetContourSize();
		DBG("on a recommence ######### : " + (String)currentPoint);
	}
	else
		oldPositionPC = positionPC;
	//DBG("P = " + (String)currentPoint);
	//DBG("[currentPoint] = " + (String)contourPointsInPixels[currentPoint].getX() + " " + (String)contourPointsInPixels[currentPoint].getY());
	Point<double> newPosition = contourPointsInPixels[currentPoint] + (positionPC ) * (getPente(currentPoint));
	//DBG("[newPosition] = " + (String)newPosition.getX() + " " + (String)newPosition.getY());
	//newPosition += contourPointsInPixels[currentPoint].;
	
	
	return newPosition;
}

