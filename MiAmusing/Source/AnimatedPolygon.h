/*
  ==============================================================================

    AnimatedPolygon.h
    Created: 23 Jan 2017 9:09:50am
    Author:  ayup1

  ==============================================================================
*/

#ifndef ANIMATEDPOLYGON_H_INCLUDED
#define ANIMATEDPOLYGON_H_INCLUDED

#include "EditablePolygon.h"
#include "JuceHeader.h"

namespace Amusing
{
	class Follower;

	class AnimatedPolygon : public Miam::EditablePolygon
		                    //public AnimatedAppComponent
	{
	public :
		AnimatedPolygon(int64_t _Id);
		AnimatedPolygon(int64_t _Id,
			Point<double> _center, int pointsCount, float radius,
			Colour _fillColour, float _canvasRatio = 1.47);
		AnimatedPolygon(int64_t _Id,
			Point<double> _center, std::vector<Point<double>>& _contourPoints,
			Colour _fillColour);

		~AnimatedPolygon();

		int GetContourSize()
		{
			//DBG((String)contourPointsInPixels.size());
			DBG((String)contourPoints.size());
			//DBG((String)contour);
			return contourPoints.size();
		}

		int GetHeight()
		{
			return centerInPixels.getY();
		}

		double GetAreteLength()
		{
			//DBG("GetAreteL");
			//DBG((String)contourPoints.size());
			return contourPoints[0].getDistanceFrom(contourPoints[1])*100;
		}

		Point<double> getPente(int P)
		{
			int after = (P + 1) % (contourPoints.size());
			return (contourPointsInPixels[after] - contourPointsInPixels[P]);
		}

		Point<double> initiateFollower();

		int currentSommet;
		double GetNextAreaLength();

		void Paint(Graphics& g);
		
		void associateFollower(std::shared_ptr<Follower> newFollower);
		std::shared_ptr<Follower> GetFollower();

		//void update();

	private :
		std::shared_ptr<Follower> follower; // a remplacer par un vector<Follower>
		double speed;
		int fromPt;
		Point<int> point;
	};
}



#endif  // ANIMATEDPOLYGON_H_INCLUDED
