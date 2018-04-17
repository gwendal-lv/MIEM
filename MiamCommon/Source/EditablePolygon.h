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
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        public :
        
        EditablePolygon(bptree::ptree& areaTree);
        
		/// \param _Id See DrawablePolygon::DrawablePolygon
        EditablePolygon(int64_t _Id);
		/// \param _Id See DrawablePolygon::DrawablePolygon
        EditablePolygon(int64_t _Id,
			bpt _center, int pointsCount, float radius,
			Colour _fillColour, float _canvasRatio = 1.47); // ratio 800/544 px
		/// \param _Id See DrawablePolygon::DrawablePolygon
        EditablePolygon(int64_t _Id,
			bpt _center, bpolygon& _contourPoints,
			Colour _fillColour);
        
        virtual std::shared_ptr<IDrawableArea> Clone() override
        {
            auto clone = std::make_shared<EditablePolygon>(*this);
            clone->onCloned();
            return clone;
        }
        
        // Construction helpers
        private :
        void init();
        void graphicalInit();
        void behaviorInit();
        
        // Destruction
        public :
        virtual ~EditablePolygon() {}
        
        
        // ----- Display functions -----
        public :
        virtual void Paint(Graphics& g) override;
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
        // Display helpers
        private :
        void computeManipulationPoint();
        
        
        // ----- Setters and Getters -----
        public :
        void SetActive(bool activate) override;
		bool IsActive() override;
		bool ShowCenter() override;
        
        // ----- Edition functions -----

        public :
        // Override of pure virtual functions from the abstract editable area
		AreaEventType TryBeginPointMove(const Point<double>& hitPoint) override;
        AreaEventType TryMovePoint(const Point<double>& newLocation) override;
		AreaEventType EndPointMove() override;
		bool SizeChanged(double size, bool minSize);
		void Rotate(double Radian);
		void updateContourPoints();
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
        bool isCenterValidWithoutContourPoint(size_t contourPointId);
        void insertPointInPixels(const bpt& newContourPoint, int position);
        void deletePoint(int position);
        
        
        
        // Graphical assets for editing
        private :
			bpt GetManipulationPoint() override;
    };
    
    
}



#endif  // EDITABLEPOLYGON_H_INCLUDED
