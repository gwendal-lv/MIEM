/*
  ==============================================================================

    Follower.h
    Created: 6 Feb 2017 2:31:39pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef FOLLOWER_H_INCLUDED
#define FOLLOWER_H_INCLUDED

#include "EditableEllipse.h"

namespace Miam
{
	class DrawablePolygon;
	class MultiAreaEvent;
	class AmusingScene;
}

namespace Amusing
{
	
	class AnimatedPolygon;

	class Follower : public Miam::EditableEllipse,
		public std::enable_shared_from_this<Follower>
	{
		public :
			Follower(int64_t _Id, bpt _center, double _r, Colour _fillColour, float _canvasRatio, std::shared_ptr<Miam::InteractiveScene> m_masterScene);
			~Follower();
			virtual IDrawableArea* Clone() const override { return new Follower(*this); }

			std::shared_ptr<Miam::AreaEvent> setPosition(double m_position);
			std::shared_ptr<AnimatedPolygon> getCurrentPolygon();
			bool isLinkTo(std::shared_ptr<AnimatedPolygon> polygon);
		private :

			
			void initArea();
			void setCenter(Point<double> newCenter);
			void setGC(Point<double> m_GC);

			std::shared_ptr<Miam::InteractiveScene> masterScene;
			std::shared_ptr<AnimatedPolygon> masterArea;
			double positionPC; // position in procent
			int currentPoint;
			Point<double> GC;
			Point<double> position;

			bool first;
			Point<double> initTranslation;
	};
}



#endif  // FOLLOWER_H_INCLUDED
