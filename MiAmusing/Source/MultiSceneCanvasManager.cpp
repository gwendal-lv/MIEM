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
#include "CompletePolygon.h"

#include "GraphicEvent.h"
#include "MultiAreaEvent.h"
#include <thread>

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
	std::shared_ptr<AmusingScene> newScene(new AmusingScene(shared_from_this(), canvasComponent->GetCanvas()));
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
				bpt(0.2f + 0.13f*j, 0.3f + 0.1f*j), 3 + 2 * j, 0.15f + 0.04f*(j + 1),
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

void MultiSceneCanvasManager::AddTrueCircle(uint64_t nextAreaId)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
		handleAndSendAreaEventSync(amusingScene->AddTrueCircle(nextAreaId));
}

void MultiSceneCanvasManager::AddFollower(uint64_t nextAreaId)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
		handleAndSendAreaEventSync(amusingScene->AddFollower(nextAreaId));
}

void MultiSceneCanvasManager::AddCompleteArea()
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
		handleAndSendAreaEventSync(amusingScene->AddCompleteArea(12));
}

void MultiSceneCanvasManager::OnFollowerTranslation(std::shared_ptr<GraphicEvent> graphicE)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if(auto areaE = std::dynamic_pointer_cast<AreaEvent> (graphicE))
			handleAndSendAreaEventSync(areaE);
	}
}

void MultiSceneCanvasManager::OnAudioPosition(double position)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		// faire la translation du curseur de la forme
		//DBG("position = " + (String)position);
		amusingScene->getFirstCompleteArea()->setReadingPosition(position);
		std::shared_ptr<AreaEvent> areaE(new AreaEvent(amusingScene->getFirstCompleteArea(), AreaEventType::NothingHappened,
			amusingScene->getFirstCompleteArea()->GetId()));
		handleAndSendAreaEventSync(areaE);
	}
}

void MultiSceneCanvasManager::SetAllAudioPositions(double position)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		
		handleAndSendAreaEventSync(amusingScene->SetAllAudioPositions(position));
	}
}

int MultiSceneCanvasManager::getNumberArea()
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
		return amusingScene->getNumberArea();
	else
		return 0;
}

void MultiSceneCanvasManager::OnDelete()
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		handleAndSendAreaEventSync(amusingScene->OnDelete());
	}
	
}

void MultiSceneCanvasManager::handleAndSendAreaEventSync(std::shared_ptr<AreaEvent> areaE)
{
	//DBG("MultiSceneCanvasManager::handleAndSendAreaEventSync : " + (String)((int)areaE->GetType()));
	if (areaE->GetType() == AreaEventType::Deleted)
	{
		DBG("need to delete");
		if (auto area = std::dynamic_pointer_cast<AnimatedPolygon> (areaE->GetConcernedArea()))
		{

			if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
				while (true)
				{
					if (auto followerToDelete = amusingScene->getFollowers(area))
					{
						DBG("followerToDelete");
						deleteAsyncDrawableObject((int)followerToDelete->GetId(), followerToDelete);
					}
					else
						break;
				}
		}
		else
		{
			DBG("area to delete : " + (String)((int)areaE->GetAreaIdInScene()) );
			deleteAsyncDrawableObject(areaE->GetAreaIdInScene(), areaE->GetConcernedArea());
		}
	}
	MultiSceneCanvasEditor::handleAndSendAreaEventSync(areaE);
}

void MultiSceneCanvasManager::deleteUnusedFollowers()
{
	DBG("a supprimer");
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		while (true)
		{
			if (auto followerToDelete = amusingScene->getFollowers(nullptr))
			{
				DBG("followerToDelete");
				std::shared_ptr<AreaEvent> areaE(new AreaEvent(followerToDelete, AreaEventType::Deleted));
				graphicSessionManager->HandleEventSync(areaE);
				deleteAsyncDrawableObject((int)followerToDelete->GetId(), followerToDelete);
				// envoyer a l'audio que la source est plus la + retirer de la liste des followers?
			}
			else
				break;
		}
	}
}

void MultiSceneCanvasManager::deleteAsyncDrawableObject(int idInScene, std::shared_ptr<IDrawableArea> originalAreaToDelete)
{
	DBG("MultiSceneCanvasManager::deleteAsyncDrawableObject");
	MultiSceneCanvasInteractor::deleteAsyncDrawableObject(idInScene, originalAreaToDelete);
}

void MultiSceneCanvasManager::OnCanvasMouseDown(const MouseEvent& mouseE)
{
	DBG("passe par MultiSceneCanvasManager::OnCanvasMouseDown");
	MultiSceneCanvasEditor::OnCanvasMouseDown(mouseE);
}