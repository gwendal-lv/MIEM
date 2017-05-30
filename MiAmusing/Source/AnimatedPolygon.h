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

#include "boost\geometry.hpp"
#include "boost\geometry\geometries\geometries.hpp"
#include "boost\geometry\geometries\polygon.hpp"

typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;
typedef boost::geometry::model::polygon<bpt> bpolygon;


namespace Amusing
{
	class Follower;

	class AnimatedPolygon : public Miam::EditablePolygon
		                    //public AnimatedAppComponent
	{
	public :
		AnimatedPolygon(int64_t _Id);
		AnimatedPolygon(int64_t _Id,
			bpt _center, int pointsCount, float radius,
			Colour _fillColour, float _canvasRatio = 1.47);
		AnimatedPolygon(int64_t _Id,
			bpt _center, bpolygon& _contourPoints,
			Colour _fillColour);

		~AnimatedPolygon();

		int GetContourSize()
		{
			//DBG((String)contourPointsInPixels.size());
			//DBG((String)contourPoints.size());
			//DBG((String)contour);
			return contourPoints.outer().size();
		}

		int GetHeight()
		{
			return (int)centerInPixels.get<1>();
		}

		double GetAreteLength()
		{
			//DBG("GetAreteL");
			//DBG((String)contourPoints.size());
			return 100 * boost::geometry::distance(contourPoints.outer().at(0), contourPoints.outer().at(1));//contourPoints[0].getDistanceFrom(contourPoints[1])*100;
		}

		Point<double> getPente(int P)
		{
			int after = (P + 1) % (contourPoints.outer().size());
			std::vector<bpt> out;
			boost::geometry::difference(contourPointsInPixels.outer().at(after), contourPointsInPixels.outer().at(P), out);
			return juce::Point<double>(out.front().get<0>(), out.front().get<1>());
		}

		bpt getCenter();

		Point<double> initiateFollower();

		int currentSommet;
		double GetNextAreaLength();

		void Paint(Graphics& g);
		
		//void associateFollower(std::shared_ptr<Follower> newFollower);
		//std::shared_ptr<Follower> GetFollower();

		bpt initializePolygone(bpt currentCenter);
		bpt getPosition(double positionPC);
		


		//void update();

	private :
		
		//std::shared_ptr<Follower> follower; // a remplacer par un vector<Follower>
		double speed;
		int fromPt;
		Point<int> point;

		bool first;
		double oldPositionPC;
		int currentPoint;
		bpt oldCenter;
		bpt oldCenterInPixels;

		Point<double> initT;
	};
}



#endif  // ANIMATEDPOLYGON_H_INCLUDED
