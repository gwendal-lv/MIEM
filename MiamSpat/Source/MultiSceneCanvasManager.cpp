/*
  ==============================================================================

    MultiSceneCanvasManager.cpp
    Created: 10 Jan 2017 9:37:54pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MultiSceneCanvasManager.h"

#include "MultiSceneCanvasComponent.h"

#include "SpatScene.h"


using namespace Miam;

MultiSceneCanvasManager::MultiSceneCanvasManager(IGraphicSessionManager* graphicSessionManager_, MultiSceneCanvasComponent* canvasComponent_, SceneCanvasComponent::Id selfId_)
:
MultiSceneCanvasEditor(graphicSessionManager_, canvasComponent_, selfId_)
{
    
}

MultiSceneCanvasManager::~MultiSceneCanvasManager()
{
    
}

void MultiSceneCanvasManager::AddScene(std::string name)
{
    // We construct scene that DO NOT allow the selection of a particular area
    std::shared_ptr<EditableScene> newScene(new SpatScene(selfPtr.lock(), canvasComponent->GetCanvas()));
    newScene->SetName(name);
    
    MultiSceneCanvasInteractor::AddScene(newScene);
}
