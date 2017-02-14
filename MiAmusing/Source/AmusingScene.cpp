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
#include "EditableEllipse.h"
#include "Follower.h"

#include "InteractiveScene.h"

using namespace Miam;
using namespace Amusing;

AmusingScene::AmusingScene(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager, SceneCanvasComponent* _canvasComponent)
	: EditableScene(_canvasManager,  _canvasComponent,true)
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

std::shared_ptr<AreaEvent> AmusingScene::AddNedgeArea(uint64_t nextAreaId, int N)
{
	// centered grey Hexagon !...
	DBG("creation du polygon a N cotes");
	std::shared_ptr<AnimatedPolygon> newPolygon(new AnimatedPolygon(nextAreaId,
		Point<double>(0.5f, 0.5f), N, 0.15f,
		Colours::grey,
		canvasComponent->GetRatio()));
	DBG("a la creation : size = "+ (String)newPolygon->GetContourSize());
	// Actual adding of this new polygon
	return AddArea(newPolygon);
}

std::shared_ptr<GraphicEvent> AmusingScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
	if(allowAreaSelection)
		return EditableScene::OnCanvasMouseDown(mouseE);
	else
	{
		std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent());

	}

}

std::shared_ptr<AreaEvent> AmusingScene::AddTrueCircle(uint64_t nextAreaId)
{
	DBG("Creation du cercle");
	std::shared_ptr<EditableEllipse> newCircle(new EditableEllipse(nextAreaId,
		Point<double>(0.5f, 0.5f), 0.1f, 0.1f, Colours::grey, canvasComponent->GetRatio()));
	return AddArea(newCircle);
}

std::shared_ptr<AreaEvent> AmusingScene::AddFollower(uint64_t nextAreaId)
{
	std::shared_ptr<Follower> newFollower(new Follower(nextAreaId, Point<double>(0.5f, 0.5f), 0.1f,Colours::grey,canvasComponent->GetRatio(),shared_from_this()));
	return AddArea(newFollower);
}

std::shared_ptr<AnimatedPolygon> AmusingScene::getFirstArea()
{
	return std::dynamic_pointer_cast<AnimatedPolygon>(areas[0]);
}

std::shared_ptr<Amusing::AnimatedPolygon> AmusingScene::hitPolygon(Point<double> hitPoint)
{
	for (int i = 0; i < areas.size(); ++i)
	{
		if(areas[i]->HitTest(hitPoint))
			return std::dynamic_pointer_cast<AnimatedPolygon>(areas[i]);
	}
	return nullptr;
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