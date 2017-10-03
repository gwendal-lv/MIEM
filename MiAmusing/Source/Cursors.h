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
	bpt getPosition();
	bpt getPositionInPixels();
	double getPositionInAssociateArea();
	void LinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	bool isLinkedTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	bool CanLinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	void Inhibit(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	std::shared_ptr<Miam::EditablePolygon> getAssociateArea();
	bool setReadingPosition(double p);
	void setCenterPositionNormalize(bpt newCenter);

private:
	JUCE_LEAK_DETECTOR(Cursor)
	double speed;
	double position;
	double initCursorSize;
	double cursorSize;
	std::shared_ptr<Miam::EditablePolygon> associate; // aire que l'on parcourt
	std::map<std::shared_ptr<Miam::EditablePolygon>,std::pair<bpt, double>> oldAssociates; // retient les aires par lesquels ont est déjà passé
};

