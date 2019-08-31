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
