/*
  ==============================================================================

    SpatEllipse.h
    Created: 28 May 2017 11:06:09am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "EditableEllipse.h"
#include "SpatArea.h"


namespace Miam
{
    
    
    // \brief Not really an interface for now... But might become one.
    class SpatEllipse : public EditableEllipse, public SpatArea
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        protected :
        virtual std::shared_ptr<SpatArea> getCastedSharedFromThis() override
        { return std::static_pointer_cast<SpatEllipse>(shared_from_this()); }
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        
        SpatEllipse(int64_t _Id);
        SpatEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio);
        
        protected :
        void init();
        public :
        
        virtual ~SpatEllipse() {}
        
        virtual std::shared_ptr<IDrawableArea> Clone() override
        {
            auto clone = std::make_shared<SpatEllipse>(*this);
            clone->onCloned();
            return clone;
        }
        
        
        
        
        // - - - - - XML import/export - - - - -
        virtual std::string GetTypeAsString() const override {return "SpatEllipse";}
        
    };
    
    
    
}
