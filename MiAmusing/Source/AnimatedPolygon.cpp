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
	point.setX((int)bcontourPointsInPixels.outer().at(0).get<0>());
	point.setY((int)bcontourPointsInPixels.outer().at(0).get<1>());
	fromPt = 0;
	SetNameVisible(false);
	currentSommet = 0;
	
}

AnimatedPolygon::AnimatedPolygon(int64_t _Id, bpt _center, int pointsCount, float radius,
	Colour _fillColour, float _canvasRatio) :
	EditablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
	currentSommet = 0;
	SetNameVisible(false);
}

AnimatedPolygon::AnimatedPolygon(int64_t _Id,
	bpt _center, bpolygon& _contourPoints, Colour _fillColour) :
	EditablePolygon(_Id, _center, _contourPoints, _fillColour)
{
	point.setX((int)bcontourPointsInPixels.outer().at(0).get<0>());
	point.setY((int)bcontourPointsInPixels.outer().at(0).get<1>());
	fromPt = 0;
	
	currentSommet = 0;
	SetNameVisible(false);
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
	g.fillEllipse((float)bcontourPointsInPixels.outer().at(0).get<0>() - contourPointsRadius,
		          (float)bcontourPointsInPixels.outer().at(0).get<1>() - contourPointsRadius, 20.0f, 20.0f);
	
}

double AnimatedPolygon::GetNextAreaLength()
{
	double distance = 0;
	if ((currentSommet + 1) >= (int)bcontourPoints.outer().size())
		distance = 100 * boost::geometry::distance(bcontourPoints.outer().at(currentSommet), bcontourPoints.outer().at(0));//contourPoints[currentSommet].getDistanceFrom(contourPoints[0]) * 100;
	else
		distance = 100 * boost::geometry::distance(bcontourPoints.outer().at(currentSommet), bcontourPoints.outer().at(currentSommet + 1));

	currentSommet = (currentSommet + 1) % bcontourPoints.outer().size();

	return distance;
}
/*
void AnimatedPolygon::associateFollower(std::shared_ptr<Follower> newFollower)
{
	follower = newFollower;
}
*/
/*
std::shared_ptr<Follower> AnimatedPolygon::GetFollower()
{
	return follower;
}*/

Point<double> AnimatedPolygon::initiateFollower()
{
	return juce::Point<double>( bcontourPoints.outer().at(0).get<0>(),bcontourPoints.outer().at(0).get<1>());
}

bpt AnimatedPolygon::getCenter()
{
	return bcenterInPixels;
}


bpt AnimatedPolygon::initializePolygone(bpt currentCenter)
{
	currentPoint = 0;
	oldPositionPC = -1; // initialise en negatif pour etre sur qu'on ne passe pas un segment

	bpt initialTranslation;
	oldCenter = currentCenter;

	initialTranslation = bpt(0, 0); //bpt(bcontourPoints.outer().at(0).get<0>(), bcontourPoints.outer().at(0).get<1>()) - currentCenter;
	oldCenter = bcenter;

	oldCenterInPixels = bcenterInPixels;

	//initT.setXY(initialTranslation.getX()*parentCanvas->getWidth(), initialTranslation.getY()*parentCanvas->getHeight());

	return initialTranslation;
}

bpt AnimatedPolygon::getPosition(double positionPC)
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
	bpt newPosition( bcontourPointsInPixels.outer().at(currentPoint).get<0>() + (positionPC ) * (getPente(currentPoint).x),
		bcontourPointsInPixels.outer().at(currentPoint).get<1>() + (positionPC) * (getPente(currentPoint).y));
	//DBG("[newPosition] = " + (String)newPosition.getX() + " " + (String)newPosition.getY());
	//newPosition += contourPointsInPixels[currentPoint].;
	
	
	return newPosition;
}

