/*
  ==============================================================================

    Cursors.h
    Created: 21 Aug 2017 2:04:21pm
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "EditableEllipse.h"
#include "Exciter.h"
#include "EditablePolygon.h"

class Cursor : public Miam::Exciter//EditableEllipse//Exciter//EditableEllipse
{
public:
	Cursor(int64_t _Id);
	Cursor(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio);
	~Cursor();

	virtual IDrawableArea* Clone() const override { return new Cursor(*this); }

	void setSpeed(double m_speed);
	void setPosition(double m_position);
	double getSpeed();
	double getPosition();
	void LinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	bool isLinkedTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	void setReadingPosition(double p);

private:
	JUCE_LEAK_DETECTOR(Cursor)
	double speed;
	double position;
	std::shared_ptr<Miam::EditablePolygon> associate; // area that enforce the speed/give center position
};

