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


using namespace Amusing;
using namespace Miam;

Follower::Follower(int64_t _Id, Point<double> _center, double _r, Colour _fillColour, float _canvasRatio, std::shared_ptr<AmusingScene> m_masterScene)
	: EditableEllipse(_Id,_center,_r,_r,_fillColour,_canvasRatio),
	masterScene(m_masterScene),
	positionPC(0), currentPoint(0)
{
	initArea();
	//SetNameVisible(false);
	//SetEnableTranslationOnly(true);
	//SetActive(true);
	
}

Follower::~Follower()
{

}

std::shared_ptr<Miam::MultiAreaEvent> Follower::setPosition(double m_position)
{
	DBG("ici");
	DBG((String)positionPC);
	double oldPosition = positionPC;
	DBG((String)oldPosition + " " + (String)positionPC);
	positionPC = m_position;
	
	//if(pas de collision avec polygones) --> ok
	Point<double> translation = (positionPC - oldPosition) * masterArea->getPente(currentPoint);
	Translate(translation);
	std::shared_ptr<MultiAreaEvent> areaE;
	return areaE;

}

void Follower::initArea()
{
	masterArea =  masterScene->getFirstArea();
	//masterArea->associateFollower(shared_from_this());
}

std::shared_ptr<AnimatedPolygon> Follower::getCurrentPolygon()
{
	return masterArea;
}