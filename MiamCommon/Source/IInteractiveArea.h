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
        virtual bool HitTest(double x, double y) = 0;
        virtual bool HitTest(bpt T)
        { return HitTest(T.get<0>(), T.get<1>()); }
 
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
        
        /// \brief Internally updates the interaction weight with a given exciter.
        ///
        /// \returns Wether the interaction between this area and an exciter has changed, or not.
        virtual std::shared_ptr<AreaEvent> UpdateInteraction(std::shared_ptr<Exciter>& exciter) = 0;
        
        /// \brief Retourne la somme (sans saturation) de l'excitation cumulée
        /// de tous les excitateurs interagissant actuellement avec la forme.
        virtual double GetTotalExcitementAmount() const = 0;
        
        

		
        
    };
    
    
}




#endif  // IINTERACTIVEAREA_H_INCLUDED
