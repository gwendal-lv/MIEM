/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
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

#ifndef INTERACTIVEPOLYGON_H_INCLUDED
#define INTERACTIVEPOLYGON_H_INCLUDED


#include "DrawablePolygon.h"
#include "InteractiveArea.h"

#include "SubTriangle.h"


namespace Miam
{
    
    /// \brief A polygonal area with interactive abilities.
    class InteractivePolygon : public DrawablePolygon, public InteractiveArea
    {
        // ================== ATTRIBUTES ====================
        protected :
        std::vector<SubTriangle> subTriangles;
        std::vector<bpolygon> edgesHitBoxes; ///< Arête k correspond aux points k et k+1
        
        private :
        
        
        
        
        
        // ================== Setters & Getters ====================
        public :
        
        double GetSurface() override;
        
        
        
        
        // ================== METHODS ====================
        
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        public :
        
        InteractivePolygon(bptree::ptree & areaTree);
        
        /// \param _Id See DrawablePolygon::DrawablePolygon
        InteractivePolygon(int64_t _Id);
        /// \param _Id See DrawablePolygon::DrawablePolygon
        InteractivePolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio);
        /// \param _Id See DrawablePolygon::DrawablePolygon
       	InteractivePolygon(int64_t _Id, bpt _center, bpolygon& _contourPoints, Colour _fillColour);
        
        virtual ~InteractivePolygon() {}
        
        virtual std::shared_ptr<IDrawableArea> Clone() override;
        protected :
        virtual void onCloned() override;
        public :
        
        // Contruction helpers
        private :
        void init();
 
        
        // - - - - - Display functions - - - - -
        public :
        // virtual void Paint(Graphics& g) override; // Nothing to paint over a normal area !
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
        
        
        // - - - - - Interactions computing - - - - -
        public :
        virtual bool HitTest(bpt T) const override;
        double ComputeInteractionWeight(bpt T) override;
        
        // ----- helpers (for interaction computing) -----
        protected :
        /// \brief Computation of the subdivisions of the polygon : it will be divided into
        /// triangles, each triangle being connected to the center.
        ///
        /// Computation of the angular values (positive, and from the center) taken by each
        /// triangle.
        void updateSubTriangles();
        /// <summary>
        /// Finds the id of the interacted sub triangle.
        ///
        /// May be optimized, but we do not care... Complexity
        /// O(N) is okay, may be O(log(N)) but... N=1..10
        /// </summary>
        /// <param name="angle"></param>
        /// <returns></returns>
        SubTriangle& findSubTriangle(double angle);
        
        
        /// \brief Computes the hit box of each edge of the polygon
        void updateEdgesHitBoxes();
        
		private :
			void computeSurface();
        
    };
    
    
}






#endif  // INTERACTIVEPOLYGON_H_INCLUDED
