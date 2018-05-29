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
}

TabCursor::TabCursor(uint64_t uniqueId, std::chrono::time_point<clock> commonStartTimePoint_, int additionnalTouchGrabRadius_) :
	Exciter::Exciter(uniqueId,commonStartTimePoint_,additionnalTouchGrabRadius_)
{
	zone = Rectangle<int>(0, 0, 10000, 10000);
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
