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

#include "AreaEvent.h"

#include "InteractiveScene.h"


using namespace Miam;


AreaEvent::AreaEvent()
:
eventType(AreaEventType::NothingHappened),
concernedArea(nullptr),
concernedScene(nullptr),
areaIdInScene(-1)
{
    
}


// Fast constructor with initializers only
AreaEvent::AreaEvent(std::shared_ptr<IDrawableArea> concernedArea_,
                     AreaEventType eventType_,
                     int areaIdInScene_,
                     std::shared_ptr<InteractiveScene> concernedScene_)
:
    eventType(eventType_),
concernedArea(concernedArea_),
    concernedScene(concernedScene_),
    areaIdInScene(areaIdInScene_)
{
    
}

// Constructor without the index of the scene
AreaEvent::AreaEvent(std::shared_ptr<IDrawableArea> concernedArea_,
                     AreaEventType eventType_,
                     std::shared_ptr<InteractiveScene> concernedScene_)
:
eventType(eventType_),
concernedArea(concernedArea_),
concernedScene(concernedScene_),
areaIdInScene(-1)
{
    
}

AreaEvent::AreaEvent( AreaEvent *e )
:
eventType(e->GetType()),
concernedArea(e->GetConcernedArea()),
concernedScene(e->GetConcernedScene()),
areaIdInScene(e->GetAreaIdInScene())
{
    
}

