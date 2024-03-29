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

#include "Cursors.h"
#include "CompletePolygon.h"
//#include <math.h>
#include <cmath>

using namespace Miam;
using namespace Amusing;

Cursor::Cursor(int64_t _Id) : Exciter(_Id, std::chrono::time_point<clock>())
{
	speed = 1;
	minimumSizePercentage = 0.01;
	maxAlpha = 64.0f / 128.0f;
}

Cursor::Cursor(int64_t _Id, bpt _center, double _r, Colour _fillColour, float _canvasRatio) :
	Exciter(_Id, std::chrono::time_point<clock>())//EditableEllipse(_Id, _center, _a,  _b, _fillColour, _canvasRatio)
{
	SetOpacityMode(OpacityMode::DependingOnExcitement);
	isFilled = true;
	center = _center;
	fillColour = _fillColour;
	a = _r;
	b = _r;
	SetIsRound(true);
	initCursorSize = a;
	cursorSize = initCursorSize;
	
	speed = 1;

	if (_canvasRatio > 1.0f) // ratio of an landscape-oriented window
	{
		xScale = 1.0f / _canvasRatio;
		yScale = 1.0f;
	}
	else // ratio of an portrait-oriented window
	{
		xScale = 1.0f;
		yScale = 1.0f*_canvasRatio;
	}
	contourPoints.clear();
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() + (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() + (b / 2)*yScale));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() - (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));

	SetIsAnimationSynchronized(false);
	minimumSizePercentage = 0.01;

	speedToSize[4.0] = 0.6;//2.0;
	speedToSize[3.0] = 0.73333;//1.666;
	speedToSize[2.0] = 0.866666;//1.3333;
	speedToSize[1.0] = 1.0;
	speedToSize[0.5] = 1.3333;
	speedToSize[1.0 / 3.0] = 1.6666;
	speedToSize[0.25] = 2.0;

	maxAlpha = 64.0f / 128.0f;
}

//Cursor::Cursor(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
//	Exciter(_Id)//EditableEllipse(_Id, _center, _a,  _b, _fillColour, _canvasRatio)
//{
//	a = _a;
//	b = _b;
//	fillColour = _fillColour;
//	if (_canvasRatio > 1.0f) // ratio of an landscape-oriented window
//	{
//		xScale = 1.0f / _canvasRatio;
//		yScale = 1.0f;
//	}
//	else // ratio of an portrait-oriented window
//	{
//		xScale = 1.0f;
//		yScale = 1.0f*_canvasRatio;
//	}
//	center = _center;
//
//
//
//	speed = 1;
//}

Cursor::~Cursor()
{
}

void Cursor::setSpeed(double m_speed)
{
	speed = m_speed;
	if (speed <= 4 && speed > 0.1)
	{
		double newCursorSize = 0.0;
		//if (speed >= 1)
		//	newCursorSize = 1.0 + (0.6 - 1.0) * (speed - 1.0) / (4.0 - 1.0); //1.0 + (speed - 1.0) * (2.0 - 1.0) / 3.0;
		//else
		//	newCursorSize = 2.0 + (1.0 - 2.0) * (speed - 0.25) / (1.0 - 0.25); //0.6 + (4 - (1.0 / speed)) * (1.0 - 0.6) / 3.0;
		if(speedToSize.find(speed)!=speedToSize.end())
			newCursorSize = speedToSize.at(speed);
		newCursorSize *= initCursorSize;
		//double newCursorSize = (double)initCursorSize / m_speed;
		double resize = newCursorSize / cursorSize;
		if (SizeChanged(resize, false))
		{
			cursorSize = newCursorSize;
			if (cursorSize > 0.5)
				setVerticesCount(64);
			else
				setVerticesCount(32);
		}
	}
}

void Cursor::setPosition(double m_position)
{
	position = m_position;
}

double Cursor::getSpeed()
{
	return speed;
}

bpt Cursor::getPosition()
{
	return center;//position;
}

bpt Cursor::getPositionInPixels()
{
	return bpt(center.get<0>() * parentCanvas->getWidth(), center.get<1>() * parentCanvas->getHeight());//centerInPixels;//position;
}

double Cursor::getPositionInAssociateArea()
{
	if (auto associateC = std::dynamic_pointer_cast<CompletePolygon>(associate))
	{
		return associateC->getLinearPercentage(center);
	}
	else
		return 0.0;
}

void Cursor::LinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon, bool rememberPreviousAssociate) // link the cursor to the form that compute its center position
{
	if (associate != m_Polygon)
	{
		if(associate == nullptr)
			associate = m_Polygon;
		else if (rememberPreviousAssociate &&  associate != nullptr && (oldAssociates.find(m_Polygon) == oldAssociates.end() || oldAssociates[m_Polygon].second > 0.05))
		{
			oldAssociates[associate].first = center;
			oldAssociates[associate].second = 0; // distance parcourue depuis que le curseur n'est plus li� � cette aire (= oldAssociate)
			associate = m_Polygon;
		}
		else
		{
			associate = m_Polygon;
		}
	}
}

bool Cursor::CanLinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon)
{
	if (oldAssociates.find(m_Polygon) == oldAssociates.end() || oldAssociates[m_Polygon].second > 0.05)
	{
		return true;
	}
	else
		return false;
}

void Cursor::Inhibit(std::shared_ptr<Miam::EditablePolygon> m_Polygon) // inhibit an area so the cursor won't go immediately on it
{
	oldAssociates[m_Polygon] = std::pair<bpt, double>(center, 0);
}

bool Cursor::isLinkedTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon)
{
	if (m_Polygon == associate)
		return true;
	else
		return false;
}

std::shared_ptr<Miam::EditablePolygon> Cursor::getAssociateArea()
{
	return associate;
}

bool Cursor::setReadingPosition(double p)
{
	if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(associate))
	{
		bpt P = complete->computeLinearCursorCenter(p); // nouveau centre
		
		// mise � jour de la liste des anciens associ� (distance et v�rification si on peut les supprimer)
		std::vector<int> positionToDelete;
		//std::map<std::shared_ptr<EditablePolygon>, std::pair<bpt, double>>::iterator it;
		auto it = oldAssociates.begin();
		while (it != oldAssociates.end())
		{
			it->second.second = boost::geometry::distance(P, it->second.first);
			if (it->second.second > 0.05)
			{
				it = oldAssociates.erase(it);
			}
			else
				it++;
		}
		//for (auto it = oldAssociates.begin(); it != oldAssociates.end(); it++)
		//{
		//	it->second.second = boost::geometry::distance(P, it->second.first);
		//	
		//	if (it->second.second > 0.05)
		//	{
		//		//std::map<std::shared_ptr<EditablePolygon>, std::pair<bpt, double>>::iterator newIt = it;
		//		it = oldAssociates.erase(it);
		//		//it = newIt;
		//	}
		//}


		//placer le curseur � ce point
		bpt translation(P.get<0>() - center.get<0>(), P.get<1>() - center.get<1>());
		translation.set<0>(translation.get<0>() * (double)parentCanvas->getWidth());
		translation.set<1>(translation.get<1>() * (double)parentCanvas->getHeight());

		Translate(juce::Point<double>(translation.get<0>(), translation.get<1>()));
		float newAlpha = complete->computeCursorAlpha(p,P,maxAlpha);
		
		if (newAlpha < 0)
			DBG("alpha n�gatif");
		SetAlpha(newAlpha);
#if !defined(OPENGL_RENDERING) || OPENGL_RENDERING == 0
		CanvasResized(parentCanvas);
#else
		RefreshOpenGLBuffers();
#endif // 

		
		
		//DBG((String)center.get<0>() + " " + (String)center.get<1>() + "setReadingPosition");
		return true;
	}
	return false;
}

void Cursor::setCenterPositionNormalize(bpt newCenter)
{
	Point<double> translation(newCenter.get<0>()*parentCanvas->getWidth() - centerInPixels.get<0>(), newCenter.get<1>()*parentCanvas->getHeight() - centerInPixels.get<1>()); // pixels
	Translate(translation);
	//DBG((String)center.get<0>() + " " + (String)center.get<1>() + "setCenterPositionNormal");
}

void Cursor::Paint(Graphics& g)
{
	if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(associate))
	{
		if(complete->isVisible())
			EditableEllipse::Paint(g);
	}
	else
		EditableEllipse::Paint(g);
}

bool Cursor::isClicked(const Point<double>& hitPoint)
{
	AreaEventType areaEventType = EditableEllipse::TryBeginPointMove(hitPoint);
	pointDraggedId = EditableAreaPointId::None;
	if (areaEventType != AreaEventType::PointDragBegins)
		return false;
	else
		return true;
}


