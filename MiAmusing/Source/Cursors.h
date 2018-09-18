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
#include <memory>

class Cursor : public Miam::Exciter, public std::enable_shared_from_this<Cursor> //EditableEllipse//Exciter//EditableEllipse
{
public:
	Cursor(int64_t _Id);
	Cursor(int64_t _Id, bpt _center, double _r, Colour _fillColour, float _canvasRatio);
	~Cursor();

	virtual std::shared_ptr<IDrawableArea> Clone() override
	{
		auto clone = std::make_shared<Cursor>(*this);
		clone->onCloned();
		return clone;
	}

	void setSpeed(double m_speed);
	void setPosition(double m_position);
	double getSpeed();
	bpt getPosition();
	bpt getPositionInPixels();
	double getPositionInAssociateArea();
	void LinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon, bool rememberPreviousAssociate);
	bool isLinkedTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	bool CanLinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	void Inhibit(std::shared_ptr<Miam::EditablePolygon> m_Polygon);
	std::shared_ptr<Miam::EditablePolygon> getAssociateArea();
	bool setReadingPosition(double p);
	void setCenterPositionNormalize(bpt newCenter);

	void Paint(Graphics & g) override;
	bool isClicked(const Point<double>& hitPoint);
	

private:
	JUCE_LEAK_DETECTOR(Cursor)
	double speed;
	double position;
	double initCursorSize;
	double cursorSize;
	std::shared_ptr<Miam::EditablePolygon> associate; // aire que l'on parcourt
	std::map<std::shared_ptr<Miam::EditablePolygon>,std::pair<bpt, double>> oldAssociates; // retient les aires par lesquels ont est d�j� pass�
};

