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
#include "InteractiveScene.h"
#include "MiamMath.h"


using namespace Amusing;
using namespace Miam;

Follower::Follower(int64_t _Id, Point<double> _center, double _r, Colour _fillColour, float _canvasRatio, std::shared_ptr<InteractiveScene> m_masterScene)
	: EditableEllipse(_Id,_center,_r,_r,_fillColour,_canvasRatio),
	masterScene(m_masterScene),
	positionPC(0), currentPoint(0)
{
	//first = true;
	initArea();
	SetNameVisible(false);
	initTranslation = masterArea->initializePolygone(center);
	position = masterArea->initiateFollower();
	
	//DBG("trX = " + (String)initTranslation.getX());
	//DBG("trY = " + (String)initTranslation.getY());
	first = true;
	//Translate(translation);
	
	//CanvasResized(this->parentCanvas);
}

Follower::~Follower()
{

}

// faire passer tout le calcul de la position dans la forme Animated
// --> le follower ne sert qu'a detecter la collision et prendre la decision lors de collision
std::shared_ptr<Miam::AreaEvent> Follower::setPosition(double m_position)
{
	
	/*
	if (currentPoint == 0 && positionPC == 0)
	{
		setCenter(masterArea->initiateFollower());
		GC = masterArea->getCenter();
	}
	else
		setGC(masterArea->getCenter());
	
	double oldPosition = positionPC;
	
	positionPC = m_position;

	if (oldPosition > positionPC) // on a recommence le son
	{
		oldPosition = 0;
		currentPoint = (currentPoint+1)%masterArea->GetContourSize();
		DBG("on a recommence ######### : " + (String)currentPoint);
	}
	
	//if(pas de collision avec polygones) --> ok*/
	/*
	DBG("[position] = " + (String)position.getX() + " " + (String)position.getY());
	Point<double> newPosition =  masterArea->getPosition(m_position);


	Point<double> translation = newPosition - position;
	position = newPosition;
	Rectangle<double> boundingBoxContour = contour.getBounds().toDouble();
	boundingBoxContour.translate(translation.getX(), translation.getY());
	
	DBG("[translation] = " + (String)translation.getX() + " " + (String)translation.getY());

	if (first == true)
	{
		initTranslation.setXY(initTranslation.getX()*parentCanvas->getWidth(), initTranslation.getY()*parentCanvas->getHeight());
		translation += initTranslation;
		first = false;
	}

	Translate(translation);
	//Translate(initTranslation);
	*/

	/*
	if (first == true)
	{
		//initTranslation.setXY(initTranslation.getX()*parentCanvas->getWidth(), initTranslation.getY()*parentCanvas->getHeight());
		//translation += initTranslation;
		DBG("position = " + (String)position.getX());
		setCenterPositionInPixels(position);
		first = false;
		DBG("follower : " + (String)parentCanvas->getWidth() + " x " + (String)parentCanvas->getHeight());
	}

	Point<double> position2 = masterArea->getPosition(m_position);
	//position.setXY(position.getX()*((double)parentCanvas->getWidth()), position.getY() * ((double)parentCanvas->getHeight()));
	setCenterPositionInPixels(position2);
	*/
	//masterArea->getPosition(m_position);
	//setCenterPosition(Point<double>(320.4, 178));

	Point<double> newPosition = masterArea->getPosition(m_position);
	/*
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(masterScene))
	{
		Point<double> interPoint = amusingScene->hitPolygon(newPosition, masterArea, masterArea->getCurrentPoint(), masterArea->getAfterPoint());
		if (  interPoint != Point<double>(0,0))
		{
			DBG("!!!!!!!!!!!!! collision en " + (String)interPoint.getX() + " " + (String)interPoint.getY() +"!!!!!!!!!");
			
			
			
		}
	}*/
	setCenterPosition(newPosition);
	
	

	std::shared_ptr<AreaEvent> areaE(new AreaEvent(shared_from_this(), AreaEventType::Translation));
	CanvasResized(this->parentCanvas);
	return areaE;

}

void Follower::initArea()
{
	if (auto masterSceneAmusing = std::dynamic_pointer_cast<AmusingScene>(masterScene))
	{
		//masterArea = masterSceneAmusing->getFirstArea();
		masterArea = masterSceneAmusing->getNextArea();
	}
	else
		DBG("pas amusing scene");
	if (masterArea == nullptr)
		DBG("NULLPTR");
}

void Follower::setCenter(Point<double> newCenter)
{
	DBG("[newCenter] = " + (String)newCenter.getX() + " " + (String)newCenter.getY());
	DBG("[center] = " + (String)center.getX() + " " + (String)center.getY());
	DBG("[centerInPixels] = " + (String)centerInPixels.getX() + " " + (String)centerInPixels.getY());

	
	Point<double> translation = (newCenter - center);
	translation.setXY(translation.getX() * (double)parentCanvas->getWidth(),translation.getY() * (double)parentCanvas->getHeight());
	DBG("[translation] = " + (String)translation.getX() + " " + (String)translation.getY());
	Translate(translation);
	DBG("[0] = " + (String)contourPoints[0].getX() + " " + (String)contourPoints[0].getY());
	CanvasResized(this->parentCanvas);
	
}

std::shared_ptr<AnimatedPolygon> Follower::getCurrentPolygon()
{
	return masterArea;
}

void Follower::setGC(Point<double> newGC)
{
	if (GC != newGC)
	{
		Point<double> translation = (newGC - GC);
		Translate(translation);
		GC = newGC;
	}
}

bool Follower::isLinkTo(std::shared_ptr<AnimatedPolygon> polygon)
{
	if (masterArea == polygon)
		return true;
}