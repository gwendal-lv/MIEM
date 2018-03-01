/*
  ==============================================================================

    PlayScene.cpp
    Created: 11 Jan 2017 10:50:21am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "PlayScene.h"
#include "Exciter.h"

PlayScene::PlayScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_)
:
Miam::EditableScene(canvasManager_, canvasComponent_, false) // no area is selectable
{
}


PlayScene::~PlayScene()
{
}



// - - - - - Canvas (mouse) events managing - - - - -

std::shared_ptr<GraphicEvent> PlayScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
    return InteractiveScene::OnCanvasMouseDown(mouseE);
}
std::shared_ptr<GraphicEvent> PlayScene::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
    return InteractiveScene::OnCanvasMouseDrag(mouseE);
}
std::shared_ptr<GraphicEvent> PlayScene::OnCanvasMouseUp(const MouseEvent& mouseE)
{
    return InteractiveScene::OnCanvasMouseUp(mouseE);
}
