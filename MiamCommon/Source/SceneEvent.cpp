/*
  ==============================================================================

    SceneEvent.cpp
    Created: 10 Jan 2017 4:11:07pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SceneEvent.h"


using namespace Miam;

SceneEvent::SceneEvent(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, std::shared_ptr<EditableScene> scene_, SceneEventType eventType_)
	: eventType(eventType_), canvasManager(canvasManager_), newScene(scene_)
{

}

SceneEvent::SceneEvent(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, std::shared_ptr<EditableScene> oldScene_, std::shared_ptr<EditableScene> newScene_, SceneEventType eventType_)
:
eventType(eventType_),
canvasManager(canvasManager_),
oldScene(oldScene_), newScene(newScene_)
{
    
}

