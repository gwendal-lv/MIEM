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

#include "MultiAreaEvent.h"

using namespace Miam;


MultiAreaEvent::MultiAreaEvent()
:
AreaEvent()
{
    
}


MultiAreaEvent::MultiAreaEvent(std::shared_ptr<IDrawableArea> concernedArea_,
                               AreaEventType eventType_,
                               std::shared_ptr<InteractiveScene> concernedScene_)
:
AreaEvent(concernedArea_, eventType_, concernedScene_)
{
    
}

MultiAreaEvent::MultiAreaEvent(std::shared_ptr<DrawableArea> concernedArea_,
               AreaEventType eventType_,
               int areaIdInScene_,
               std::shared_ptr<InteractiveScene> concernedScene_)
:
AreaEvent(concernedArea_, eventType_, areaIdInScene_, concernedScene_)
{
    
}

MultiAreaEvent::MultiAreaEvent( AreaEvent *e )
:
AreaEvent( e )
{
    
}


void MultiAreaEvent::AddAreaEvent(std::shared_ptr<AreaEvent> areaE)
{
    otherEvents.push_back(areaE);
}

void MultiAreaEvent::AddAreaEvent(AreaEvent* areaE)
{
    otherEvents.push_back(std::shared_ptr<AreaEvent>(areaE));
}

