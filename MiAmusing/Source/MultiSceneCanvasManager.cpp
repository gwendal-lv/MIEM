/*
  ==============================================================================

    MultiSceneCanvasManager.cpp
    Created: 10 Jan 2017 9:37:54pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MultiSceneCanvasManager.h"

#include "MultiSceneCanvasComponent.h"
#include "MultiSceneCanvasComponentAmusing.h"

#include "AmusingScene.h"

#include "CompletePolygon.h"
#include "TabCursor.h"

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

void MultiSceneCanvasManager::AddScene(std::string name,bool selectNewScene)
{
    // We construct scene that DO NOT allow the selection of a particular area
    //std::shared_ptr<EditableScene> newScene(new AmusingScene(this, canvasComponent->GetCanvas()));
	DBG("passe dans le bon");
	std::shared_ptr<AmusingScene> newScene(new AmusingScene(shared_from_this(), canvasComponent->GetCanvas()));
    newScene->SetName(name);
    
    MultiSceneCanvasInteractor::AddScene(newScene,selectNewScene);
}

void MultiSceneCanvasManager::AddNedgeArea(uint64_t nextAreaId, int N)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		std::shared_ptr<AreaEvent> areaE = amusingScene->AddNedgeArea(nextAreaId, N);
		handleAndSendAreaEventSync(areaE);
		amusingScene->AddIntersections(areaE->GetConcernedArea());
		handleAndSendAreaEventSync(amusingScene->AddCursor(areaE->GetConcernedArea()));
	}

}

void MultiSceneCanvasManager::AddTrueCircle(uint64_t nextAreaId)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
		handleAndSendAreaEventSync(amusingScene->AddTrueCircle(nextAreaId));
}

void MultiSceneCanvasManager::AddCompleteArea()
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
		handleAndSendAreaEventSync(amusingScene->AddCompleteArea(12));
}

void MultiSceneCanvasManager::AddAreaToScene(size_t sceneIndex, std::shared_ptr<IInteractiveArea> area_)
{
	SelectScene((int)sceneIndex); // lors du deuxième passage, le premier excitateur est supprimé
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(scenes[sceneIndex]))
	{
		if (auto amusingArea = std::dynamic_pointer_cast<CompletePolygon>(area_))
		{
			amusingScene->AddIntersections(amusingArea);
			addAreaToScene(scenes[sceneIndex], area_);
	
			handleAndSendAreaEventSync(amusingScene->AddCursor(area_));
		
			amusingArea->setCursorsSpeed(0, getSpeed(amusingArea));

			// evts pour mettre à jour du coté audio
			handleAndSendAreaEventSync(std::shared_ptr<AreaEvent>(new AreaEvent(amusingArea->getCursor(0), AreaEventType::ShapeChanged, selectedScene)));
			handleAndSendAreaEventSync(std::shared_ptr<AreaEvent>(new AreaEvent(amusingArea, AreaEventType::ShapeChanged, selectedScene)));
		}
	}
}

void MultiSceneCanvasManager::AddAllIntersectionsToScene(int sceneIndex)
{
	if (auto currentScene = std::dynamic_pointer_cast<AmusingScene>(scenes[sceneIndex]))
	{
		currentScene->AddAllIntersections();
	}
}

void MultiSceneCanvasManager::hideAddPolygon()
{
	if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
	{
		myGraphicSessionManager->hideAddPolygon();
	}
}

void MultiSceneCanvasManager::showAddPolygon()
{
	if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
	{
		myGraphicSessionManager->showAddPolygon();
	}
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
		if (amusingScene->isDrew(cursor) && cursor->setReadingPosition(position)) // vérifie si le curseur est dessiné et le met à jour (seulement si la condition "dessiné" est déjà vérifiée)
		{

			
				std::shared_ptr<AreaEvent> areaE(new AreaEvent(cursor, AreaEventType::NothingHappened, selectedScene));
				handleAndSendAreaEventSync(areaE);
			
		}
		/*if(auto completeA = std::dynamic_pointer_cast<CompletePolygon>(area))
			handleAndSendAreaEventSync(completeA->setReadingPosition(position));*/
	}
}

void MultiSceneCanvasManager::lookForAreasConcerned(Colour concernedColour)
{
	for (int i = 0; i < (int)scenes.size(); ++i)
	{
		if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(scenes[i]))
		{
			amusingScene->lookForAreasToUpdate(concernedColour);
		}
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

void MultiSceneCanvasManager::OnDeleteExciter()
{
	if (selectedScene)
	{
		if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
		{
			auto areaE = amusingScene->DeleteTabExciter();

			handleAndSendAreaEventSync(areaE);

			// Changement de mode : plus d'excitateur sélectionné.
			SetMode(CanvasManagerMode::ExcitersEdition);
		}
	}
	else throw std::runtime_error("Cannot delete an exciter : no scene selected");
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

void MultiSceneCanvasManager::resetAreaPosition()
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		std::shared_ptr<GraphicEvent> graphicE = amusingScene->resetAreaPosition();
		SetMode(CanvasManagerMode::AreaSelected);
		handleAndSendEventSync(graphicE);
		handleAndSendEventSync(amusingScene->DeleteTabExciter());
	}
}

std::shared_ptr<bptree::ptree> MultiSceneCanvasManager::GetTree()
{
	bptree::ptree scenesInnerTree;
	// Internal properties. Some others may be written, this function can be overriden
	auto canvasInnerTree = std::make_shared<bptree::ptree>();
	canvasInnerTree->put("<xmlattr>.index", selfId);
	canvasInnerTree->put("<xmlattr>.widthOnSaveTime", canvasComponent->getWidth());
	canvasInnerTree->put("<xmlattr>.heightOnSaveTime", canvasComponent->getHeight());
	canvasInnerTree->put("<xmlattr>.BPM", ((GraphicSessionManager*)graphicSessionManager)->getTempo());
	// Scenes writing
	for (size_t i = 0; i<scenes.size(); i++)
	{
		
		auto sceneTree = scenes[i]->GetTree();
		sceneTree->put("<xmlattr>.index", i);
		scenesInnerTree.add_child("scene", *sceneTree);
	}
	canvasInnerTree->add_child("scenes", scenesInnerTree);
	return canvasInnerTree;
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



void MultiSceneCanvasManager::deleteAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToDelete)
{

	MultiSceneCanvasInteractor::deleteAsyncDrawableObject(originalAreaToDelete);
}

void MultiSceneCanvasManager::OnCanvasMouseDown(const MouseEvent& mouseE)
{
	DBG("passe par MultiSceneCanvasManager::OnCanvasMouseDown");
	MultiSceneCanvasEditor::OnCanvasMouseDown(mouseE);
}

void MultiSceneCanvasManager::OnCanvasMouseDoubleClick(const MouseEvent & mouseE)
{
	// juste vérification si le double click a été effectué dans une aire ou pas
	// peut-être ajouter un calcul pour voir où faire apparaitre les options en fonction de comment est l'aire
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		std::shared_ptr<GraphicEvent> graphicE = amusingScene->OnCanvasMouseDoubleClick(mouseE);
		handleAndSendEventSync(graphicE);
		if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(graphicE))
		{
			if (areaE->GetType() == AreaEventType::Selected)
			{
				amusingScene->HideUnselectedAreas();
				if (auto amusingCanvas = (MultiSceneCanvasComponentAmusing*)canvasComponent)
				{
					if (auto selectedArea = std::dynamic_pointer_cast<CompletePolygon>(amusingScene->GetSelectedArea()))
					{
						amusingCanvas->showAreaOptions(true);
						SetMode(CanvasManagerMode::EditingArea);
						selectedArea->DisableTranslation(true);
					}
				}
			}
		}
	}
}

void MultiSceneCanvasManager::OnCanvasMouseUp(const MouseEvent& mouseE)
{
	DBG("passe par MultiSceneCanvasManager::OnCanvasMouseUp");
	std::shared_ptr<AmusingScene> amusingScene;
	switch (mode)
	{
	case CanvasManagerMode::PlayingWithExciters:
	case CanvasManagerMode::ExcitersEdition:
	case CanvasManagerMode::ExciterSelected:
		MultiSceneCanvasEditor::OnCanvasMouseUp(mouseE);
		if (amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
		{
			handleAndSendEventSync(std::shared_ptr<AreaEvent>(new AreaEvent(amusingScene->GetSelectedExciter(), AreaEventType::ShapeChanged, selectedScene)));
		}
		break;
	default :
		MultiSceneCanvasEditor::OnCanvasMouseUp(mouseE);
		break;
	}

	
}

void MultiSceneCanvasManager::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
	std::shared_ptr<GraphicEvent> graphicE;
	std::shared_ptr<AmusingScene> amusingScene;
	std::shared_ptr<Cursor> currentCursor;
	std::shared_ptr<CompletePolygon> completeP;
	switch (mode)
	{
	case Miam::CanvasManagerMode::PlayingWithExciters:
	case Miam::CanvasManagerMode::ExcitersEdition:
	case Miam::CanvasManagerMode::ExciterSelected:
		// when using the TabCursor
		if (amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
			graphicE = amusingScene->OnInteractiveCanvasMouseDrag(mouseE);
		else
			graphicE = selectedScene->InteractiveScene::OnCanvasMouseDrag(mouseE);
		if (auto multiE = std::dynamic_pointer_cast<MultiAreaEvent>(graphicE))
		{
			for (int i = 0; i < multiE->GetOtherEventsCount(); ++i)
			{
				if (auto areaEvent = std::dynamic_pointer_cast<AreaEvent>(multiE->GetOtherEvent(i)))
				{
					if (auto area = std::dynamic_pointer_cast<TabCursor>(areaEvent->GetConcernedArea()))
					{
						std::shared_ptr<AreaEvent> newAreaE(new AreaEvent());
						switch (currentOptionClicked)
						{
						case Volume:
							ChangeVelocity(area->getPercentage());
							handleAndSendEventSync(graphicE);
							newAreaE = std::shared_ptr<AreaEvent>(new AreaEvent(GetSelectedArea(), AreaEventType::ColorChanged, selectedScene));
							handleAndSendEventSync(newAreaE);
							break;
						case Speed:
							ChangeSpeed(speedTab[area->getIndexValue()]);
							handleAndSendEventSync(graphicE);
							newAreaE = std::shared_ptr<AreaEvent>(new AreaEvent(GetSelectedArea(), AreaEventType::ShapeChanged, selectedScene));
							handleAndSendEventSync(newAreaE);
							break;
						case Octave:
							ChangeBaseNote(area->getNearestDivision());
							handleAndSendEventSync(graphicE);
							newAreaE = std::shared_ptr<AreaEvent>(new AreaEvent(GetSelectedArea(), AreaEventType::ShapeChanged, selectedScene));
							handleAndSendEventSync(newAreaE);
							break;
						case Sample:
							ChangeColour(colorCode[(int)floor((1.0 - area->getPercentage()) * colorCode.size())], floor((1.0 - area->getPercentage()) * colorCode.size()));
							handleAndSendEventSync(graphicE);
							newAreaE = std::shared_ptr<AreaEvent>(new AreaEvent(GetSelectedArea(), AreaEventType::ShapeChanged, selectedScene));
							handleAndSendEventSync(newAreaE);
							break;
						default:
							break;
						}

					}
					else
					{
						handleAndSendEventSync(areaEvent);
					}
				}
			}
		}
		else if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(graphicE))
		{
			if (auto area = std::dynamic_pointer_cast<TabCursor>(areaE->GetConcernedArea()))
			{
				std::shared_ptr<AreaEvent> newAreaE(new AreaEvent());
				switch (currentOptionClicked)
				{
				case Volume:
					ChangeVelocity(area->getPercentage());
					handleAndSendEventSync(graphicE);
					newAreaE = std::shared_ptr<AreaEvent>(new AreaEvent(GetSelectedArea(), AreaEventType::ColorChanged, selectedScene));
					handleAndSendEventSync(newAreaE);
					break;
				case Speed:
					ChangeSpeed(speedTab[area->getIndexValue()]);
					handleAndSendEventSync(graphicE);
					newAreaE = std::shared_ptr<AreaEvent>(new AreaEvent(GetSelectedArea(), AreaEventType::ShapeChanged, selectedScene));
					handleAndSendEventSync(newAreaE);
					if (completeP = std::dynamic_pointer_cast<CompletePolygon>(GetSelectedArea()))
					{
						currentCursor = completeP->getCursor(0);
						newAreaE = std::shared_ptr<AreaEvent>(new AreaEvent(currentCursor, AreaEventType::ShapeChanged, selectedScene));
					}
					break;
				case Octave:
					ChangeBaseNote(area->getNearestDivision());
					handleAndSendEventSync(graphicE);
					newAreaE = std::shared_ptr<AreaEvent>(new AreaEvent(GetSelectedArea(), AreaEventType::ShapeChanged, selectedScene));
					handleAndSendEventSync(newAreaE);
					break;
				case Sample:
					ChangeColour(colorCode[(int)floor((1.0 - area->getPercentage()) * colorCode.size())], floor((1.0 - area->getPercentage()) * colorCode.size()));
					handleAndSendEventSync(graphicE);
					newAreaE = std::shared_ptr<AreaEvent>(new AreaEvent(GetSelectedArea(), AreaEventType::ShapeChanged, selectedScene));
					handleAndSendEventSync(newAreaE);
					break;
				default:
					break;
				}

			}
		}
		else
			handleAndSendEventSync(graphicE);
		break;
	default:
		MultiSceneCanvasInteractor::OnCanvasMouseDrag(mouseE);
		break;
	}
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



void MultiSceneCanvasManager::ChangeBaseNote(int newBaseNote)
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
			return myGraphicSessionManager->getSpeed(area);//amusingScene->GetSelectedArea());
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
			myGraphicSessionManager->setVelocityArea(amusingScene->GetSelectedArea(), newVelocity * 127.0);
		}
		//graphicSessionManager->setSpeedArea(amusingScene->GetSelectedArea(), newSpeed);
		handleAndSendAreaEventSync(amusingScene->SetSelectedAreaOpacity(newVelocity/**127.0*/));
	}
}

void MultiSceneCanvasManager::SetColorCode(int Nsamples, Colour _colorCode[])
{
	for (int i = 0; i < Nsamples; ++i)
		colorCode.push_back(_colorCode[i]);
}

void MultiSceneCanvasManager::ChangeColour(Colour newColour, int idx)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			myGraphicSessionManager->setColor(amusingScene->GetSelectedArea(), idx);
		}
		
		handleAndSendAreaEventSync(amusingScene->SetSelectedAreaColour(newColour));
	}
}

double MultiSceneCanvasManager::getVelocity(std::shared_ptr<IEditableArea> area)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			return myGraphicSessionManager->getVelocity(area);
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
			return myGraphicSessionManager->getOctave(area);
		}
		else
			return 0.0;
	}
	else
		return 64.0;
}

int MultiSceneCanvasManager::getCurrentColor(std::shared_ptr<IEditableArea> area)
{
	if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
	{
		if (auto myGraphicSessionManager = (GraphicSessionManager*)graphicSessionManager)
		{
			return myGraphicSessionManager->getColor(amusingScene->GetSelectedArea());
		}
		else
			return 0;
	}
	else
		return 0;
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
			//selectedScene->EnableExcitersLowOpacity(true);
			//selectedScene->EnableAreasLowOpacity(true);
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
			
			selectedScene->SetAreasOpacityMode(OpacityMode::Independent);//->EnableAreasLowOpacity(false);
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
		selectedScene->SetExcitersOpacityMode(OpacityMode::Independent);//->EnableExcitersLowOpacity(false);
		selectedScene->SetAreasOpacityMode(OpacityMode::Independent);//->EnableAreasLowOpacity(true);
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

void MultiSceneCanvasManager::SetEditingMode(OptionButtonClicked optionClicked)
{
	currentOptionClicked = optionClicked;
	if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(selectedScene->GetSelectedArea()))
	{
		std::shared_ptr<TabCursor> tabCursor;
		std::shared_ptr<Cursor> currentCursor;
		std::shared_ptr<AmusingScene> amusingScene;

		if (currentOptionClicked != Speed)
		{
			if (currentCursorSize != 1.0)
			{
				currentCursor = completeArea->getCursor(0);
				currentCursor->SizeChanged(1.0/currentCursorSize, false);
				currentCursorSize = 1.0;
			}
		}
		
			switch (currentOptionClicked)
			{
			case Sample:
				SetMode(CanvasManagerMode::ExciterSelected);
				completeArea->showAllTarget(false);
				completeArea->SetActive(false);
				completeArea->SetOpacityMode(OpacityMode::Independent);
				break;
			case Octave:
				if (tabCursor = std::dynamic_pointer_cast<TabCursor>(selectedScene->GetSelectedExciter()))
				{
					tabCursor->setPercentage(getOctave(completeArea) / 9.0 - 1.0/18.0);
					tabCursor->SetNumDivisions(9);
					tabCursor->EnableMagnet(true);
				}
				SetMode(CanvasManagerMode::ExciterSelected);
				completeArea->showAllTarget(false);
				completeArea->SetActive(false);
				completeArea->SetOpacityMode(OpacityMode::Independent);
				break;
			case Volume:
				if (tabCursor = std::dynamic_pointer_cast<TabCursor>(selectedScene->GetSelectedExciter()))
				{
					tabCursor->EnableMagnet(false);
					tabCursor->setPercentage(getVelocity(completeArea) / 128.0);
				}
				SetMode(CanvasManagerMode::ExciterSelected);
				completeArea->showAllTarget(false);
				completeArea->SetActive(false);
				completeArea->SetOpacityMode(OpacityMode::Independent);
				break;
			case Speed:
				//if (tabCursor = std::dynamic_pointer_cast<TabCursor>(selectedScene->GetSelectedExciter()))
				//{
				//	/*
				//	if (getSpeed(completeArea) > 1)
				//		tabCursor->setPercentage((getSpeed(completeArea) + 2) / 7.0 - 1.0 / 14.0);
				//	else
				//		tabCursor->setPercentage((4.0 - 1.0 / getSpeed(completeArea)) / 7.0 - 1.0 / 14.0);
				//	tabCursor->SetNumDivisions(7);
				//	tabCursor->EnableMagnet(true);
				//	*/
				//	tabCursor->EnableMagnet(false);
				//	tabCursor->setCenterPosition(completeArea->getPolygon().outer().at(0));
				//	tabCursor->updateContourPoints();
				//}
				//currentCursor = completeArea->getCursor(0);
				//currentCursorSize = 2.0;
				//currentCursor->SizeChanged(2.0, false);
				//currentCursor->SetAlpha(0.1);
				////currentCursor->SetEnableTranslationOnly(false);
				////currentCursor->SetActive(true);
				
				if (tabCursor = std::dynamic_pointer_cast<TabCursor>(selectedScene->GetSelectedExciter()))
				{
					tabCursor->setIndexValue(3);
				}

				completeArea->showAllTarget(false);
				SetMode(CanvasManagerMode::ExciterSelected);
				completeArea->SetActive(false);
				completeArea->SetOpacityMode(OpacityMode::Independent);

				if (amusingScene = std::dynamic_pointer_cast<AmusingScene>(selectedScene))
				{
					handleAndSendAreaEventSync(amusingScene->addShadowCursor());
				}

				currentCursor = completeArea->getCursor(0);
				currentCursorSize = 2.0;
				currentCursor->SizeChanged(2.0, false);
				currentCursor->SetAlpha(0.1);
				break;
			case Rhythm:
				completeArea->showAllTarget(true);
				completeArea->SetActive(true);
				SetMode(CanvasManagerMode::EditingArea);
				//completeArea->SetActive(true);
				break;
			case Closed:
				break;
			default:
				break;
			}
			if(currentCursor)
				handleAndSendAreaEventSync(std::shared_ptr<AreaEvent>(new AreaEvent(currentCursor, AreaEventType::Translation, selectedScene)));
			if (tabCursor)
			{
				handleAndSendAreaEventSync(std::shared_ptr<AreaEvent>(new AreaEvent(tabCursor, AreaEventType::Translation, selectedScene)));
			}
			handleAndSendAreaEventSync(std::shared_ptr<AreaEvent>(new AreaEvent(completeArea, AreaEventType::ShapeChanged, selectedScene)));
		
	}
}

void MultiSceneCanvasManager::UnselectScene()
{
	selectedScene = nullptr;
}
