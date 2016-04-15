//
//  DrawablePolygon.hpp
//  MiamEdit
//
//  Created by Gwendal Le Vaillant on 18/02/2016.
//
//

#ifndef DrawablePolygon_hpp
#define DrawablePolygon_hpp

#include "../JuceLibraryCode/JuceHeader.h"

#include "DrawableArea.h"

namespace Miam {
    class DrawablePolygon : public DrawableArea
    {
        
        
        // ========== ATTRIBUTES ==========
        protected :
        // Geometric data
        std::vector<Point<double>> contourPoints; // *normalized* coordinates
        Path contour; // coordinates in *pixels*
        
        
        
        
        // ========== METHODS ==========
        
        // Construction
        public :
        DrawablePolygon(int64_t _Id);
        DrawablePolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius, Colour _fillColour, float screenRatio = 1.77777777); // ratio 16/9è
        DrawablePolygon(int64_t _Id, Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour);
        
        // (re)Construction helpers
        private :
        void createJucePolygon(int width = 160, int height = 90);
        
        // Destruction
        public :
        virtual ~DrawablePolygon();
        
        
        
        
        // Display functions
        public :
        virtual void Paint(Graphics& g);
        virtual void CanvasResized(int width, int height);
        

    };
}


#endif /* DrawablePolygon_hpp */
