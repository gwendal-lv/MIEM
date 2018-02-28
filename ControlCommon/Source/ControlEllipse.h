/*
  ==============================================================================

    ControlEllipse.h
    Created: 28 May 2017 11:06:09am
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
