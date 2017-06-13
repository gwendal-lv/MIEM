/*
  ==============================================================================

    AmusingScene.cpp
    Created: 8 Dec 2016 2:56:07pm
    Author:  ayup1

  ==============================================================================
*/

#include "AmusingScene.h"

#include "MultiSceneCanvasManager.h"
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
	newPolygon->setCursorVisible(true, canvasComponent);
	return AddArea(newPolygon);
}

std::shared_ptr<AreaEvent> AmusingScene::AddNedgeArea(uint64_t nextAreaId, int N)
{
	// centered grey Hexagon !...
	//DBG("creation du polygon a N cotes");
	std::shared_ptr<CompletePolygon> newPolygon(new CompletePolygon(nextAreaId,
		bpt(0.5f, 0.5f), N, 0.15f,
		Colours::grey,
		canvasComponent->GetRatio()));
	newPolygon->SetActive(false);
	newPolygon->CanvasResized(canvasComponent);
	if (areas.size() > 0)
	{
		for (int i = 0; i<(int)areas.size(); ++i)
		{
			if (auto hitP = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
			{
				if (auto singleAreaE = std::shared_ptr<AreaEvent>(newPolygon->intersection(hitP,nextAreaId)))
				{
					std::shared_ptr<AreaEvent> deleteE1;
					std::shared_ptr<MultiAreaEvent> multiE;
					switch (singleAreaE->GetType())
					{
					case AreaEventType::Added:
						DBG("hitP->GetId()" + (String)hitP->GetId());
						DBG("nextAreaId" + (String)nextAreaId);
						// Fusion : need to add this area and to delete the 2 others (in fact just the first one, newPolygon had been created but not added so the smart pointer will do it for us
						multiE = std::shared_ptr<MultiAreaEvent>(new MultiAreaEvent());
						multiE->AddAreaEvent(AddArea(std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea())));
						deleteE1 = deleteAreaByUniqueId(hitP->GetId());
						multiE->AddAreaEvent(deleteE1);
						return multiE;
						break;
					case AreaEventType::NothingHappened:
						// Intersection
					default:
						// wrong
						break;
					}
				}

			}
		}
	}
	newPolygon->setCursorVisible(true, canvasComponent);
	std::shared_ptr<AreaEvent> areaE = AddArea(newPolygon);

	//DBG("a la creation : size = " + (String)newPolygon->GetContourSize());
	// Actual adding of this new polygon
	return areaE;
}

std::shared_ptr<GraphicEvent> AmusingScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
	/*
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
	}*/

	if (allowAreaSelection)
	{
		//deleting = false;

		if (deleting)
		{
			std::shared_ptr<GraphicEvent> graphicE = EditableScene::OnCanvasMouseDown(mouseE);
			canvasComponent->setMouseCursor(MouseCursor::StandardCursorType::NormalCursor);
			deleting = false;
			return DeleteSelectedArea();
			
		}
		else
		{
			return EditableScene::OnCanvasMouseDown(mouseE);
		}
	}
	else
	{
		
		
			std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent());
			return graphicE;
		
	}

}

std::shared_ptr<GraphicEvent> AmusingScene::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
	std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent()); // default empty event
																// float position (more accurate) converted to double
	Point<double> mouseLocation = mouseE.position.toDouble();

	graphicE = EditableScene::OnCanvasMouseDrag(mouseE);
	
	if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(graphicE))
	{
		if (auto draggedArea = std::dynamic_pointer_cast<CompletePolygon>(areaE->GetConcernedArea()))
		{
			for (int i = 0; i < (int)areas.size(); ++i)
			{
				if (areaE->GetConcernedArea() != areas[i])
				{
					if (auto hitP = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
					{
						if (auto singleAreaE = std::shared_ptr<AreaEvent>(draggedArea->intersection(hitP,2)))
						{
							std::shared_ptr<AreaEvent> deleteE1;
							std::shared_ptr<MultiAreaEvent> multiE;
							switch (singleAreaE->GetType())
							{
							case AreaEventType::Added :
								// for the moment just do nothing, fusion only on mousedragstop (mouseUp)
								break;
							case AreaEventType::NothingHappened :
								// Intersection
							default:
								// wrong
								break;
							}
						}
												
					}
				}
			}
		}
		return std::shared_ptr<AreaEvent>(new AreaEvent(areaE->GetConcernedArea(), areaE->GetType(), areaE->GetAreaIdInScene(), shared_from_this()));
	}
	else
		return graphicE;
}

std::shared_ptr<GraphicEvent> AmusingScene::OnCanvasMouseUp(const MouseEvent& mouseE)
{
	std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent()); // default empty event
	Point<float> clicLocation = mouseE.position; // float position (more accurate)

	if (allowAreaSelection)
	{
		if (mouseE.source.getIndex() == 0)
		{
			if (selectedArea)
			{
				AreaEventType areaEventType = selectedArea->EndPointMove();
				graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, areaEventType, selectedArea->GetId(), shared_from_this()));
			}
		}
	}

	if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(graphicE))
	{
		if (auto draggedArea = std::dynamic_pointer_cast<CompletePolygon>(areaE->GetConcernedArea()))
		{
			for (int i = 0; i < (int)areas.size(); ++i)
			{
				if (areaE->GetConcernedArea() != areas[i])
				{
					if (auto hitP = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
					{
						if (auto singleAreaE = std::shared_ptr<AreaEvent>(draggedArea->intersection(hitP, 2)))
						{
							std::shared_ptr<AreaEvent> deleteE1;
							std::shared_ptr<AreaEvent> deleteE2;
							std::shared_ptr<MultiAreaEvent> multiE;
							auto selectedAreaBackup = selectedArea;
							switch (singleAreaE->GetType())
							{
							case AreaEventType::Added:
								// Fusion : need to add this area and to delete the 2 others
								DBG("hitP->GetId()" + (String)hitP->GetId());
								DBG("draggedArea->GetId()" + (String)draggedArea->GetId());
								//multiE = std::shared_ptr<MultiAreaEvent>(new MultiAreaEvent());
								//multiE->AddAreaEvent(AddArea(std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea())));
								
								if (selectedArea)
								{
									//auto selectedAreaBackup = selectedArea;
									SetSelectedArea(nullptr);
								}
								deleteE1 = deleteAreaByUniqueId(hitP->GetId());
								deleteE2 = deleteAreaByUniqueId(draggedArea->GetId());
								//multiE->AddAreaEvent(deleteE1);
								//multiE->AddAreaEvent(deleteE2);

								// j'ai l'impression que ça reste extra laid, il y a déjà les fonctions toutes faites pour add et delete, 
								// mais il faut les refaire juste pour pvr appeler le graphicSessionManager à chaque fois :/
								if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
								{
									manager->OnFusion(deleteE1);
									manager->OnFusion(deleteE2);
									manager->OnFusion(AddArea(std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea())));
									
								}
								//canvasManager.lock()->handleAsyncUpdate 
								DBG("ici");
								return multiE;
							case AreaEventType::NothingHappened:
								// Intersection
								break;
							default:
								// wrong
								break;
							}
						}

					}
				}
			}
		}
		return std::shared_ptr<AreaEvent>(new AreaEvent(areaE->GetConcernedArea(), areaE->GetType(), areaE->GetAreaIdInScene(), shared_from_this()));
	}

	return graphicE;
}

std::shared_ptr<AreaEvent> AmusingScene::AddTrueCircle(uint64_t nextAreaId)
{
	DBG("Creation du cercle");
	std::shared_ptr<EditableEllipse> newCircle(new EditableEllipse(nextAreaId,
		bpt(0.5f, 0.5f), 0.2f, 0.2f, Colours::grey, canvasComponent->GetRatio()));
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
		areaE = std::shared_ptr<Miam::MultiAreaEvent>(new Miam::MultiAreaEvent());
		if (auto completeA = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
		{
			//if (first == true)
			//{
			//	//completeA->setCursorVisible(true);
			//	areaE = std::shared_ptr<Miam::MultiAreaEvent>(new Miam::MultiAreaEvent(areas[i], Miam::AreaEventType::NothingHappened, areas[i]->GetId()));
			//	first = false;
			//}
			//completeA->setCursorVisible(true);
			completeA->setReadingPosition(position);
			areaE->AddAreaEvent(std::shared_ptr<AreaEvent>(new AreaEvent(areas[i], Miam::AreaEventType::NothingHappened)));
		}
	}
	//DBG("areaType = " + (String)((int)areaE->GetType()));
	if (areaE == nullptr)
		areaE = std::shared_ptr<MultiAreaEvent>(new MultiAreaEvent(nullptr,Miam::AreaEventType::NothingHappened));
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

std::shared_ptr<AreaEvent> AmusingScene::resendArea(int idx)
{
	return std::shared_ptr<AreaEvent>(new AreaEvent(areas[idx], AreaEventType::ShapeChanged,idx, shared_from_this()));
}

