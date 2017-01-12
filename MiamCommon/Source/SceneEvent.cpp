/*
  ==============================================================================

    SceneEvent.cpp
    Created: 10 Jan 2017 4:11:07pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SceneEvent.h"


using namespace Miam;

SceneEvent::SceneEvent(MultiSceneCanvasInteractor* canvasManager_, std::shared_ptr<EditableScene> oldScene_, std::shared_ptr<EditableScene> newScene_)
:
canvasManager(canvasManager_), oldScene(oldScene_), newScene(newScene_)
{
    
}
