/*
  ==============================================================================

    ControlArea.h
    Created: 28 May 2017 11:04:43am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <memory>
#include <list>

// #include "ControlState.hpp"

#include "IEditableArea.h"


namespace Miam
{
 
    // Forward declaration of template
    template<typename T>
    class ControlState;
    
    class Exciter;
    
    
    /// \brief Abtract class containing methods implemented by any area meant for
    /// interpolated controlling with Miam::ControlState
    ///
    /// Is an interface by inheritance, but its own features are all fully implemented
    class ControlArea : public virtual IEditableArea //,
                     //public std::enable_shared_from_this<ControlArea>
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        std::shared_ptr< ControlState<double> > linkedState;

        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        int GetStateIndex();
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction (no polymorphic cloning) - - - - -
        ControlArea();
        
        /// \brief Virtual-inherited class (this one for example) are
        /// constructed first... So we cannot call
        /// pure virtual methods from the constructor of this class
        void OnVirtualBaseClassesContructed();
        
        virtual ~ControlArea() {}

        
        // - - - - - Linked State attributes - - - - -
        protected :
        void setDefaultName();
        virtual std::shared_ptr<ControlArea> getCastedSharedFromThis() = 0;
        
        public :
        /// \brief Will notify itself to state after linked
        ///
        /// Also auto-unnotifies to the previous state, if necessary
        void LinkToState(std::shared_ptr< ControlState<double> > state);
        
        void OnStateNameChanged();
        
        
        
        // - - - - - XML import/export - - - - -
        protected :
        std::shared_ptr<bptree::ptree> getStateTree();
        
    };
    
    
    
}
