/*
  ==============================================================================

    MultiAreaEvent.cpp
    Created: 22 Jan 2017 6:24:15pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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

