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

AmusingScene::~AmusingScene()
{

}

/*
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
		//if (selectedArea->GetPointDraggedId() == -40)
		//{
			//DBG("appler OnSurfaceChanged de GraphicSessionManager");
			/canvasManager->OnSurfaceChanged(selectedArea->GetArea());
		//}
		
			
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
}*/