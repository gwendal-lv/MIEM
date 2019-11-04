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
#include "MiemSegment.h"


namespace Miam
{
    /// \brief A polygonal area with interactive abilities.
    class InteractivePolygon : public DrawablePolygon, public InteractiveArea
    {
        // ================== ATTRIBUTES ====================
        protected :
        std::vector<SubTriangle> subTriangles; ///< subtriangle k is made of point k-1 and k
        std::vector<Segment> segments; ///< Idem subtriangles
        
        // TODO make indexes consistent
        std::vector<bpolygon> edgesHitBoxes; ///< Arête k correspond aux points k et k+1
        
        /// \brief The max distance between the center and a vertex of the outline polygon
        double maxDistanceFromCenter_px = 0.0;
        
        /// \brief Uncorrected weight at the center of the shape.
        /// Used as a normalized factor for the smooth (v1.2) weights computation
        double rawCenterWeight = 1.0;
        
        /// \brief Inside the smooth weights computation, a weighted sum is computed. This ratio
        /// quantifies the ratio between the center's weight, and the combined weight of
        /// all segments.
        ///
        /// This factor must be high if the center is close to several edges, in order
        /// to compensate for the very important of nearby edges.
        /// See examples and more explanation in the MIEM_Surfaces git repo (Python tests)
        ///
        /// The idea is: for each very close segment, almost +2.0 should be added to the factor:
        /// +1.0 for the center itself, and +1.0 to compensate for the very close segment.
        /// *
        double centerInfluenceFactor = 1.0;
        
        /// \brief The spline distorsion applied to smooth weights is needed, because of the
        /// hard gradients obtained near the outline of the polygon.
        ///
        /// The class selected here works well with the centerInfluenceFactor formula
        /// implemented in the .cpp
        /// See a description of the spline in Math:: class
        const int distorsionSplineClass = 2;
        
        
        public :
#ifdef __MIEM_EXPERIMENTS
        /// \brief Dirty, global, thread unsafe variable for telling all areas wether to use
        /// the new or the old weights computation method (for EXPERIMENTS only)
        static bool useSmoothWeights;
#endif
       
        
        
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
        
        // ----- helpers (for interaction computing before v1.2.0) -----
        protected :
        /// \brief Computation of the subdivisions of the polygon : it will be divided into
        /// triangles, each triangle being connected to the center. Also updates the
        /// segments (for fast, smooth weights computations).
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
        
        // ----- helpers (for interaction computing after v1.2.0) -----
        /// \brief Uncorrected, undistorted smooth interaction weight.
        /// Computes a weighted sum between z-values of the edges (all equal to 0) and the z-value
        /// of the center (equal to 1.0). The weights are 1/distance.
        double computeRawSmoothInteractionWeight(bpt T);
        
        
        /// \brief Computes the hit box of each edge of the polygon
        void updateEdgesHitBoxes();
        
		private :
			void computeSurface();
        
    };
    
    
}






#endif  // INTERACTIVEPOLYGON_H_INCLUDED
