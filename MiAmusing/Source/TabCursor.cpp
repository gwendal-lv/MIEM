/*
  ==============================================================================

    TabCursor.cpp
    Created: 23 May 2018 11:14:24am
    Author:  ayup1

  ==============================================================================
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
}

TabCursor::TabCursor(uint64_t uniqueId, std::chrono::time_point<clock> commonStartTimePoint_, int additionnalTouchGrabRadius_) :
	Exciter::Exciter(uniqueId,commonStartTimePoint_,additionnalTouchGrabRadius_)
{
	zone = Rectangle<int>(0, 0, 10000, 10000);
	numDivisions = 0;
	magnetized = false;
	initCursorSize = a;
	currentAreaResize = 1.0;
}

TabCursor::~TabCursor()
{
}

void TabCursor::setZone(Rectangle<int> _zone)
{
	zone = _zone;
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

			return areaEventType;
		}
		else
			return AreaEventType::NothingHappened;
	}
	else
	{
		return Exciter::TryMovePoint(newLocation);
	}
}

AreaEventType TabCursor::EndPointMove()
{
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

		if (!allSizeEnabled && pointDraggedId == EditableAreaPointId::ManipulationPoint)
		{
			double currentSize = a;
			double currentResize = currentSize / initCursorSize;
			double maxDiff = 100000.0;
			double resizeToApply;
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
	}
	return Exciter::EndPointMove();
}

int TabCursor::getIndexValue()
{
	return currentSizeIdx;
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
	CanvasResized(parentCanvas);
}

void TabCursor::setIndexValue(int _idxValue)
{
	double newSize = currentAreaResize * authorizedSize[_idxValue];
	currentSizeIdx = _idxValue;
	SizeChanged(newSize, false);
	updateContourPoints();
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
