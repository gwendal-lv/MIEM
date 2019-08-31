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

#include "EditablePolygon.h"
#include "ControlArea.h"


namespace Miam
{
    
    
    // \brief Not really an interface for now... But might become one.
    class ControlPolygon : public EditablePolygon, public ControlArea
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        protected :
        virtual std::shared_ptr<ControlArea> getCastedSharedFromThis() override
        { return std::static_pointer_cast<ControlPolygon>(shared_from_this()); }
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        
        ControlPolygon(bptree::ptree & areaTree);
        
        /// \param _Id See EditablePolygon::EditablePolygon
        ControlPolygon(int64_t _Id);
        /// \param _Id See EditablePolygon::EditablePolygon
        ControlPolygon(int64_t _Id,
                        bpt _center, int pointsCount, float radius,
                        Colour _fillColour, float _canvasRatio = 1.47); // ratio 800/544 px
        /// \param _Id See EditablePolygon::EditablePolygon
        ControlPolygon(int64_t _Id,
                        bpt _center, bpolygon& _contourPoints,
                        Colour _fillColour);
        
        protected :
        void init();
        public :
        
        virtual ~ControlPolygon() {}
        
        virtual std::shared_ptr<IDrawableArea> Clone() override
        {
            auto clone = std::make_shared<ControlPolygon>(*this);
            clone->onCloned();
            return clone;
        }
        
        
        
        // - - - - - XML import/export - - - - -
        virtual std::string GetTypeAsString() const override {return "SpatPolygon";}
        virtual std::shared_ptr<bptree::ptree> GetTree() override;
        
        
    };
    
    
    
}
