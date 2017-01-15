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
        

		// Events that happen at the beginning of a material input
		PointDragBegins,
		AnotherMonoTouchPointDragAlreadyBegun,

		// Events (flows of events) happening during a modification
        ShapeChanged, ///< editing contour points
        Translation,
        //Rotation,
        //Scaling, ///< "surface changed" case
        RotScale, ///< the whole area is being manipulated (but not translated)
        //TraRotScale, ///< 2D modification from 2 touch point inputs
        

		// Events that happen at the end of a material input
		PointDragStops,



		// related to selection state
		Selected,
		Unselected,

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
