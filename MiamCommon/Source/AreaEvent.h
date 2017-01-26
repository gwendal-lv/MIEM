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
        
        
        // - - - - - Events on areas addition / deletion - - - - -
        
        Added, ///< Area has just been added to the scene
        Deleted, ///< Area has just been deleted to the scene
        
        

		// - - - - - Events that happen at the beginning of a material input - - - - -
        
		PointDragBegins, ///< The last input was close enough to a point to begin a 2D transformation
		AnotherMonoTouchPointDragAlreadyBegun, ///< voilà

        
        
		//  - - - - - Events (flows of events) happening during a modification - - - - -
        
        ShapeChanged, ///< editing contour points
        Translation, ///< Pure translation
        //Rotation,
        //Scaling, ///< "surface changed" case
        RotScale, ///< the whole area is being manipulated (but not translated)
        //TraRotScale, ///< 2D modification from 2 touch point inputs
        
        

		//  - - - - - Events that happen at the end of a material input - - - - -
        
		PointDragStops, ///< The input was release and is not linked to the area anymore



		//  - - - - - Event related to selection state - - - - -
        
		Selected, ///< Area has just been selected
		Unselected, ///< Area has just been selected

        
        
    };
    
    
    
    class AreaEvent : public GraphicEvent
    {
		private :
		AreaEventType eventType = AreaEventType::NothingHappened;

        protected :
        std::shared_ptr<IDrawableArea> concernedArea;
        
        public :
        AreaEventType GetType() {return eventType;}
        std::shared_ptr<IDrawableArea> GetConcernedArea() {return concernedArea;}
        
        AreaEvent(std::shared_ptr<IDrawableArea> concernedArea_, AreaEventType eventType_);
        virtual ~AreaEvent() {}
        
    };
}


#endif  // AREAEVENT_H_INCLUDED
