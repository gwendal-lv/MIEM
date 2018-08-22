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

#include "IEditableArea.h"


namespace Miam
{
    /// \brief Abstract class adding attributes for all areas that can be edited
    class EditableArea : public virtual IEditableArea
    {
        
        
        
        // = = = = = = = = = = ENUMS = = = = = = = = = =
        public :
        
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
        
        // ----- Setters and Getters -----
        public :
        void SetActive(bool activate) override;
        
        void SetEnableTranslationOnly(bool enable_) override {enableTranslationOnly = enable_;}
        
		int GetPointDraggedId() override { return pointDraggedId; }
        
        
        
        // ===== COMMON ATTRIBUTES TO ALL EDITABLE AREAS =====
        protected :
        
        // Status
        bool isActive; ///< Wether this area is marking itself as actively editable
        bool enableTranslationOnly; ///< Wether any 2D geometric transformation, or the translation only is authorized
        
        // Basic drawing attributes
        
        
        // Additionnal graphical assets

		/// \brief Special point, only visible when area is active, which allows the 
		/// user to scale and rotate the whole area.
        Point<double> manipulationPointInPixels;
		bpt bmanipulationPointInPixels;
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
        //int pointDraggedId;
        Point<double> lastLocation; ///< Back-up for EditableArea::pointDraggedId
        
        float minimumSizePercentage = 0.03f; ///< In % of mean of canvas width and height

		protected :
			int pointDraggedId;
        
    };
    
    
}



#endif  // EDITABLEAREA_H_INCLUDED
