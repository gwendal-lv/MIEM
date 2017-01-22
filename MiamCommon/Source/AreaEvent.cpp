/*
  ==============================================================================

    AreaEvent.cpp
    Created: 31 Dec 2016 10:55:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "AreaEvent.h"

#include "InteractiveScene.h"


using namespace Miam;


AreaEvent::AreaEvent()
:
eventType(AreaEventType::NothingHappened),
concernedArea(nullptr),
//secondConcernedArea(nullptr),
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
//secondConcernedArea(nullptr),
    concernedScene(concernedScene_),
    areaIdInScene(areaIdInScene_)
{
    
}


/*
// Fast constructor with initializers only - 2 AREAS VERSION
AreaEvent::AreaEvent(std::shared_ptr<IDrawableArea> concernedArea_,
                     std::shared_ptr<IDrawableArea> secondConcernedArea_,
                     AreaEventType eventType_,
                     int areaIdInScene_,
                     std::shared_ptr<InteractiveScene> concernedScene_)
:
eventType(eventType_),
concernedArea(concernedArea_),
secondConcernedArea(secondConcernedArea_),
concernedScene(concernedScene_),
areaIdInScene(areaIdInScene_)
{
    
}
*/
