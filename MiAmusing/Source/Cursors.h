/*
  ==============================================================================

    Cursors.h
    Created: 21 Aug 2017 2:04:21pm
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "EditableEllipse.h"
#include "EditablePolygon.h"

class Cursor : public Miam::EditableEllipse
{
public:
	Cursor(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio);
	~Cursor();

	void setSpeed(double m_speed);
	void setPosition(double m_position);
	double getSpeed();
	double getPosition();
	void LinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	bool isLinkedTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon);

private:
	double speed;
	double position;
	std::shared_ptr<Miam::EditablePolygon> associate;
};

