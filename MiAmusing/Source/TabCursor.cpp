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
#include "TabCursor.h"

#include "Exciter.h"

using namespace Miam;

TabCursor::TabCursor(bptree::ptree & areaTree, std::chrono::time_point<clock> commonStartTimePoint_, int additionnalTouchGrabRadius_) : 
	Exciter(areaTree,commonStartTimePoint_,additionnalTouchGrabRadius_)
{
	zone = Rectangle<int>(0, 0, 10000, 10000);
	numDivisions = 0;
	magnetized = false;
	initCursorSize = a;
	currentAreaResize = 1.0;
	allSizeEnabled = true;
	
	speedToSize[4.0] = 0.6;//2.0;
	speedToSize[3.0] = 0.73333;//1.666;
	speedToSize[2.0] = 0.866666;//1.3333;
	speedToSize[1.0] = 1.0;
	speedToSize[0.5] = 1.3333;
	speedToSize[1.0 / 3.0] = 1.6666;
	speedToSize[0.25] = 2.0;
}

TabCursor::TabCursor(uint64_t uniqueId, std::chrono::time_point<clock> commonStartTimePoint_, int additionnalTouchGrabRadius_) :
	Exciter::Exciter(uniqueId,commonStartTimePoint_,additionnalTouchGrabRadius_)
{
	zone = Rectangle<int>(0, 0, 10000, 10000);
	numDivisions = 0;
	magnetized = false;
	initCursorSize = a;
	currentAreaResize = 1.0;
	allSizeEnabled = true;

	speedToSize[4.0] = 0.6;//2.0;
	speedToSize[3.0] = 0.73333;//1.666;
	speedToSize[2.0] = 0.866666;//1.3333;
	speedToSize[1.0] = 1.0;
	speedToSize[0.5] = 1.3333;
	speedToSize[1.0 / 3.0] = 1.6666;
	speedToSize[0.25] = 2.0;
}

TabCursor::~TabCursor()
{
}

void TabCursor::setZone(Rectangle<int> _zone)
{
	zone = _zone;
}

AreaEventType TabCursor::TryBeginPointMove(const Point<double>& hitPoint)
{
	AreaEventType areaEventType = EditableEllipse::TryBeginPointMove(hitPoint);
	if (!allSizeEnabled)
	{
		if (pointDraggedId == EditableAreaPointId::WholeArea || pointDraggedId == EditableAreaPointId::Center)
		{
			areaEventType = AreaEventType::NothingHappened;
			pointDraggedId = EditableAreaPointId::None;
		}
	}
	return areaEventType;
}

AreaEventType TabCursor::TryMovePoint(const Point<double>& newLocation)
{
	if (allSizeEnabled)
	{
		if (zone.contains(newLocation.toInt()))
		{


			AreaEventType areaEventType = Exciter::TryMovePoint(newLocation);

			bpt newCenter = centerInPixels;
			if (zone.getHeight() > zone.getWidth())
				newCenter.set<0>(zone.getX() + zone.getWidth() / 2);
			else
				newCenter.set<1>(zone.getY() + zone.getHeight() / 2);



			setCenterPosition(newCenter);
#if defined(OPENGL_RENDERING) && (OPENGL_RENDERING == 1)
			RefreshOpenGLBuffers();
#endif
			return areaEventType;
		}
		else
			return AreaEventType::NothingHappened;
	}
	else
	{
		AreaEventType areaEventType = Exciter::TryMovePoint(newLocation);
#if defined(OPENGL_RENDERING) && (OPENGL_RENDERING == 1)
		RefreshOpenGLBuffers();
#endif
		return areaEventType;
	}
}

AreaEventType TabCursor::EndPointMove()
{
	if (magnetized && allSizeEnabled)
	{
		double maxDist = 1000.0;
		int nearest = 0;
		bpt newCenter = centerInPixels;
		if (zone.getHeight() > zone.getWidth())
		{
			int space = int(zone.getHeight() / numDivisions);
			for (int i = 0; i < numDivisions; ++i)
			{
				if (abs(space / 2 + (i * space - newCenter.get<1>()) < maxDist))
				{
					maxDist = abs(space / 2 + i * space - newCenter.get<1>());
					nearest = i;
				}
			}
			newCenter.set<1>(zone.getY() + (nearest * space) + space / 2);
		}
		else
		{
			int space = int(zone.getWidth() / numDivisions);
			for (int i = 0; i < numDivisions; ++i)
			{
				if (abs(space / 2 + (i * space) - newCenter.get<0>()) < maxDist)
				{
					maxDist = abs(space / 2 + (i * space) - newCenter.get<1>());
					nearest = i;
				}
			}
			newCenter.set<0>(zone.getX() + (nearest * space) + space / 2);
		}
		setCenterPosition(newCenter);
		CanvasResized(parentCanvas);
	}
	if (!allSizeEnabled)
	{
		bpt newCenter = centerInPixels;
		if (magnetized)
		{
			double maxDist = 1000.0;
			int nearest = 0;

			if (zone.getHeight() > zone.getWidth())
			{
				int space = int(zone.getHeight() / numDivisions);
				for (int i = 0; i < numDivisions; ++i)
				{
					if (abs(space / 2 + (i * space - newCenter.get<1>()) < maxDist))
					{
						maxDist = abs(space / 2 + i * space - newCenter.get<1>());
						nearest = i;
					}
				}
				newCenter.set<1>(zone.getY() + (nearest * space) + space / 2);
			}
			else
			{
				int space = int(zone.getWidth() / numDivisions);
				for (int i = 0; i < numDivisions; ++i)
				{
					if (abs(space / 2 + (i * space) - newCenter.get<0>()) < maxDist)
					{
						maxDist = abs(space / 2 + (i * space) - newCenter.get<1>());
						nearest = i;
					}
				}
				newCenter.set<0>(zone.getX() + (nearest * space) + space / 2);
			}
		}

		if (!allSizeEnabled && (pointDraggedId == EditableAreaPointId::ManipulationPoint || pointDraggedId > 0))
		{
			double currentSize = a;
			double currentResize = currentSize / initCursorSize;
			double maxDiff = 100000.0;
			double resizeToApply = 0; // ne restera jamais � 0 comme on prend un maxDiff assez grand
			for (int i = 0; i < 7; ++i)
			{
				if (abs(currentResize - currentAreaResize * authorizedSize[i]) < maxDiff)
				{
					resizeToApply = currentAreaResize * authorizedSize[i];
					maxDiff = abs(currentResize - currentAreaResize * authorizedSize[i]);
					currentSizeIdx = i;
				}
			}
			double effectiveResize = resizeToApply / currentResize;
			if (SizeChanged(effectiveResize, false))
			{
				if (a > 0.5)
					setVerticesCount(64);
				else
					setVerticesCount(32);
			}
		}

		setCenterPosition(newCenter);
		CanvasResized(parentCanvas);
#if defined(OPENGL_RENDERING) && (OPENGL_RENDERING == 1)
		RefreshOpenGLBuffers();
#endif
	}
	return Exciter::EndPointMove();
}

int TabCursor::getIndexValue()
{
	return 6 - currentSizeIdx;
}

double TabCursor::getPercentage()
{
	if (zone.getHeight() > zone.getWidth())
		return 1.0 - centerInPixels.get<1>() / (double)zone.getHeight();
	else
		return centerInPixels.get<0>() / (double)zone.getWidth();
}

void TabCursor::setPercentage(double pc)
{
	bpt newCenter = centerInPixels;
	if (zone.getHeight() > zone.getWidth())
		newCenter.set<1>(zone.getY() + (1.0 - pc) * (double)zone.getHeight());
	else
		newCenter.set<0>(zone.getX() + pc * (double)zone.getWidth());

	setCenterPosition(newCenter);

#if !defined(OPENGL_RENDERING) || OPENGL_RENDERING == 0
	CanvasResized(parentCanvas);
#else
	RefreshOpenGLBuffers();
#endif
}

void TabCursor::setIndexValue(int _idxValue)
{
	double newSize = currentAreaResize * authorizedSize[_idxValue];
	currentSizeIdx = _idxValue;
	SizeChanged(newSize, false);
	updateContourPoints();

#if !defined(OPENGL_RENDERING) || OPENGL_RENDERING == 0
	CanvasResized(parentCanvas);
#else
	RefreshOpenGLBuffers();
#endif
}

void TabCursor::SetSpeed(double _speed)
{
	// mettre un std::map<double, double> speedToSize;
	double sizeToReach = 1.0;
	if (speedToSize.find(_speed) != speedToSize.end())
	{
		sizeToReach = speedToSize[_speed];
	}
	sizeToReach *= initCursorSize;
	double resize = sizeToReach / a;
	SizeChanged(resize, false);
}

void TabCursor::SetNumDivisions(int _numDivisions)
{
	numDivisions = _numDivisions;
}

void TabCursor::EnableMagnet(bool _shouldBeMagnetized)
{
	magnetized = _shouldBeMagnetized;
}

int TabCursor::getNearestDivision()
{
	double maxDist = 1000.0;
	int nearest = 0;
	if (zone.getHeight() > zone.getWidth())
	{
		int space = int(zone.getHeight() / numDivisions);
		for (int i = 0; i < numDivisions; ++i)
		{
			if (abs(space / 2 + (i * space - centerInPixels.get<1>())) < maxDist)
			{
				maxDist = abs(space / 2 + i * space - centerInPixels.get<1>());
				nearest = i;
			}
		}
		return numDivisions-nearest - 1;
	}
	else
	{
		int space = int(zone.getWidth() / numDivisions);
		for (int i = 0; i < numDivisions; ++i)
		{
			if (abs(space / 2 + (i * space) - centerInPixels.get<0>()) < maxDist)
			{
				maxDist = abs(space / 2 + (i * space) - centerInPixels.get<1>());
				nearest = i;
			}
		}
		return nearest;
	}
}

void TabCursor::freeSize(bool m_freeSize)
{
	allSizeEnabled = m_freeSize;
	//enableTranslationOnly = false;
}

void TabCursor::SetCurrentSize(double _currentSize)
{
	currentAreaResize = _currentSize;
}
