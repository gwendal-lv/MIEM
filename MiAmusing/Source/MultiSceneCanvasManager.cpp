/*
  ==============================================================================

    MultiSceneCanvasManager.cpp
    Created: 10 Jan 2017 9:37:54pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MultiSceneCanvasManager.h"

#include "MultiSceneCanvasComponent.h"

#include "AmusingScene.h"

#include "AnimatedPolygon.h"


using namespace Amusing;

MultiSceneCanvasManager::MultiSceneCanvasManager(IGraphicSessionManager* graphicSessionManager_, MultiSceneCanvasComponent* canvasComponent_, SceneCanvasComponent::Id selfId_)
:
MultiSceneCanvasEditor(graphicSessionManager_, canvasComponent_, selfId_),
graphicSessionManager(graphicSessionManager_)
{
    
}

MultiSceneCanvasManager::~MultiSceneCanvasManager()
{
    
}

void MultiSceneCanvasManager::AddScene(std::string name)
{
    // We construct scene that DO NOT allow the selection of a particular area
    //std::shared_ptr<EditableScene> newScene(new AmusingScene(this, canvasComponent->GetCanvas()));
	DBG("passe dans le bon");
	std::shared_ptr<AmusingScene> newScene(new AmusingScene(selfPtr.lock(), canvasComponent->GetCanvas()));
    newScene->SetName(name);
    
    MultiSceneCanvasInteractor::AddScene(newScene);
}

void MultiSceneCanvasManager::__AddAnimatedTestAreas()
{
	DBG("ICI");
	for (size_t i = 0; i < scenes.size(); i++)
	{
		int areasCount = 2 + (rand() % 3);

		auto editableScene = std::dynamic_pointer_cast<EditableScene>(scenes[i]);
		for (int j = 0; j<areasCount; j++)
		{
			// Only polygons added for now
			std::shared_ptr<AnimatedPolygon> currentEditablePolygon(new AnimatedPolygon(
				graphicSessionManager->GetNextAreaId(),
				Point<double>(0.2f + 0.13f*j, 0.3f + 0.1f*j), 3 + 2 * j, 0.15f + 0.04f*(j + 1),
				Colour(80 * (uint8)j, 0, 255),
				canvasComponent->GetCanvas()->GetRatio()));
			editableScene->AddArea(currentEditablePolygon);
		}
	}
}

void MultiSceneCanvasManager::AddNedgeArea(uint64_t nextAreaId, int N)
{
	if(auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
		handleAndSendAreaEventSync(amusingScene->AddNedgeArea(nextAreaId,N));
}