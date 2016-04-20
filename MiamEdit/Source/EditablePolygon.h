/*
  ==============================================================================

    EditablePolygon.h
    Created: 26 Mar 2016 11:08:16am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef EDITABLEPOLYGON_H_INCLUDED
#define EDITABLEPOLYGON_H_INCLUDED

#include <vector>

#include "InteractivePolygon.h"
#include "EditableArea.h"


namespace Miam {
    
 
    
    
    
    class EditablePolygon : public InteractivePolygon, public EditableArea
    {
        
        // Construction
        public :
        EditablePolygon(int64_t _Id);
        EditablePolygon(int64_t _Id, Point<double> _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio = 1.47); // ratio 800/544 px
        EditablePolygon(int64_t _Id, Point<double> _center, std::vector<Point<double>>& _contourPoints, Colour _fillColour);
        
        // Construction helper
        private :
        void init();
        void graphicalInit();
        void behaviorInit();
        
        
        // Display functions
        public :
        virtual void Paint(Graphics& g) override;
        virtual void CanvasResized(int width, int height) override;
        // helpers
        private :
        void computeManipulationPoint();
        
        
        // Setters and Getters
        public :
        void SetActive(bool activate) override;
        
        
        // Edition functions
        public :
        // Override of pure virtual functions from the abstract editable area
        bool HitTest(const Point<double>& hitPoint) override;
        bool tryBeginPointMove(const Point<double>& hitPoint) override;
        void movePoint(const Point<double>& newLocation) override;
        void endPointMove() override;
        void Translate(const Point<double>& translation) override;
        protected :
        void recreateNormalizedPoints() override;
        // Polygon-specific editing function
        public :
        bool tryCreatePoint(const Point<double>& hitPoint);
        String tryDeletePoint(const Point<double>& hitPoint);
        // helpers
        private :
        bool isNewContourPointValid(const Point<double>& newLocation);
        bool isNewCenterValid(const Point<double>& newLocation);
        bool isCenterValidWithoutContourPoint(int contourPointId);
        void insertPointInPixels(const Point<double>& newContourPoint, int position);
        void deletePoint(int position);
        
        
        
        // Graphical assets for editing
        private :
        
    };
    
    
}



#endif  // EDITABLEPOLYGON_H_INCLUDED
