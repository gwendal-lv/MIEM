/*
  ==============================================================================

    Cursors.cpp
    Created: 21 Aug 2017 2:04:21pm
    Author:  ayup1

  ==============================================================================
*/

#include "Cursors.h"

using namespace Miam;

Cursor::Cursor(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
	EditableEllipse(_Id, _center, _a,  _b, _fillColour, _canvasRatio)
{
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