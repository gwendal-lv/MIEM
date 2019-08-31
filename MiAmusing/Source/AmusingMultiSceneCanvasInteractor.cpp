/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
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

#include "AmusingMultiSceneCanvasInteractor.h"
#include "MultiSceneCanvasComponent.h"
#include "IGraphicSessionManager.h"
#include "AmusingScene.h"
#include "EditablePolygon.h"

AmusingMultiSceneCanvasInteractor::AmusingMultiSceneCanvasInteractor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId) 
	: MultiSceneCanvasEditor(_graphicSessionManager, _canvasComponent, _selfId)
{

}
AmusingMultiSceneCanvasInteractor::~AmusingMultiSceneCanvasInteractor()
{

}


void AmusingMultiSceneCanvasInteractor::AddScene(std::string name)
{
	std::shared_ptr<AmusingScene> newScene(new AmusingScene(this, canvasComponent->GetCanvas()));
	newScene->SetName(name);
	scenes.push_back(newScene);
	SelectScene((int)(scenes.size()) - 1);
	// Graphical updates
	canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
}

void AmusingMultiSceneCanvasInteractor::__AddTestAreas()
{
	for (int i = 0; i < scenes.size(); i++)
	{
		int areasCount = 2 + (rand() % 3);

		auto editableScene = std::dynamic_pointer_cast<AmusingScene>(scenes[i]);
		for (int j = 0; j<areasCount; j++)
		{
			// Only polygons added for now. Ajouter la creation des buffers audio associes a chaque aire
			std::shared_ptr<EditablePolygon> currentEditablePolygon(new EditablePolygon(
				graphicSessionManager->GetNextAreaId(),
				Point<double>(0.2f + 0.13f*j, 0.3f + 0.1f*j), 3 + 2 * j, 0.15f + 0.04f*(j + 1),
				Colour(80 * (uint8)j, 0, 255),
				canvasComponent->GetCanvas()->GetRatio()));
			editableScene->AddArea(currentEditablePolygon);
			//graphicSessionManager->SceneA
		}
	}
}

void AmusingMultiSceneCanvasInteractor::OnSurfaceChanged(double newSurface)
{
	DBG("AmusingMultiSceneCanvasInteractor::OnSurfaceChanged");
	// envoyer l'aire au presenter ou au graphicSessionManager qui fera la conversion aire/frequence
	// le presenter enverra alors la frequence au model
	graphicSessionManager->OnSurfaceChanged(newSurface);
}

/*
void AmusingMultiSceneCanvas::OnAreaCreated()
{
	graphicSessionManager->OnAreaCreated(editableScene->GetName(),currentEditablePolygon->GetId());
}
*/