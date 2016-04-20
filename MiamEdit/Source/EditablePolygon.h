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
    
 
    
    
    /// \brief InterativePolygon that can be edited by the Presenter module, especially
	/// from the SceneEditionManager.
    class EditablePolygon : public InteractivePolygon, public EditableArea
    {
        
        // Construction
        public :
		/// \param _Id See DrawablePolygon::DrawablePolygon
        EditablePolygon(int64_t _Id);
		/// \param _Id See DrawablePolygon::DrawablePolygon
        EditablePolygon(int64_t _Id,
			Point<double> _center, int pointsCount, float radius,
			Colour _fillColour, float _canvasRatio = 1.47); // ratio 800/544 px
		/// \param _Id See DrawablePolygon::DrawablePolygon
        EditablePolygon(int64_t _Id,
			Point<double> _center, std::vector<Point<double>>& _contourPoints,
			Colour _fillColour);
        
        // Construction helpers
        private :
        void init();
        void graphicalInit();
        void behaviorInit();
        
        
        // ----- Display functions -----
        public :
        virtual void Paint(Graphics& g) override;
        virtual void CanvasResized(int width, int height) override;
        // Display helpers
        private :
        void computeManipulationPoint();
        
        
        // ----- Setters and Getters -----
        public :
        void SetActive(bool activate) override;
        
        
        // ----- Edition functions -----

        public :
        // Override of pure virtual functions from the abstract editable area
        bool HitTest(const Point<double>& hitPoint) override;
        bool TryBeginPointMove(const Point<double>& hitPoint) override;
        void MovePoint(const Point<double>& newLocation) override;
        void EndPointMove() override;
        void Translate(const Point<double>& translation) override;
        protected :
        void recreateNormalizedPoints() override;
        // Polygon-specific editing function
        public :
		/// \brief Asks the polygonal area if a new point may be created close to
		/// the hitPoint.
		///
		/// \param hitPoint The user event point that may create a point.
		///
		/// \return Whether the hitPoint was close enough to an existing point, or not.
        bool TryCreatePoint(const Point<double>& hitPoint);
		/// \brief Asks the polygonal area if the point closest to the hitPoint
		/// may be deleted.
		///
		/// \param hitPoint The user event point that may delete a point.
		///
		/// \return The result of the deletion (empty juce::String if deletion happened).
        String TryDeletePoint(const Point<double>& hitPoint);
        // Editing helpers
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
