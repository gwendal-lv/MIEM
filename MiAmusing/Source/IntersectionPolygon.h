/*
  ==============================================================================

    IntersectionPolygon.h
    Created: 6 Feb 2018 8:49:07am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "DrawablePolygon.h"
#include "CompletePolygon.h"
#include "InteractivePolygon.h"

namespace Amusing
{
	/* IntersectionPolygon construct itself on base of two parents polygons*/
	class IntersectionPolygon : public Miam::InteractivePolygon
	{
	public:
		IntersectionPolygon(int64_t _Id, std::shared_ptr<CompletePolygon> m_parent1, std::shared_ptr<CompletePolygon> m_parent2, Colour _fillColour);

		virtual std::shared_ptr<IDrawableArea> Clone() override
		{
			auto clone = std::make_shared<IntersectionPolygon>(*this);
			clone->onCloned();
			return clone;
		}

		~IntersectionPolygon();

		virtual void Paint(Graphics& g) override;
		virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;

		void setSurfaceThreshold(double m_threshold);

		bool isChild(std::shared_ptr<CompletePolygon> m_parent);
		bool isEmpty();

		std::shared_ptr<CompletePolygon> getNearestParent(bpt location);
		std::shared_ptr<CompletePolygon> getOtherParent(std::shared_ptr<CompletePolygon> parentA);
		int getApexesCount(std::shared_ptr<CompletePolygon> parent);
		double getApexesAngle(std::shared_ptr<CompletePolygon> parent, int index);

	private:
		// parents of the polygon -> determine this polygon
		std::shared_ptr<CompletePolygon> parent1, parent2;

		// threshold to use the intersection (each piece must be bigger than the treshold)
		double threshold;

		// the intersection can lead to multiple "pieces" of polygon
		std::deque<bpolygon> inter;

		// angular position of apexes belonging to parentX 
		std::vector<double> parent1ApexesAngle;
		std::vector<double> parent2ApexesAngle;
	};

}