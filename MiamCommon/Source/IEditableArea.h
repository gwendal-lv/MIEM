/*
  ==============================================================================

    IEditableArea.h
    Created: 8 Oct 2016 11:38:29am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IEDITABLEAREA_H_INCLUDED
#define IEDITABLEAREA_H_INCLUDED

#include "IInteractiveArea.h"


namespace Miam
{
    
    /// \brief Interface for all areas that can be edited
    class IEditableArea : public virtual IInteractiveArea
    {
        
        
        
        // ===== COMMON METHODS TO ALL EDITABLE AREAS =====
        
        // Construction/destruction
        public :
        IEditableArea() {}
        virtual ~IEditableArea() {}
        
        // ----- Points move -----
        public :
        
        /// \brief Asks the area to prepare itself for moving a point
        ///
        /// \param hitPoint The user event point that may be associated to one of the
        /// polygon's points.
        ///
        /// \return Whether the hitPoint has launched a point move or not
        virtual bool TryBeginPointMove(const Point<double>& hitPoint) = 0;
        /// \brief Actually moves the point which has been internally selected if
        /// EditableArea::TryBeginPointMove was successful. The move order may not be obeyed if
        /// not validated by the area itself.
        ///
        /// \return Wether the point was moved or not.
        virtual bool TryMovePoint(const Point<double>& newLocation) = 0;
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

		public :
		virtual int GetPointDraggedId() = 0;

		protected :
		int pointDraggedId;
        
    };
    
    
}




#endif  // IEDITABLEAREA_H_INCLUDED
