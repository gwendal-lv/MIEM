/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
