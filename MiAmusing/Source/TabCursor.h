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

		Miam::AreaEventType TryBeginPointMove(const Point<double>& hitPoint) override;
		Miam::AreaEventType TryMovePoint(const Point<double>& newLocation);
		Miam::AreaEventType EndPointMove();

		double getPercentage();

		void setPercentage(double pc);

		void SetNumDivisions(int _numDivisions);

		void EnableMagnet(bool _shouldBeMagnetized);

		int getNearestDivision();

		void freeSize(bool m_sizeIsFree);

		void SetCurrentSize(double _currentSize);

		int getIndexValue();

		void setIndexValue(int idxValue);

	private :
		Rectangle<int> zone;
		int numDivisions;
		bool magnetized;
		bool allSizeEnabled;
		double initCursorSize;
		double authorizedSize[7] = {0.6, 0.73333, 0.866666, 1.0, 1.3333, 1.6666, 2.0/* 1.0 / 4.0, 1.0 / 3.0, 0.5, 1.0, 2.0, 3.0, 4.0 */};
		double currentAreaResize;
		int currentSizeIdx;
};