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

#ifndef IINTERACTIVEAREA_H_INCLUDED
#define IINTERACTIVEAREA_H_INCLUDED


#include <memory> 
#include "IDrawableArea.h"

#include "AreaEvent.h"
#include "AreasGroup.h"

#include "AudioUtils.hpp"

namespace Miam
{
    class Exciter;
    class AreasGroup;
    
    
    /// \brief Pure interface for all areas that can have an interaction
    /// with a user (via mouse, touch, MIDI events...). See Miam::InteractiveArea
    class IInteractiveArea : public virtual IDrawableArea
    {
        
        
        // Construction/destruction
        public :
        IInteractiveArea() {}
        virtual ~IInteractiveArea() {}
        
        
        // Interactions computing
        public :
        /// \brief Collision test : tells whether the given point is inside the area
        /// or not. Coordinates in pixels within a canvas.
        virtual bool HitTest(bpt T) const = 0;
        
        // Surcharge qui devrait disparaître dès que possible...
        protected :
        bool hitTest(double x, double y) const
        { return HitTest(bpt(x,y)); }
        public :
 
        /// \brief Computes a coefficient that quantifies the interaction between a
        /// given point and this area.
        ///
        /// This coefficient is called the "interaction weight", and is guaranteed to be a
        /// spatial-continuous variable.
        /// From version 1.2.0, the gradient of this interaction weight is also continuous.
        ///
        /// \param T The "hit point", in pixels within a canvas,
        /// at which the amount of interaction is computed.
        ///
        /// \return The interaction weight in [0.0 ; 1.0]
        virtual double ComputeInteractionWeight(bpt T) = 0;
        /// \brief Computes the interaction weights of the shape into an image that must have
        /// been previously created, and initialized to zero.
        virtual void ComputeInteractionWeightsInImage(std::vector<double>& img, size_t imgW, size_t imgH) = 0;
        
		virtual double GetSurface() = 0;
        
        /// \brief Will store the group as a weak_pointer
        virtual void SetAreasGroup(std::shared_ptr<AreasGroup>&) = 0;
        /// \brief Returns the group (of overlapping areas) to which this area
        /// belongs. Might be nullptr.
        virtual std::shared_ptr<AreasGroup> GetAreasGroup() = 0;
        
        // - - - - - Interaction avec Excitateurs, spécifiquement - - - - -
        

        virtual std::shared_ptr<AreaEvent> UpdateInteraction(std::shared_ptr<Exciter>& exciter, bool forceDisableInteraction = false) = 0;
        virtual void OnExciterDestruction() = 0;
        
        virtual double GetTotalLinearExcitement() const = 0;
        virtual double GetTotalAudioExcitement() const = 0;
        virtual double GetTotalInteractionWeight() const = 0;
        
        virtual void OnNewExcitementAmount(const std::shared_ptr<Exciter>& sender, Excitement excitementAmount) = 0;

		
        
    };
    
    
}




#endif  // IINTERACTIVEAREA_H_INCLUDED
