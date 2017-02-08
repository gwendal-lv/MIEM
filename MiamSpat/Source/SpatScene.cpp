/*
  ==============================================================================

    SpatScene.cpp
    Created: 11 Jan 2017 10:50:21am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatScene.h"
#include "Exciter.h"

SpatScene::SpatScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_)
:
Miam::EditableScene(canvasManager_, canvasComponent_, false) // no area is selectable
{
}


SpatScene::~SpatScene()
{
}



// - - - - - Canvas (mouse) events managing - - - - -

std::shared_ptr<GraphicEvent> SpatScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
    return InteractiveScene::OnCanvasMouseDown(mouseE);
}
std::shared_ptr<GraphicEvent> SpatScene::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
    return InteractiveScene::OnCanvasMouseDrag(mouseE);
}
std::shared_ptr<GraphicEvent> SpatScene::OnCanvasMouseUp(const MouseEvent& mouseE)
{
    return InteractiveScene::OnCanvasMouseUp(mouseE);
}
