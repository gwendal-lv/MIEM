//
//  DrawablePolygon.hpp
//  MiamEdit
//
//  Created by Gwendal Le Vaillant on 18/02/2016.
//
//

#ifndef DrawablePolygon_hpp
#define DrawablePolygon_hpp


#include "JuceHeader.h"
#include "DrawableArea.h"


namespace Miam {
    
    
    
    /// \brief A polygon that can be drawn on a SceneCanvasComponent.
    class DrawablePolygon : public DrawableArea
    {
        
        
        // ========== ATTRIBUTES ==========
        protected :
        // Geometric data
        std::vector<Point<double>> contourPoints; ///< Normalized contour points coordinates (x,y in [0.0;1.0])
        Path contour; ///< Closed contour path (coordinates in pixels, relative to its canvas)
        
        
        
        
        // ========== METHODS ==========
        
        // Construction
        public :
        
        /// \brief Default constructor that builds a centered dark grey triangle
        ///
        /// \param See DrawableArea::id
        DrawablePolygon(int64_t _Id);
        /// \brief Construction of a regular polygon defined by the given parameters.
        ///
        /// \param _Id See DrawableArea::id
        /// \param _center See DrawableArea::center
        /// \param pointsCount Number of point of the polygon.
        /// \param radius Distance (normalized) between the center and each contour point.
        /// \param _fillColour See DrawableArea::fillColour
        /// \param _canvasRatio Current canvas ratio, optionnal but needed to draw a
        /// regular-looking polygon (16/9 by default)
        DrawablePolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio = 1.77777777f);
        /// \brief Construction of a polygon defined
        /// Construction of any polygon defined by its center and contour points
        ///
        /// \param _Id See DrawableArea::id
        /// \param _center See DrawableArea::center
        /// \param _contourPoints See DrawablePolygon::contourPoints
        /// \param _fillColour See DrawableArea::fillColour
        DrawablePolygon(int64_t _Id, Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour);
        
        // (re)Construction helpers
        private :
        void createJucePolygon(int width = 160, int height = 90);
        
        public :
        /// \brief Destructor.
        virtual ~DrawablePolygon();
        
        
        
        
        // Display functions
        public :
        virtual void Paint(Graphics& g) override;
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
        

    };
}


#endif /* DrawablePolygon_hpp */