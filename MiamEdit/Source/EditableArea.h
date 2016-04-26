/*
  ==============================================================================

    EditableArea.h
    Created: 12 Apr 2016 4:39:52pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef EDITABLEAREA_H_INCLUDED
#define EDITABLEAREA_H_INCLUDED


#include <vector>

#include "JuceHeader.h"



namespace Miam
{
    /// \brief Abstract class that defines an interface for all areas that can be edited
    class EditableArea
    {
        
        
        
        // = = = = = = = = = = ENUMS = = = = = = = = = =
        protected :
        
        /// \brief Contains the integer Id of a contour or a special area point.
        ///
        /// Values from 0 to +inf refer to "normal" area contour points, negative values refer
        /// to area points associated to "special functions" (the center, or editing points).
        enum EditableAreaPointId : int { // enum *class* brings cast (to int) issues...
            
            ManipulationPoint = -40, ///< Point that allows rotation and scaling of the whole area
            Center = -30, ///< Center of interaction
            WholeArea = -20, ///< Special point representing the whole area
            
            None = -10 ///< No point selected
            
        };
        
        
        
        
        
        // ===== COMMON METHODS TO ALL EDITABLE AREAS =====
        
        // Construction/destruction
        public :
        EditableArea();
        virtual ~EditableArea();
        
        // ----- Points move -----
        public :
		/// \brief Collision test : tells whether the given point is inside the area
		/// or not
        virtual bool HitTest(const Point<double>& hitPoint) = 0;
		/// \brief Asks the area to prepare itself for moving a point
		///
		/// \param hitPoint The user event point that may be associated to one of the
		/// polygon's points.
		///
		/// \return Whether the hitPoint has launched a point move or not
        virtual bool TryBeginPointMove(const Point<double>& hitPoint) = 0;
		/// \brief Actually moves the point which has been internally selected if
		/// EditableArea::TryBeginPointMove was successful.
        virtual void MovePoint(const Point<double>& newLocation) = 0;
		/// \brief Stop the movement and unselects the internally selected point.
        virtual void EndPointMove() = 0;
        

        // ----- Operations on whole polygon -----
        public :
		/// \brief Performs a translation of the whole area.
		///
		/// \param translation Vector describing the translation (pixel coordinates).
        virtual void Translate(const Point<double>& translation) = 0;
		/// \brief Called from the SceneEditionManager, see EditableArea::isActive
        virtual void SetActive(bool activate) = 0;
        
        // Various updates
        protected :
		/// \brief Internal update function which re-computes all normalized points
		/// coordinates from the current (freshly modified) points with pixel
		/// coordinates.
        virtual void recreateNormalizedPoints() = 0;
        
        
        
        // ===== COMMON ATTRIBUTES TO ALL EDITABLE AREAS =====
        protected :
        
        // Status
        bool isActive; ///< Wether this area is marking itself as active (currently being edited) or not.
        
        // Basic drawing attributes
        
        
        // Additionnal graphical assets

		/// \brief Special point, only visible when area is active, which allows the 
		/// user to scale and rotate the whole area.
        Point<double> manipulationPointInPixels;
		///< For all editing graphical elements drawn on top of a basic DrawableArea
        Colour editingElementsColour;
        
		/// \brief On-screen display radius in pixels (see
		/// InteractiveArea::contourPointsInPixels)
        float contourPointsRadius;
		/// \brief On-screen display radius in pixels (see
		/// EditableArea::manipulationPointInPixels)
        float manipulationPointRadius;
        
		/// \brief Maximum distance in pixels between a displayed point (normal or
		/// special), and a user event that could move this point.
        float pointDraggingRadius;
		/// \brief ID of the only point of this area that is being moved.
        int pointDraggedId;
        Point<double> lastLocation; ///< Back-up for EditableArea::pointDraggedId
        
        float minimumSizePercentage = 0.03f; ///< In % of mean of canvas width and height
        
    };
    
    
}



#endif  // EDITABLEAREA_H_INCLUDED
