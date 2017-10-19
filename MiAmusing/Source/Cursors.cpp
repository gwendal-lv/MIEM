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

Cursor::Cursor(int64_t _Id) : Exciter(_Id, std::chrono::time_point<clock>())
{
	speed = 1;
}

Cursor::Cursor(int64_t _Id, bpt _center, double _r, Colour _fillColour, float _canvasRatio) :
	Exciter(_Id, std::chrono::time_point<clock>())//EditableEllipse(_Id, _center, _a,  _b, _fillColour, _canvasRatio)
{
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
	double newCursorSize = (double)initCursorSize / m_speed;
	double resize = newCursorSize / cursorSize;
	if(SizeChanged(resize,false))
		cursorSize = newCursorSize;
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

void Cursor::LinkTo(std::shared_ptr<Miam::EditablePolygon> m_Polygon) // link the cursor to the form that compute its center position
{
	if (associate != m_Polygon)
	{
		if(associate == nullptr)
			associate = m_Polygon;
		else if (associate != nullptr && (oldAssociates.find(m_Polygon) == oldAssociates.end() || oldAssociates[m_Polygon].second > 0.05))
		{
			oldAssociates[associate].first = center;
			oldAssociates[associate].second = 0; // distance parcourue depuis que le curseur n'est plus lié à cette aire (= oldAssociate)
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
		
		// mise à jour de la liste des anciens associé (distance et vérification si on peut les supprimer)
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
	EditableEllipse::Paint(g);
}