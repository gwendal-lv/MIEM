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
        
        void AddAreaEvent(std::shared_ptr<AreaEvent> areaE);
        void AddAreaEvent(AreaEvent* areaE);
        
        /// \brief This count does not include the main AreaEvent
        size_t GetOtherEventsCount() const {return otherEvents.size();}
        std::shared_ptr<AreaEvent>& GetOtherEvent(size_t i) {return otherEvents[i];}
        
        
        // Same constructors as the single-area version
        
        MultiAreaEvent();
        
        MultiAreaEvent(std::shared_ptr<IDrawableArea> concernedArea_,
                       AreaEventType eventType_,
                       std::shared_ptr<InteractiveScene> concernedScene_);
        
        MultiAreaEvent(std::shared_ptr<DrawableArea> concernedArea_,
                  AreaEventType eventType_,
                  int areaIdInScene_ = -1,
                  std::shared_ptr<InteractiveScene> concernedScene_ = nullptr);
        
        /// \brief Crée simplement l'évènement principal de cette classe, en le construisant par copie de
        /// l'évènement fourni en paramètre.
        MultiAreaEvent( AreaEvent *e );
        
        virtual ~MultiAreaEvent() {}
        
    };
}


#endif  // MULTIAREAEVENT_H_INCLUDED
