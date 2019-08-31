/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
        /// \return An info about what just happened (for example has something begun ?)
        virtual AreaEventType TryBeginPointMove(const Point<double>& hitPoint) = 0;
        /// \brief Actually moves the point which has been internally selected if
        /// EditableArea::TryBeginPointMove was successful. The move order may not be obeyed if
        /// not validated by the area itself.
        ///
        /// \return An information of what happened to this area during this last small move
        virtual AreaEventType TryMovePoint(const Point<double>& newLocation) = 0;
        /// \brief Stop the movement and unselects the internally selected point.
		///
		/// \return An info about what just happened (if something just stopped for example)
        virtual AreaEventType EndPointMove() = 0;
        
        
        // ----- Operations on whole polygon -----
        public :
        /// \brief Performs a translation of the whole area.
        ///
        /// \param translation Vector describing the translation (pixel coordinates).
        virtual void Translate(const Point<double>& translation) = 0;
        /// \brief See EditableArea::isActive
        virtual void SetActive(bool activate) = 0;
        /// \brief See EditableArea::enableTranslationOnly
        virtual void SetEnableTranslationOnly(bool enableTranslationOnly) = 0;
        
        // Various updates
        protected :
        /// \brief Internal update function which re-computes all normalized points
        /// coordinates from the current (freshly modified) points with pixel
        /// coordinates.
        virtual void recreateNormalizedPoints() = 0;

		public :
		virtual int GetPointDraggedId() = 0;
        
    };
    
    
}




#endif  // IEDITABLEAREA_H_INCLUDED
