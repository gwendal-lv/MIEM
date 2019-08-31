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

		virtual void RefreshOpenGLBuffers() override;

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