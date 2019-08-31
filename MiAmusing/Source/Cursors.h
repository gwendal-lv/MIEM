/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
	void setMaxAlpha(float _a) { maxAlpha = _a; }

private:
	JUCE_LEAK_DETECTOR(Cursor)
	double speed;
	double position;
	double initCursorSize;
	double cursorSize;
	std::shared_ptr<Miam::EditablePolygon> associate; // aire que l'on parcourt
	std::map<std::shared_ptr<Miam::EditablePolygon>,std::pair<bpt, double>> oldAssociates; // retient les aires par lesquels ont est d�j� pass�

	float maxAlpha;
	std::map<double, double> speedToSize;
};

