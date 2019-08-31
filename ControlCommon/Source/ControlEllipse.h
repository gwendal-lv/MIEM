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

#include "EditableEllipse.h"
#include "ControlArea.h"


namespace Miam
{
    
    
    // \brief Not really an interface for now... But might become one.
    class ControlEllipse : public EditableEllipse, public ControlArea
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        protected :
        virtual std::shared_ptr<ControlArea> getCastedSharedFromThis() override
        { return std::static_pointer_cast<ControlEllipse>(shared_from_this()); }
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        
        ControlEllipse(int64_t _Id);
        ControlEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio);
        
        protected :
        void init();
        public :
        
        virtual ~ControlEllipse() {}
        
        virtual std::shared_ptr<IDrawableArea> Clone() override
        {
            auto clone = std::make_shared<ControlEllipse>(*this);
            clone->onCloned();
            return clone;
        }
        
        
        
        
        // - - - - - XML import/export - - - - -
        virtual std::string GetTypeAsString() const override {return "SpatEllipse";}
        
    };
    
    
    
}
