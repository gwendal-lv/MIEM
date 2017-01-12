/*
  ==============================================================================

    AreaEvent.h
    Created: 31 Dec 2016 10:55:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef AREAEVENT_H_INCLUDED
#define AREAEVENT_H_INCLUDED


#include <memory>


#include "GraphicEvent.h"

#include "IDrawableArea.h"


namespace Miam
{
    
    
    enum class AreaEventType {
    
        /// None,
        
        NothingHappened,
        
        ShapeChanged, ///< editing contour points
        
        Translation,
        
        //Rotation,
        //Scaling, ///< "surface changed" case
        
        RotScale, ///< the whole area is being manipulated (but not translated)
        
        //TraRotScale, ///< would correspond to a microsft "delta manipulation"
        
    };
    
    
    
    class AreaEvent : public GraphicEvent
    {
        protected :
        AreaEventType eventType = AreaEventType::NothingHappened;
        std::shared_ptr<IDrawableArea> concernedArea;
        
        public :
        AreaEventType GetType() {return eventType;}
        std::shared_ptr<IDrawableArea> GetConcernedArea() {return concernedArea;}
        
        AreaEvent(std::shared_ptr<IDrawableArea> concernedArea_, AreaEventType eventType_);
        virtual ~AreaEvent() {}
        
    };
}


#endif  // AREAEVENT_H_INCLUDED
