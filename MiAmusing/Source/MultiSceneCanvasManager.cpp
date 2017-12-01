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

void MultiSceneCanvasManager::OnAudioPosition(double /*position*/)
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
		int hitAreaId = 0;
		if (amusingScene->isDrew(cursor) && cursor->setReadingPosition(position)) // vérifie si le curseur est dessiné et le met à jour (seulement si la condition "dessiné" est déjà vérifiée)
		{

			
				std::shared_ptr<AreaEvent> areaE(new AreaEvent(cursor, AreaEventType::NothingHappened, selectedScene));
				handleAndSendAreaEventSync(areaE);
			
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

void MultiSceneCanvasManager::handleAndSendMultiAreaEventSync(std::shared_ptr<MultiAreaEvent> multiAreaE)
{
	// Cast de l'évènement principal vers la classe mère
	auto mainAreaE = std::make_shared<AreaEvent>(multiAreaE.get());
	MultiSceneCanvasInteractor::handleAndSendAreaEventSync(mainAreaE, true); // pas de notif

												  // Dispatching des sous-évènements
	for (size_t i = 0; i<multiAreaE->GetOtherEventsCount(); i++)
	{
		MultiSceneCanvasInteractor::handleAndSendAreaEventSync(multiAreaE->GetOtherEvent(i), true); // pas de notif
	}

	// Ensuite on envoie le gros multiarea event au GraphicSessionManager en 1 paquet
	// (plutôt que chacun des petits séparément, pour lesquels la notification a été supprimée)
	graphicSessionManager->HandleEventSync(multiAreaE);
}

void MultiSceneCanvasManager::handleAndSendAreaEventSync(std::shared_ptr<AreaEvent> areaE)
{
	
	if (auto multiE = std::dynamic_pointer_cast<MultiAreaEvent>(areaE))
	{
		handleAndSendMultiAreaEventSync(multiE);
		
	}
	else
	{
		
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
				deleteAsyncDrawableObject(followerToDelete);
				// envoyer a l'audio que la source est plus la + retirer de la liste des followers?
			}
			else
				break;
		}
	}
}

void MultiSceneCanvasManager::deleteAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToDelete)
{

	MultiSceneCanvasInteractor::deleteAsyncDrawableObject(originalAreaToDelete);
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
	for (int j = 0; j < (int)scenes.size(); ++j)
	{
		DBG("send new event");
		std::shared_ptr<SceneEvent> sceneE(new SceneEvent(shared_from_this(), scenes[j], SceneEventType::NothingHappened));
		handleAndSendEventSync(sceneE);
	}
}



void MultiSceneCanvasManager::ChangeBaseNote(double newBaseNote)
{
	DBG("new base = " + (String)newBaseNote);
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			myGraphicSessionManager->setOctave(amusingScene->GetSelectedArea(), newBaseNote);
		}
		//graphicSessionManager->setSpeedArea(amusingScene->GetSelectedArea(), newSpeed);
		handleAndSendAreaEventSync(std::shared_ptr<AreaEvent>(new AreaEvent(amusingScene->GetSelectedArea(),AreaEventType::ShapeChanged,amusingScene)));//amusingScene->SetSelectedAreaOpacity(newVelocity / 127.0));
	}
}

void MultiSceneCanvasManager::ChangeSpeed(double newSpeed)
{
	//DBG("new speed = " + (String)newSpeed);
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			myGraphicSessionManager->setSpeedArea(amusingScene->GetSelectedArea(), newSpeed);
		}
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
		else
			return 1.0f;
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

void MultiSceneCanvasManager::ChangeColour(Colour newColour)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		/*if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			myGraphicSessionManager->setVelocityArea(amusingScene->GetSelectedArea(), newVelocity);
		}*/
		
		handleAndSendAreaEventSync(amusingScene->SetSelectedAreaColour(newColour));
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
		else
			return 64.0;
	}
	else
		return 64.0;
}

double MultiSceneCanvasManager::getOctave(std::shared_ptr<IEditableArea> area)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			return myGraphicSessionManager->getOctave(amusingScene->GetSelectedArea());
		}
		else
			return 0.0;
	}
	else
		return 64.0;
}

// - - - - - - - - - - running Mode - - - - - - - - - -

// The cases below are to be FORCED by the IGraphicSessionManager !
// Or by the canvasinteractor itself
void MultiSceneCanvasManager::SetMode(Miam::CanvasManagerMode newMode)
{
	/* Comportement assez asymétrique concernant les excitateurs.
	* Si on sort d'un mode excitateur, on arrête toutes les transfos (et on vérifie
	* ça hors du switch juste en-dessous, à la dégueu quoi (à améliorer))
	* Si on entre dans le mode excitateur : c'est géré dans le switch proprement
	*/
	if (
		(mode == CanvasManagerMode::ExcitersEdition || mode == CanvasManagerMode::ExciterSelected)
		&&
		(mode != CanvasManagerMode::ExcitersEdition && mode != CanvasManagerMode::ExciterSelected)
		)
	{
		selectedScene->StopCurrentTransformations();
	}

	// We don't do a specific action on every mode change !
	// But a few require checks and action
	switch (newMode) {

	case CanvasManagerMode::Unselected:
		// Unselection of a selected area (1 area selected for all canvases...)
		// And everything becomes dark, for another canvas to become
		if (selectedScene) // on first scene adding... there would be a problem
		{
			auto areaE = selectedScene->SetSelectedArea(nullptr, false);
			selectedScene->EnableExcitersLowOpacity(true);
			selectedScene->EnableAreasLowOpacity(true);
			// Pas d'évènements renvoyés pour les opacités : on update le tout
			recreateAllAsyncDrawableObjects();
			//handleAndSendAreaEventSync(areaE);
		}
		// Graphical updates
		if (mode != CanvasManagerMode::Unselected)
		{
			canvasComponent->SetIsSelectedForEditing(false);
		}
		break;

	case CanvasManagerMode::SceneOnlySelected:

		// à quoi servent VRAIMENT les lignes là-dessous ?
		/*if (selectedScene) // on first scene adding... there would be a problem
		GetSelectedScene()->SetSelectedArea(nullptr, false);*/
		// Graphical updates
		if (mode == CanvasManagerMode::Unselected)
		{
			canvasComponent->SetIsSelectedForEditing(true);
		}

		// Mise en quasi-transparence des excitateurs seulement
		if (selectedScene) // sinon pb à l'initialisation
		{
			
			selectedScene->EnableAreasLowOpacity(false);
			// Pas d'évènements renvoyés : on update le tout
			//recreateAllAsyncDrawableObjects();
		}
		break;

		// Quand on passe en mode excitateurs (on y passe forcément avant
		// d'en sélectionner un...), on arrête les transfos en cours
		// !! On doit vérifier qu'on était pas déjà en mode excitateurs !
	case CanvasManagerMode::ExcitersEdition:
		if (mode != CanvasManagerMode::ExciterSelected)
			selectedScene->StopCurrentTransformations();

		// Mise en quasi-transparence des aires graphiques à exciter seulement
		selectedScene->EnableExcitersLowOpacity(false);
		selectedScene->EnableAreasLowOpacity(true);
		// Pas d'évènements renvoyés : on update le tout
		recreateAllAsyncDrawableObjects();
		break;

		// Default case : we just apply the new mode
	default:
		break;
	}

	mode = newMode;


	graphicSessionManager->CanvasModeChanged(mode);
}
