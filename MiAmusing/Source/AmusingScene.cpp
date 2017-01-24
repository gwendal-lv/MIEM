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
#include "AnimatedPolygon.h"

using namespace Miam;
using namespace Amusing;

AmusingScene::AmusingScene(MultiSceneCanvasInteractor* _canvasManager, SceneCanvasComponent* _canvasComponent) : EditableScene(_canvasManager,  _canvasComponent)
{

}

AmusingScene::~AmusingScene()
{

}

void AmusingScene::AddAnimatedArea(uint64_t nextAreaId)
{
	// centered grey Hexagon !...
	std::shared_ptr<AnimatedPolygon> newPolygon(new AnimatedPolygon(nextAreaId,
		Point<double>(0.5f, 0.5f), 6, 0.15f,
		Colours::grey,
		canvasComponent->GetRatio()));

	// Actual adding of this new polygon
	AddArea(newPolygon);
}

void AmusingScene::AddNedgeArea(uint64_t nextAreaId, int N)
{
	// centered grey Hexagon !...
	std::shared_ptr<AnimatedPolygon> newPolygon(new AnimatedPolygon(nextAreaId,
		Point<double>(0.5f, 0.5f), N, 0.15f,
		Colours::grey,
		canvasComponent->GetRatio()));

	// Actual adding of this new polygon
	AddArea(newPolygon);
}

std::shared_ptr<GraphicEvent> AmusingScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
	return EditableScene::OnCanvasMouseDown(mouseE);

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