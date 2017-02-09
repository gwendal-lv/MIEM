/*
  ==============================================================================

    Follower.cpp
    Created: 6 Feb 2017 2:31:39pm
    Author:  ayup1

  ==============================================================================
*/

#include "Follower.h"

#include "AnimatedPolygon.h"

#include "AmusingScene.h"

#include "MultiAreaEvent.h"

#include "SceneCanvasComponent.h"


using namespace Amusing;
using namespace Miam;

Follower::Follower(int64_t _Id, Point<double> _center, double _r, Colour _fillColour, float _canvasRatio, std::shared_ptr<AmusingScene> m_masterScene)
	: EditableEllipse(_Id,_center,_r,_r,_fillColour,_canvasRatio),
	masterScene(m_masterScene),
	positionPC(0), currentPoint(0)
{
	first = true;
	initArea();
	SetNameVisible(false);
	//SetEnableTranslationOnly(true);
	//SetActive(true);
	
}

Follower::~Follower()
{

}

std::shared_ptr<Miam::AreaEvent> Follower::setPosition(double m_position)
{
	DBG("[0] = " + (String)contourPoints[0].getX() + " " + (String)contourPoints[0].getY());
	DBG("[1] = " + (String)contourPoints[1].getX() + " " + (String)contourPoints[1].getY());
	DBG("[0]pix = " + (String)contourPointsInPixels[0].getX() + " " + (String)contourPoints[0].getY());
	DBG("[1]pix = " + (String)contourPointsInPixels[1].getX() + " " + (String)contourPoints[1].getY());

	//DBG("ici");
	if(currentPoint == 0 && positionPC == 0)
		setCenter(masterArea->initiateFollower());
	//DBG((String)positionPC);
	double oldPosition = positionPC;
	//DBG((String)oldPosition + " " + (String)m_position);
	positionPC = m_position;

	if (oldPosition > positionPC) // on a recommence le son
	{
		oldPosition = 0;
		currentPoint = (currentPoint+1)%masterArea->GetContourSize();
		DBG("on a recommence ######### : " + (String)currentPoint);
	}
	
	//if(pas de collision avec polygones) --> ok
	Point<double> translation = (positionPC - oldPosition) * (masterArea->getPente(currentPoint));
	Rectangle<double> boundingBoxContour = contour.getBounds().toDouble();
	boundingBoxContour.translate(translation.getX(), translation.getY());
	//translation.setXY(translation.getX() * (double)parentCanvas->getWidth()
	//	, translation.getY()*(double)parentCanvas->getHeight());
	//DBG("translation = " + (String)translation.getX() + " " + (String)translation.getY());
	Translate(translation);
	std::shared_ptr<AreaEvent> areaE(new AreaEvent(shared_from_this(), AreaEventType::Translation));
	return areaE;

}

void Follower::initArea()
{
	masterArea = masterScene->getFirstArea();
	
}

void Follower::setCenter(Point<double> newCenter)
{
	Point<double> translation = (newCenter - center);
	Translate(translation);
	
}

std::shared_ptr<AnimatedPolygon> Follower::getCurrentPolygon()
{
	return masterArea;
}