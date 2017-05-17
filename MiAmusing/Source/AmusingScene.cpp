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
#include "CompletePolygon.h"

#include "InteractiveScene.h"
#include "SceneEvent.h"
#include "MultiAreaEvent.h"

#include "boost\geometry.hpp"
#include "boost\geometry\geometries\geometries.hpp"
#include "boost\geometry\geometries\polygon.hpp"

typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;
typedef boost::geometry::model::polygon<bpt> bpolygon;

using namespace Miam;
using namespace Amusing;

AmusingScene::AmusingScene(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager, SceneCanvasComponent* _canvasComponent)
	: EditableScene(_canvasManager, _canvasComponent, true)
{
	Nfollower = 0;
	deleting = false;
	deleteEvent = std::shared_ptr<AreaEvent>(new AreaEvent(nullptr, AreaEventType::NothingHappened));
	
}

AmusingScene::~AmusingScene()
{

}

void AmusingScene::AddAnimatedArea(uint64_t nextAreaId)
{
	// centered grey Hexagon !...
	std::shared_ptr<AnimatedPolygon> newPolygon(new AnimatedPolygon(nextAreaId,
		bpt(0.5f, 0.5f), 6, 0.15f,
		Colours::grey,
		canvasComponent->GetRatio()));

	// Actual adding of this new polygon
	AddArea(newPolygon);
}

std::shared_ptr<AreaEvent> AmusingScene::AddCompleteArea(uint64_t nextAreaId)
{
	std::shared_ptr<CompletePolygon> newPolygon(new CompletePolygon(nextAreaId,
		bpt(0.5f, 0.5f), 6, 0.15f,
		Colours::grey,
		canvasComponent->GetRatio()));
	// Actual adding of this new polygon
	return AddArea(newPolygon);
}

std::shared_ptr<AreaEvent> AmusingScene::AddNedgeArea(uint64_t nextAreaId, int N)
{
	// centered grey Hexagon !...
	DBG("creation du polygon a N cotes");
	std::shared_ptr<CompletePolygon> newPolygon(new CompletePolygon(nextAreaId,
		bpt(0.5f, 0.5f), N, 0.15f,
		Colours::grey,
		canvasComponent->GetRatio()));
	if (areas.size() > 0)
	{
		for (int i = 0; i<(int)areas.size(); ++i)
		{
			if (auto hitP = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
			{
				std::vector<bpt> inter = newPolygon->intersection(hitP->getPolygon());
				//testDephasage = areas[i] // utiliser des box pr verifi a quel segment ca appartient et deduire le dephasage :)
				testDephasage = hitP->getPercentage(inter[0]);
				DBG("intersection #0 : " + (String)inter[0].get<0>() + " " + (String)inter[0].get<1>() + " = " + (String)testDephasage);
				// creer nouvelle forme pour chaque intersection
				DBG((String)inter.size() + " intersections");
				for (int j = 0; j < (int)inter.size(); ++j)
				{
					//DBG("intersection #" + (String)j + " : " + (String)boost::geometry::area(inter.at(j)));
					/*
					for (int k = 0; k < (int)inter.at(j).outer().size(); ++k)
					{

					}
					*/
				}
			}
		}
	}
	std::shared_ptr<AreaEvent> areaE = AddArea(newPolygon);

	//DBG("a la creation : size = " + (String)newPolygon->GetContourSize());
	// Actual adding of this new polygon
	return areaE;
}

std::shared_ptr<GraphicEvent> AmusingScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
	if (deleting)
	{
		DBG("AmusingScene::OnCanvasMouseDown -> deleting");
		// for tout les polygons, regarder si on a click dans un polygon et supprimer s'il faut
		for (int i = 0; i < (int)areas.size(); ++i)
		{
			if (areas[i]->HitTest(mouseE.position.toDouble().x, mouseE.position.toDouble().y))
			{
				// rajouter une verif si jamais deux polygone sont l'un au dessus de l'autre
				//std::shared_ptr<SceneEvent> sceneE(new SceneEvent(canvasManager.lock(),shared_from_this(),Miam::SceneEventType::SceneChanged));
				// supprimer la forme, et les follower associes
				//sceneE->SetMessage("Area deleted");
				//return sceneE;
				DBG("polygon hit");
				deleteEvent = std::shared_ptr<AreaEvent> (new AreaEvent(areas[i], AreaEventType::Deleted, (int)areas[i]->GetId(), shared_from_this()));
				DBG("delete Event : " + (String)((int)deleteEvent->GetType()));
				
				//std::shared_ptr<AreaEvent> areaE;
				//return areaE;
				return deleteEvent;
			}
		}
	}

	if (allowAreaSelection)
		return EditableScene::OnCanvasMouseDown(mouseE);
	else
	{
		
		
			std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent());
			return graphicE;
		
	}

}

std::shared_ptr<AreaEvent> AmusingScene::AddTrueCircle(uint64_t nextAreaId)
{
	DBG("Creation du cercle");
	std::shared_ptr<EditableEllipse> newCircle(new EditableEllipse(nextAreaId,
		bpt(0.5f, 0.5f), 0.1f, 0.1f, Colours::grey, canvasComponent->GetRatio()));
	return AddArea(newCircle);
}

std::shared_ptr<AreaEvent> AmusingScene::AddFollower(uint64_t nextAreaId)
{
	std::shared_ptr<Follower> newFollower(new Follower(nextAreaId, bpt(0.5f, 0.5f), 0.1f, Colours::grey, canvasComponent->GetRatio(), shared_from_this()));
	followers.push_back(newFollower);

	// Forced graphical updates
	newFollower->CanvasResized(canvasComponent);

	return std::make_shared<AreaEvent>(newFollower, AreaEventType::Added, nextAreaId, shared_from_this());
	
}

std::shared_ptr<Follower> AmusingScene::getFollowers(std::shared_ptr<Amusing::AnimatedPolygon> masterArea)
{
	for(int i=0;i<(int)followers.size();++i)
		if (followers[i]->isLinkTo(masterArea))
		{
			return followers[i];
		}
	return nullptr;
}

std::shared_ptr<AnimatedPolygon> AmusingScene::getFirstArea()
{
	return std::dynamic_pointer_cast<AnimatedPolygon>(areas[0]);
}

std::shared_ptr<Amusing::CompletePolygon> AmusingScene::getFirstCompleteArea()
{
	
	return std::dynamic_pointer_cast<CompletePolygon>(areas[0]);
}

std::shared_ptr<MultiAreaEvent> AmusingScene::SetAllAudioPositions(double position)
{
	std::shared_ptr<Miam::MultiAreaEvent> areaE;
	bool first = true;
	for (int i = 0; i < areas.size(); ++i)
	{
		if (auto completeA = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
		{
			if (first == true)
			{
				completeA->setCursorVisible(true);
				areaE = std::shared_ptr<Miam::MultiAreaEvent>(new Miam::MultiAreaEvent(areas[i], Miam::AreaEventType::NothingHappened, areas[i]->GetId()));
				first = false;
			}
			completeA->setReadingPosition(position);
			
		}
	}
	return areaE;
}

std::shared_ptr<AnimatedPolygon> AmusingScene::getNextArea()
{
	DBG("Nfollower = " + (String)Nfollower);
	++Nfollower;
	return std::dynamic_pointer_cast<AnimatedPolygon>(areas[(Nfollower - 1)]);
}

Point<double> AmusingScene::hitPolygon(Point<double> hitPoint, std::shared_ptr<Amusing::AnimatedPolygon> polygon, Point<double> P, Point<double> Q)
{
	/*for (int i = 0; i < areas.size(); ++i)
	{
		if (areas[i]->HitTest(hitPoint) && areas[i] != polygon)
		{
			if (auto anime = std::dynamic_pointer_cast<AnimatedPolygon> (areas[i]))
			{
				return anime->computeInteractionPoint(hitPoint, P, Q);
			}
		}

	}*/
	return Point<double>(0, 0);
}

/*
std::shared_ptr<Amusing::AnimatedPolygon> AmusingScene::hitPolygon(Point<double> hitPoint, std::shared_ptr<Amusing::AnimatedPolygon> polygon)
{
for (int i = 0; i < areas.size(); ++i)
{
if(areas[i]->HitTest(hitPoint) && areas[i] != polygon)
return std::dynamic_pointer_cast<AnimatedPolygon>(areas[i]);
}
return nullptr;
}
*/
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

int AmusingScene::getNumberArea()
{
	return areas.size();
}

std::shared_ptr<AreaEvent> AmusingScene::OnDelete()
{
	canvasComponent->setMouseCursor(MouseCursor::StandardCursorType::CrosshairCursor);
	deleting = true;
	//while (deleteEvent->GetType() != AreaEventType::Deleted)
	//{
		//DBG("waiting the click");
		// faire un wait pour pas rester bloquer la et voir le click
	//}
	return deleteEvent;
}