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

#include "SceneEvent.h"
#include "GraphicEvent.h"
#include "MultiAreaEvent.h"
#include "GraphicSessionManager.h"
#include <thread>
#include "boost\geometry.hpp"

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
	DBG("__AddAnimatedtestAreas");
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
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		std::shared_ptr<AreaEvent> areaE = amusingScene->AddNedgeArea(nextAreaId, N);
		handleAndSendAreaEventSync(areaE);
		handleAndSendAreaEventSync(amusingScene->AddCursor(areaE->GetConcernedArea()));
	}

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

std::shared_ptr<EditableScene> MultiSceneCanvasManager::GetSelectedScene()
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		return selectedScene;
	}
	else
		return nullptr;
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
		//amusingScene->getFirstCompleteArea()->setReadingPosition(position);
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

void MultiSceneCanvasManager::SetAudioPositions(std::shared_ptr<Cursor> cursor, double position)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		bpt oldPosition = cursor->getPosition();//InPixels();
		if (amusingScene->isDrew(cursor) && cursor->setReadingPosition(position)) // vérifie si le curseur est dessiné et le met à jour (seulement si la condition "dessiné" est déjà vérifiée)
		{
			if (auto eventA = amusingScene->checkCursorPosition(cursor)) // regarder si collision avec une autre aire
			{
				// envoyer les evts de collision (Update pour lier le RH à la TL correspondante)
				if (auto completeP = std::dynamic_pointer_cast<CompletePolygon>(eventA->GetConcernedArea()))
				{
					
					std::vector<bpt> inter;
					boost::geometry::model::segment<bpt> segB = completeP->getSegment(oldPosition);//InPixels(oldPosition);
					boost::geometry::model::segment<bpt> segA(oldPosition, cursor->getPosition());//InPixels());
					boost::geometry::intersection(segA, segB, inter);
					if (inter.size() == 1)
					{
						cursor->setCenterPositionNormalize(inter[0]);//cursor->setCenterPosition(inter[0]);
					}
					//double old = cursor->getPositionInAssociateArea();
					DBG("change d'aire");
					cursor->LinkTo(completeP);
					//DBG("change d'aire : old "+ (String)old +" new "+ (String)cursor->getPositionInAssociateArea());
					handleAndSendAreaEventSync(std::shared_ptr<AreaEvent>(new AreaEvent(cursor,eventA->GetType(),cursor->GetId(),eventA->GetConcernedScene())));
				}
				else
					handleAndSendAreaEventSync(eventA);
			}
			else
			{
				std::shared_ptr<AreaEvent> areaE(new AreaEvent(cursor, AreaEventType::NothingHappened));
				handleAndSendAreaEventSync(areaE);
			}
		}
		/*if(auto completeA = std::dynamic_pointer_cast<CompletePolygon>(area))
			handleAndSendAreaEventSync(completeA->setReadingPosition(position));*/
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
	
	if (auto multiE = std::dynamic_pointer_cast<MultiAreaEvent>(areaE))
	{
		
		for (int i = 0; i < multiE->GetOtherEventsCount(); i++)
		{
			
			MultiSceneCanvasEditor::handleAndSendAreaEventSync(multiE->GetOtherEvent(i));
		}
	}
	else
	{
		if (areaE->GetType() == AreaEventType::Deleted) // verifier si ca sert encore a qqch
		{
			
			//if (auto area = std::dynamic_pointer_cast<AnimatedPolygon> (areaE->GetConcernedArea()))
			//{

			//	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
			//		while (true)
			//		{
			//			if (auto followerToDelete = amusingScene->getFollowers(area))
			//			{
			//				DBG("followerToDelete");
			//				deleteAsyncDrawableObject((int)followerToDelete->GetId(), followerToDelete);
			//			}
			//			else
			//				break;
			//		}
			//}
			//else
			//{
			//	DBG("area to delete : " + (String)((int)areaE->GetAreaIdInScene()));
			//	//deleteAsyncDrawableObject(areaE->GetAreaIdInScene(), areaE->GetConcernedArea());
			//}
		}
		MultiSceneCanvasEditor::handleAndSendAreaEventSync(areaE);
	}
	
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

	MultiSceneCanvasInteractor::deleteAsyncDrawableObject(idInScene, originalAreaToDelete);
}

void MultiSceneCanvasManager::OnCanvasMouseDown(const MouseEvent& mouseE)
{
	DBG("passe par MultiSceneCanvasManager::OnCanvasMouseDown");
	MultiSceneCanvasEditor::OnCanvasMouseDown(mouseE);
}

void MultiSceneCanvasManager::OnCanvasMouseUp(const MouseEvent& mouseE)
{
	DBG("passe par MultiSceneCanvasManager::OnCanvasMouseUp");
	MultiSceneCanvasEditor::OnCanvasMouseUp(mouseE);
}

void MultiSceneCanvasManager::OnInteraction(std::shared_ptr<AreaEvent> areaE)
{
	handleAndSendAreaEventSync(areaE);
	//graphicSessionManager->HandleEventSync(areaE);
}



void MultiSceneCanvasManager::SetAllChannels()
{
	for (int j = 0; j < scenes.size(); ++j)
	{
		DBG("send new event");
		std::shared_ptr<SceneEvent> sceneE(new SceneEvent(shared_from_this(), scenes[j], SceneEventType::NothingHappened));
		handleAndSendEventSync(sceneE);
	}
}

void MultiSceneCanvasManager::resendToModel()
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		for (int i = 0; i < amusingScene->getNumberArea(); i++)
			handleAndSendEventSync(amusingScene->resendArea(i));
	}
}

void MultiSceneCanvasManager::ChangeBaseNote(double newBaseNote)
{
	DBG("new base = " + (String)newBaseNote);
	
}

void MultiSceneCanvasManager::ChangeSpeed(double newSpeed)
{
	//DBG("new speed = " + (String)newSpeed);
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		/*if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			myGraphicSessionManager->setSpeedArea(amusingScene->GetSelectedArea(), newSpeed);
		}*/
		//graphicSessionManager->setSpeedArea(amusingScene->GetSelectedArea(), newSpeed);
		//handleAndSendAreaEventSync(amusingScene->SetSelectedAreaCursor(newSpeed));
		if (auto selectedC = std::dynamic_pointer_cast<CompletePolygon>(amusingScene->GetSelectedArea()))
		{
			for (int i = 0; i < selectedC->getCursorsCount(); i++)
			{
				handleAndSendAreaEventSync(amusingScene->SetSelectedAreaCursor(i, newSpeed));
			}
		}
	}
}

double MultiSceneCanvasManager::getSpeed(std::shared_ptr<IEditableArea> area)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			return myGraphicSessionManager->getSpeed(amusingScene->GetSelectedArea());
		}
	}
	else
		return 1.0f;
}

void MultiSceneCanvasManager::ChangeVelocity(double newVelocity)
{
	//DBG("new speed = " + (String)newSpeed);
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			myGraphicSessionManager->setVelocityArea(amusingScene->GetSelectedArea(), newVelocity);
		}
		//graphicSessionManager->setSpeedArea(amusingScene->GetSelectedArea(), newSpeed);
		handleAndSendAreaEventSync(amusingScene->SetSelectedAreaOpacity(newVelocity/127.0));
	}
}

double MultiSceneCanvasManager::getVelocity(std::shared_ptr<IEditableArea> area)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			return myGraphicSessionManager->getVelocity(amusingScene->GetSelectedArea());
		}
	}
	else
		return 64.0;
}