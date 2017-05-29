/*
  ==============================================================================

    SpatArea.h
    Created: 28 May 2017 11:04:43am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <memory>


#include "SpatState.hpp"

#include "IEditableArea.h"


namespace Miam
{
 
    // Forward declaration of template
    template<typename T>
    class SpatState;
    
    
    /// \brief Abtract class containing methods implemented by any area meant for
    /// spatialization with Miam::SpatStates
    ///
    /// Is an interface by inheritance, but its own features are all fully implemented
    class SpatArea : public virtual IEditableArea,
                     public std::enable_shared_from_this<SpatArea>
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        std::shared_ptr< SpatState<double> > linkedSpatState;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        int GetSpatStateIndex();
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction (no polymorphic cloning) - - - - -
        
        /// \brief Virtual-inherited class (this one for example) are
        /// constructed first... So we cannot call
        /// pure virtual methods from the constructor of this class
        void OnVirtualBaseClassesContructed();
        
        virtual ~SpatArea() {}

        
        // - - - - - Spat attributes - - - - -
        protected :
        void setDefaultSpatName();
        
        public :
        /// \brief Will notify itself to spat state after linked
        ///
        /// Also auto-unnotifies to the previous spat state, if necessary
        void LinkToSpatState(std::shared_ptr< SpatState<double> > spatState);
        
        void OnSpatStateNameChanged();
        
    };
    
    
    
}
