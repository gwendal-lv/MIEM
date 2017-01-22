/*
  ==============================================================================

    MultiAreaEvent.h
    Created: 22 Jan 2017 6:24:15pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MULTIAREAEVENT_H_INCLUDED
#define MULTIAREAEVENT_H_INCLUDED


#include <vector>
#include <memory>

#include "AreaEvent.h"


namespace Miam
{
    class MultiAreaEvent : public AreaEvent
    {
        
        protected :
        std::vector<std::shared_ptr<AreaEvent>> otherEvents;
        
        
        
        
        
        public :
        
        /// \brief Will transform it to a shared_ptr (will be automtically freed)
        void AddAreaEvent(std::shared_ptr<AreaEvent> areaE);
        
        size_t GetOtherEventsCount() {return otherEvents.size();}
        std::shared_ptr<AreaEvent> GetOtherEvent(size_t i) {return otherEvents[i];}
        
        
        // Same constructors
        
        MultiAreaEvent();
        MultiAreaEvent(std::shared_ptr<IDrawableArea> concernedArea_,
                  AreaEventType eventType_,
                  int areaIdInScene_ = -1,
                  std::shared_ptr<InteractiveScene> concernedScene_ = nullptr);
        
        
        virtual ~MultiAreaEvent() {}
        
    };
}


#endif  // MULTIAREAEVENT_H_INCLUDED
