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
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        
        SpatEllipse(int64_t _Id);
        SpatEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio);
        
        protected :
        void init();
        public :
        
        virtual ~SpatEllipse() {}
        
        virtual IDrawableArea* Clone() const override {return new SpatEllipse(*this);}
        
        
        
    };
    
    
    
}
