/*
  ==============================================================================

    Cursors.cpp
    Created: 21 Aug 2017 2:04:21pm
    Author:  ayup1

  ==============================================================================
*/

#include "Cursors.h"
#include "CompletePolygon.h"
//#include <math.h>
#include <cmath>

using namespace Miam;
using namespace Amusing;

Cursor::Cursor(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
	EditableEllipse(_Id, _center, _a,  _b, _fillColour, _canvasRatio)
{
	speed = 1;
}

Cursor::~Cursor()
{
}

void Cursor::setSpeed(double m_speed)
{
	speed = m_speed;
}

void Cursor::setPosition(double m_position)
{
	position = m_position;
}

double Cursor::getSpeed()
{
	return speed;
}

double Cursor::getPosition()
{
	return position;
}

void Cursor::LinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon)
{
	associate = m_Polygon;
}

bool Cursor::isLinkedTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon)
{
	if (m_Polygon == associate)
		return false;
	else
		return true;
}

void Cursor::setReadingPosition(double p)
{
	if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(associate))
	{
		bpt P = complete->computeCursorCenter(p); // nouveau centre
		
		//placer le curseur à ce point
		bpt translation(P.get<0>() - center.get<0>(), P.get<1>() - center.get<1>());
		translation.set<0>(translation.get<0>() * (double)parentCanvas->getWidth());
		translation.set<1>(translation.get<1>() * (double)parentCanvas->getHeight());

		Translate(juce::Point<double>(translation.get<0>(), translation.get<1>()));
		float newAlpha = complete->computeCursorAlpha(p,P);
		
		if (newAlpha < 0)
			DBG("alpha négatif");
		SetAlpha(newAlpha);
		CanvasResized(parentCanvas);
	}
}