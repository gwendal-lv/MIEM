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
    
    
    
    /// \brief Abstract class that defines an interface for all areas that can be edited
    class EditableArea
    {
        
        // ===== COMMON METHODS TO ALL EDITABLE AREAS =====
        
        // Construction/destruction
        public :
        EditableArea();
        virtual ~EditableArea();
        
        // Points move !!!!!!!!!!!!!!!!!!!!!!!! REFACTORING NEEDED !!!!!!!!!!!!!!!!!!!!!!!!
        public :
        virtual bool HitTest(const Point<double>& hitPoint) = 0;
        virtual bool tryBeginPointMove(const Point<double>& hitPoint) = 0;
        virtual void movePoint(const Point<double>& newLocation) = 0;
        virtual void endPointMove() = 0;
        
        // ----- Operations on whole polygon -----
        public :
        virtual void Translate(const Point<double>& translation) = 0;
        virtual void SetActive(bool activate) = 0;
        
        // Various updates
        protected :
        virtual void recreateNormalizedPoints() = 0;
        
        
        
        // ===== COMMON ATTRIBUTES TO ALL EDITABLE AREAS =====
        protected :
        
        // Status
        bool isActive;
        
        // Basic drawing attributes
        int canvasWidth, canvasHeight;
        
        
        // Additionnal graphical assets
        Point<double> manipulationPointInPixels;
        Colour editingElementsColour;
        
        float contourPointsRadius; // pixels
        float manipulationPointRadius; // pixels
        
        float pointDraggingRadius; // pixels
        int pointDragged;
        Point<double> lastLocation; // pixels
        
        float minimumSizePercentage = 0.03f; ///< % of mean of canvas width and height
        
    };
    
    
}



#endif  // EDITABLEAREA_H_INCLUDED
