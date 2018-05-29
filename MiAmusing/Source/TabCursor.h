/*
  ==============================================================================

    TabCursor.h
    Created: 23 May 2018 11:14:24am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "Exciter.h"

class TabCursor : public Miam::Exciter
{
	public :
		TabCursor(bptree::ptree & areaTree,
			std::chrono::time_point<clock> commonStartTimePoint_,
			int additionnalTouchGrabRadius_ = AdditionnalGrabRadius::None);
		TabCursor(uint64_t uniqueId,
			std::chrono::time_point<clock> commonStartTimePoint_,
			int additionnalTouchGrabRadius_ = AdditionnalGrabRadius::None);

		~TabCursor();

		virtual std::shared_ptr<IDrawableArea> Clone() override
		{
			auto clone = std::make_shared<TabCursor>(*this);
			clone->onCloned();
			return clone;
		}

		void setZone(Rectangle<int> _zone);

		Miam::AreaEventType TryMovePoint(const Point<double>& newLocation);

		double getPercentage();

		void setPercentage(double pc);

	private :
		Rectangle<int> zone;

};