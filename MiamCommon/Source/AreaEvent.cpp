/*
  ==============================================================================

    AreaEvent.cpp
    Created: 31 Dec 2016 10:55:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "AreaEvent.h"

using namespace Miam;

AreaEvent::AreaEvent(std::shared_ptr<IDrawableArea> concernedArea_, AreaEventType eventType_) :
    eventType(eventType_),
    concernedArea(concernedArea_)
{
    
}
