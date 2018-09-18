/*
  ==============================================================================

    IInteractiveArea.h
    Created: 8 Oct 2016 11:37:34am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IINTERACTIVEAREA_H_INCLUDED
#define IINTERACTIVEAREA_H_INCLUDED


#include <memory> 
#include "IDrawableArea.h"

#include "AreaEvent.h"

#include "AudioUtils.hpp"

namespace Miam
{
    class Exciter;
    
    
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
        ///
        /// \param T The "hit point", in pixels within a canvas,
        /// at which the amount of interaction is computed.
        ///
        /// \return The interaction weight in [0.0 ; 1.0]
        virtual double ComputeInteractionWeight(bpt T) = 0;
        
		virtual double GetSurface() = 0;
        
        
        // - - - - - Interaction avec Excitateurs, spécifiquement - - - - -
        

        virtual std::shared_ptr<AreaEvent> UpdateInteraction(std::shared_ptr<Exciter>& exciter) = 0;
        virtual void OnExciterDestruction() = 0;
        
        virtual double GetTotalLinearExcitement() const = 0;
        virtual double GetTotalAudioExcitement() const = 0;
        virtual double GetTotalInteractionWeight() const = 0;
        
        virtual void OnNewExcitementAmount(const std::shared_ptr<Exciter>& sender, Excitement excitementAmount) = 0;

		
        
    };
    
    
}




#endif  // IINTERACTIVEAREA_H_INCLUDED
