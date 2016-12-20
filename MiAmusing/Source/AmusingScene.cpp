/*
  ==============================================================================

    AmusingScene.cpp
    Created: 8 Dec 2016 2:56:07pm
    Author:  ayup1

  ==============================================================================
*/

#include "AmusingScene.h"

#include "MultiSceneCanvasInteractor.h"

#include "SceneCanvasComponent.h"


AmusingScene::AmusingScene(MultiSceneCanvasInteractor* _canvasManager, SceneCanvasComponent* _canvasComponent) : EditableScene(_canvasManager,  _canvasComponent)
{

}

std::string AmusingScene::OnCanvasMouseDown(Point<int> &clicLocation)
{
	DBG("OnCanvasMouseDown");

	std::string returnMsg;

	// When an area is already selected
	if (canvasManager->GetMode() == CanvasManagerMode::AreaSelected)
	{
		DBG("Area selected");
		// did we clic next to a point, or at least inside the area ?
		if (!selectedArea->TryBeginPointMove(clicLocation.toDouble()))
		{
			/* if not, we are sure that we clicked outside (checked by tryBeginPointMove)
			* => it is a DEselection (maybe selection of another just after this)
			*/
			SetSelectedArea(nullptr);
		}
	}
	// While no area is selected : we look for a new one to select,
	// starting from the area on the upper layer (last draw on canvas)
	for (int i = (int)(areas.size()) - 1; // int and not the *unsigned* size_t
		(i >= 0 && canvasManager->GetMode() == CanvasManagerMode::NothingSelected);
		i--)
	{
		if (areas[i]->HitTest(clicLocation.toDouble()))
		{
			// !!!!!!!!!!!!! TEST DES POIDS D'INTERACTION !!!!!!!!!!!
			//std::cout << "poids d'interaction = " << areasOrderedForDrawing[i]->ComputeInteractionWeight(clicLocation.toDouble()) << std::endl;

			// On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
			DBG("Set selected area");
			std::shared_ptr<EditableArea> areaToSelect = std::dynamic_pointer_cast<EditableArea>(areas[i]);
			if (areaToSelect)
				SetSelectedArea(areaToSelect);
			else
				throw std::runtime_error(std::string("Cannot cast the area that should be selected to a Miam::EditableArea"));
		}
	}
	// New point creation POLYGONE SEULEMENT, PAS LES AUTRES FORMES !!!!!
	// Le bouton doit d'ailleurs apparaître de manière dynamique....
	if (canvasManager->GetMode() == CanvasManagerMode::WaitingForPointCreation)
	{
		DBG("Waiting for point creation");
		// On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
		std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
		if (selectedPolygon)
		{
			if (!selectedPolygon->TryCreatePoint(clicLocation.toDouble()))
				returnMsg = "No point created : please click closer to an existing point !";
		}
		else
			throw std::runtime_error(std::string("Cannot create a point to the selected area (must be a polygon !)"));

		// No matter wether the point was created or not, we quit the creation and return
		// to the normal selected mode
		canvasManager->SetMode(CanvasManagerMode::AreaSelected);
	}
	// Existing point deletion
	else if (canvasManager->GetMode() == CanvasManagerMode::WaitingForPointDeletion)
	{
		DBG("Waiting for point deletion");
		// On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
		std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
		if (selectedPolygon)
		{
			returnMsg = (selectedPolygon->TryDeletePoint(clicLocation.toDouble())).toStdString();
		}
		else
			throw std::runtime_error(std::string("Cannot create a point to the selected area (must be a polygon !)"));

		canvasManager->SetMode(CanvasManagerMode::AreaSelected);
	}

	return returnMsg;
}

bool AmusingScene::OnCanvasMouseDrag(Point<int> &mouseLocation)
{
	//DBG("OnCanvasMouseDrag");
	if (selectedArea)
	{
		DBG("AmusingScene::OnCanvasMouseDrag");
		selectedArea->TryMovePoint(mouseLocation.toDouble());
		canvasComponent->repaint();
		DBG((String)selectedArea->GetPointDraggedId());
		switch (selectedArea->GetPointDraggedId())
		{
		case -10:
			DBG("No point selected");
			break;
		case -20:
			DBG("Whole area");
			break;
		case -30 :
			DBG("Center");
			break;
		case -40 :
			DBG("Manipulation Point");
			break;
		default:
			DBG("appler OnSurfaceChanged de GraphicSessionManager");
			canvasManager->OnSurfaceChanged(selectedArea->GetArea());

			break;
		}
		/*if (selectedArea->GetPointDraggedId() == -40)
		{
			DBG("appler OnSurfaceChanged de GraphicSessionManager");
			canvasManager->OnSurfaceChanged(selectedArea->GetArea());
		}*/
		
			
		//canvasManager->On
		return true;
	}
	else
		return false;
}

bool AmusingScene::OnCanvasMouseUp()
{
	//DBG("OnCanvasMouseUp");
	if (selectedArea)
	{
		selectedArea->EndPointMove();
		return true;
	}
	else
		return false;
}