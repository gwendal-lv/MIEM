/*
  ==============================================================================

    InteractivePolygon.h
    Created: 13 Apr 2016 3:56:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef INTERACTIVEPOLYGON_H_INCLUDED
#define INTERACTIVEPOLYGON_H_INCLUDED


#include "DrawablePolygon.h"
#include "InteractiveArea.h"

#include "SubTriangle.h"


namespace Miam
{
    
    
    class InteractivePolygon : public DrawablePolygon, public InteractiveArea
    {
        
        
        // Construction
        public :
        InteractivePolygon(int64_t _Id);
        InteractivePolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio);
        InteractivePolygon(int64_t _Id, Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour);
        
        private :
        void init();
        
        
        
        
        // Display functions
        public :
        // virtual void Paint(Graphics& g) override; // Nothing to paint over a normal area !
        virtual void CanvasResized(int width, int height) override;
        
        
        
        
        // Updates
        protected :
        /// <summary>
        /// Computation of the subdivisions of the polygon.
        /// It will be divided into triangles, each triangle being connected
        /// to the center.
        ///
        /// Computation of the angular values (from the center) taken by each
        /// triangle.
        /// </summary>
        void updateSubTriangles();
        
        
        // Interactions computing
        public :
        /// <summary>
        /// Computes the weight of an interaction between a point (touch, mouse, ...)
        /// and the polygon.
        /// </summary>
        /// <param name="T">Interaction point</param>
        double ComputeInteractionWeight(Point<double> T) override;
        
        // Private helpers (for interaction computing)
        private :
        /// <summary>
        /// Finds the id of the interacted sub triangle.
        ///
        /// May be optimized, but we do not care... Complexity
        /// O(N) is okay, may be O(log(N)) but... N=1..10
        /// </summary>
        /// <param name="angle"></param>
        /// <returns></returns>
        SubTriangle& findSubTriangle(double angle);
        
        
        // Attributes
        private :
        std::vector<SubTriangle> subTriangles;
        
        
    };
    
    
}






#endif  // INTERACTIVEPOLYGON_H_INCLUDED
