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
