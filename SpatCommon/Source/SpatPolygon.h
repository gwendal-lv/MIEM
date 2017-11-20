/*
  ==============================================================================

    SpatPolygon.h
    Created: 28 May 2017 11:06:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "EditablePolygon.h"
#include "SpatArea.h"


namespace Miam
{
    
    
    // \brief Not really an interface for now... But might become one.
    class SpatPolygon : public EditablePolygon, public SpatArea
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        protected :
        virtual std::shared_ptr<SpatArea> getCastedSharedFromThis() override
        { return std::static_pointer_cast<SpatPolygon>(shared_from_this()); }
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        
        SpatPolygon(bptree::ptree & areaTree);
        
        /// \param _Id See EditablePolygon::EditablePolygon
        SpatPolygon(int64_t _Id);
        /// \param _Id See EditablePolygon::EditablePolygon
        SpatPolygon(int64_t _Id,
                        bpt _center, int pointsCount, float radius,
                        Colour _fillColour, float _canvasRatio = 1.47); // ratio 800/544 px
        /// \param _Id See EditablePolygon::EditablePolygon
        SpatPolygon(int64_t _Id,
                        bpt _center, bpolygon& _contourPoints,
                        Colour _fillColour);
        
        protected :
        void init();
        public :
        
        virtual ~SpatPolygon() {}
        
        virtual std::shared_ptr<IDrawableArea> Clone() const override
        { return std::make_shared<SpatPolygon>(*this); }
        
        
        
        // - - - - - XML import/export - - - - -
        virtual std::string GetTypeAsString() const override {return "SpatPolygon";}
        virtual std::shared_ptr<bptree::ptree> GetTree() override;
        
        
    };
    
    
    
}
