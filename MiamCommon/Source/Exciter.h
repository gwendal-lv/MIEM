/*
  ==============================================================================

    Exciter.h
    Created: 11 Jan 2017 2:36:45pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef EXCITER_H_INCLUDED
#define EXCITER_H_INCLUDED


#include <vector>

#include "EditablePolygon.h"


namespace Miam
{
    
    /// \brief 
    class Exciter : public EditablePolygon
    {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        double volume;
        
        /// \brief Ids of the Mouse/touch inputs manipulating this area
        std::vector<int> touchInputSources;
        
        std::shared_ptr<EditablePolygon> volumeControlArea;
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        
        /// \brief Will automatically define the shape of the exciter
        Exciter();
        
        virtual ~Exciter() {}
        
        virtual IDrawableArea* Clone() const override {return new Exciter(*this);}
        
        
    };
    
    
    
}




#endif  // EXCITER_H_INCLUDED
