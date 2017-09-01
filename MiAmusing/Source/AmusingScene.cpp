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
#include "Cursors.h"

#include "InteractiveScene.h"
#include "SceneEvent.h"
#include "MultiAreaEvent.h"

#include "boost\geometry.hpp"
#include "boost\geometry\geometries\geometries.hpp"
#include "boost\geometry\geometries\polygon.hpp"

#include <vector>

#include "AmusingSceneComponent.h"

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
	DBG("areas size" + (String)areas.size());
	//DBG("associate size " + (String)associateArea.size());
	for(int i = 0; i< currentExciters.size();i++)
		if (auto currentCursor = std::dynamic_pointer_cast<Cursor>(currentExciters[i]))
			if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
				manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(currentCursor, AreaEventType::Deleted, (int)areas.size() + i, shared_from_this())));
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
	//if (areas.size() > 0)
	//{
	//	for (int i = 0; i<(int)areas.size(); ++i)
	//	{
	//		if (auto hitP = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
	//		{
	//			if (auto singleAreaE = std::shared_ptr<AreaEvent>(newPolygon->intersection(hitP,nextAreaId)))
	//			{
	//				std::shared_ptr<AreaEvent> deleteE1;
	//				//std::shared_ptr<MultiAreaEvent> multiE;
	//				switch (singleAreaE->GetType())
	//				{
	//				case AreaEventType::Added:
	//					DBG("hitP->GetId()" + (String)hitP->GetId());
	//					DBG("nextAreaId" + (String)nextAreaId);
	//					// Fusion : need to add this area and to delete the 2 others (in fact just the first one, newPolygon had been created but not added so the smart pointer will do it for us
	//					//multiE = std::shared_ptr<MultiAreaEvent>(new MultiAreaEvent());
	//					//multiE->AddAreaEvent(AddArea(std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea())));
	//					deleteE1 = deleteAreaByUniqueId(hitP->GetId());
	//					//multiE->AddAreaEvent(deleteE1);
	//					if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
	//					{
	//						manager->OnFusion(deleteE1);
	//						//manager->OnFusion(deleteE2);
	//						return(AddArea(std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea())));

	//					}
	//					//return multiE;
	//					break;
	//				case AreaEventType::NothingHappened:
	//					// Intersection
	//				default:
	//					// wrong
	//					break;
	//				}
	//			}

	//		}
	//	}
	//}
	newPolygon->setCursorVisible(true, canvasComponent);
	std::shared_ptr<AreaEvent> areaE = AddArea(newPolygon);

	//DBG("a la creation : size = " + (String)newPolygon->GetContourSize());
	// Actual adding of this new polygon
	return areaE;
}

void AmusingScene::DeleteIntersections(std::shared_ptr<Amusing::CompletePolygon> parent)
{
	bool order = true;
	for (int i = 0; i < areas.size(); i++)
	{
		if (auto parent2 = std::dynamic_pointer_cast<CompletePolygon> (areas[i]))
		{
			if (parent == parent2)
				order = false;

			
			
			if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
			{
				// seek for the vector of intersection of the two polygons through the map
				std::map<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>::iterator it;
				if(order)
					it = parentTochildArea.find(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent, parent2));
				else
					it = parentTochildArea.find(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent2, parent));
				if (it != parentTochildArea.end())
				{
					// need to delete the intersections of the parent with the polygon
					DBG("suppression des " + (String)it->second.size() + " intersections liees");
					int vecSize = it->second.size();
					for (int j = vecSize -1; j >= 0; j--)
					{
						auto intersectionBackUp = it->second.at(j);
						it->second.erase(it->second.begin() + j);
						manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(intersectionBackUp, AreaEventType::Deleted, (int)areas.size() + j, shared_from_this())));
					}

					// then we erase the all line
					parentTochildArea.erase(it);
				}
				
			}
			
		}
	}
}


std::shared_ptr<AreaEvent> AmusingScene::DeleteSelectedArea()
{
	if (selectedArea)
	{
		auto selectedAreaBackup = selectedArea;
		std::vector<int> cursorToDeleteID;
		if (auto selectedCompleteArea = std::dynamic_pointer_cast<CompletePolygon>(selectedArea))
		{
			DeleteIntersections(selectedCompleteArea);
			for (int i = 0; i < currentExciters.size(); i++)
				if (auto currentCursor = std::dynamic_pointer_cast<Cursor>(currentExciters[i]))
					if (currentCursor->isLinkedTo(selectedCompleteArea))
					{
						currentCursor->LinkTo(nullptr);
						if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
							manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(currentCursor,AreaEventType::Deleted,(int)areas.size()+i,shared_from_this())));
						associateArea.erase(currentCursor);
						cursorToDeleteID.push_back(i);
					}
			for (int i = 0; i < cursorToDeleteID.size(); i++)
				currentExciters.erase(currentExciters.begin() + cursorToDeleteID[i]);
		}

		SetSelectedArea(nullptr);
		
		return deleteAreaByUniqueId(selectedAreaBackup->GetId());
	}
	else throw std::runtime_error("Impossible to delete the selected area (no area selected");
}

std::shared_ptr<AreaEvent> AmusingScene::DeleteCursor(std::shared_ptr<Cursor> cursorToDelete)
{
	return std::shared_ptr<AreaEvent>(new AreaEvent());
}

std::shared_ptr<GraphicEvent> AmusingScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
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
			if (areaE->GetType() == AreaEventType::Translation)
			{
				if (auto sceneComponent = (AmusingSceneComponent*)canvasComponent)
				{
					sceneComponent->SetAreaOptionsCenter(draggedArea->getCenter());
				}
			}

			if (lookForAreasInteractions(draggedArea))
				return graphicE;
			else
				return std::shared_ptr<AreaEvent>(new AreaEvent());
		}
		else
			return graphicE;
	}
	else
		return graphicE;
}

void AmusingScene::AddCursor()
{
	// creation du curseur
	bpt cursorCenter(0.0, 0.0);
	float cursorSize = 0.1f;
	std::shared_ptr<Cursor> newCursor(new Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, 1.47f)); //Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, 1.47f)); //Cursor(0));//Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, 1.47f));

	//ajouter le nouveau curseur à la liste de curseurs
	//cursors.push_back(newCursor);

	// il faut l'associer à une aire : chercher l'aire concernée ou la passer en paramètre
	// - dire à l'aire qu'elle est associée à une forme (voir si toujours nécessaire)
	// - garder une trace pour pvr les gérer dans une map<cursor,aire> ou <aire,cursor>

}

std::shared_ptr<AreaEvent> AmusingScene::AddCursor(std::shared_ptr<IDrawableArea> area)
{
	// creation du curseur
	bpt cursorCenter;
	if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(area))
		cursorCenter = completeArea->computeCursorCenter(0);
	else
		cursorCenter = bpt(0, 0);
	float cursorSize = 0.1f;
	std::shared_ptr<Cursor> newCursor(new Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, 1.47f)); //Cursor(0));//Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, 1.47f));

	//ajouter le nouveau curseur à la liste de curseurs
	//cursors.push_back(newCursor);
	std::shared_ptr<AreaEvent> areaE = AddExciter(newCursor);

	// il faut l'associer à une aire : chercher l'aire concernée ou la passer en paramètre
	// - dire à l'aire qu'elle est associée à une forme (voir si toujours nécessaire -> oui car c'est l'aire qui lui fournit sa vitesse...)
	if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(area))// association à l'aire donnant sa vitesse
	{
		completeArea->linkTo(newCursor);
		newCursor->LinkTo(completeArea);
	}
	// - garder une trace pour pvr les gérer dans une map<cursor,aire> ou <aire,cursor>
	associateArea[newCursor] = area; // association à l'aire qui déterminera sa position et donc le son produit

	newCursor->CanvasResized(canvasComponent);//trouver le nouveau centre

	//std::shared_ptr<AreaEvent> areaE(new AreaEvent(newCursor, AreaEventType::Added, (int)areas.size() + cursors.size() -1, shared_from_this()));
	return areaE;
}

std::shared_ptr<IDrawableArea> AmusingScene::getAssociateArea(std::shared_ptr<Cursor> cursor)
{
	if (associateArea.find(cursor) != associateArea.end())
		return associateArea[cursor];
	else
		return nullptr;
}

std::shared_ptr<AreaEvent> AmusingScene::AddIntersectionArea(std::shared_ptr<Amusing::CompletePolygon> parent1, std::shared_ptr<Amusing::CompletePolygon> parent2, std::shared_ptr<CompletePolygon> newIntersection)
{
	// voir s'il faut mettre ça dans AddAllIntersection, pour pas devoir rechercher à chaque fois 

	// add the areas to the map
	/*std::map<std::pair<std::shared_ptr<IEditableArea>, std::shared_ptr<IEditableArea>>, std::vector<std::shared_ptr<IEditableArea>>>::iterator it;
	it = parentTochildArea.find(std::pair<std::shared_ptr<IEditableArea>, std::shared_ptr<IEditableArea>>(parent1, parent2));
	if (it == parentTochildArea.end())
	{
		std::vector<std::shared_ptr<IEditableArea>> vec;
		parentTochildArea.insert(std::pair<std::pair<std::shared_ptr<IEditableArea>, std::shared_ptr<IEditableArea>>, std::vector<std::shared_ptr<IEditableArea>>>(std::pair<std::shared_ptr<IEditableArea>, std::shared_ptr<IEditableArea>>(parent1, parent2), vec));
	}
	it = parentTochildArea.find(std::pair<std::shared_ptr<IEditableArea>, std::shared_ptr<IEditableArea>>(parent1, parent2));
	it->second.push_back(newIntersection);*/

	currentIntersectionsAreas.push_back(newIntersection);

	// Forced graphical updates
	newIntersection->CanvasResized(canvasComponent);

	// WARNING
	// The id is the ID relative to all drawable objects....
	std::shared_ptr<AreaEvent> areaE(new AreaEvent(newIntersection, AreaEventType::Added, newIntersection->GetId(), shared_from_this()));
	areaE->SetMessage("intersection");
	return areaE;
}

void AmusingScene::AddAllIntersections(std::shared_ptr<Amusing::CompletePolygon> parent1, std::shared_ptr<Amusing::CompletePolygon> parent2, std::shared_ptr<MultiAreaEvent> multiE)
{
	if (multiE->GetOtherEventsCount() > 0)
	{
		if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
		{
			// seek for the vector of intersection of the two polygons through the map
			std::map<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>::iterator it;
			it = parentTochildArea.find(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2));
			if (it == parentTochildArea.end())
			{
				// If there are no intersections yet, add a new vector containing the intersections to the map
				// then add all the intersections to the list of intersections
				std::vector<std::shared_ptr<CompletePolygon>> vec;
				parentTochildArea.insert(std::pair<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2), vec));
				std::map<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>::iterator newIt;
				newIt = parentTochildArea.find(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2));
				for (int j = 0; j < multiE->GetOtherEventsCount(); j++)
				{
					//TO DO : add a size condition
					//vec.push_back(std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(j)->GetConcernedArea()));
					newIt->second.push_back(std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(j)->GetConcernedArea()));
					manager->OnInteraction(AddIntersectionArea(parent1, parent2, std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(j)->GetConcernedArea())));
				}
				//parentTochildArea.insert(std::pair<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2), vec));
			}
			else
			{
				// If there are already intersections between those polygon -> verify if it's just a modification of an existing intersections, or a new one
				//		compute the area of intersections of the previous intersections and the new intersections
				//		we use this area as similarity criterion
				
				std::vector<bool> intersectionStillExist(it->second.size(),false);
				for (int i = 0; i < multiE->GetOtherEventsCount(); i++)
				{
					bool needToAdd = true;
					for (int j = 0; j < it->second.size(); j++)
					{
						bpolygon newIntersection = std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(i)->GetConcernedArea())->getPolygon();
						bpolygon oldIntersection = it->second.at(j)->getPolygon();
						boost::geometry::correct(newIntersection);
						boost::geometry::correct(oldIntersection);
						// move this inside a function "similarityCriterion"
						double newArea = 0;
						std::deque<bpolygon> overlapArea;
						boost::geometry::intersection(newIntersection,oldIntersection, overlapArea);
						for (int k = 0; k < overlapArea.size(); k++)
						{
							newArea += boost::geometry::area(overlapArea[k]);
						}
						
						double oldArea = boost::geometry::area(oldIntersection);
						double ratio = newArea / oldArea;
						if (ratio> (1/10) && ratio < 10)
						{
							// similar enough -> need to modify the already existing intersection
							it->second.at(j)->Copy(std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(i)->GetConcernedArea()));
							std::shared_ptr<AreaEvent> areaEvent(new AreaEvent(it->second.at(j), AreaEventType::ShapeChanged, (int)areas.size() + j, shared_from_this()));
							areaEvent->SetMessage("intersection");
							manager->OnInteraction(areaEvent);

							// we don't need to add the intersection, and because we modify an existing intersection, this intersection still exists
							intersectionStillExist.at(j) = true;
							needToAdd = false;
							
						}
						

					}
					if (needToAdd == true)
					{
						DBG("new intersection to add");
						// the two areas are too different -> add the new area
						auto CA = std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(i)->GetConcernedArea());
						CA->setCursorVisible(false,canvasComponent);
						it->second.push_back(std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(i)->GetConcernedArea()));
						manager->OnInteraction(AddIntersectionArea(parent1, parent2, std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(i)->GetConcernedArea())));

					}
				}
				// delete the intersection no longer existing
				for (int i = 0; i < intersectionStillExist.size(); i++)
				{
					if (intersectionStillExist[i] == false)
					{
						auto intersectionBackUp = it->second.at(i);
						it->second.erase(it->second.begin() + i);
						manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(intersectionBackUp, AreaEventType::Deleted, (int)areas.size() + i, shared_from_this())));
					}
						
				}
			}

		}
	}
	else
	{
		if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
		{
			// seek for the vector of intersection of the two polygons through the map
			std::map<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>::iterator it;
			it = parentTochildArea.find(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2));
			if (it != parentTochildArea.end())
			{
				// previously there was intersections but they don't exist anymore
				// first we delete all the intersections and send the info to the manager
				for (int i = 0; i < it->second.size(); i++)
				{
					auto intersectionBackUp = it->second.at(i);
					it->second.erase(it->second.begin() + i);
					manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(intersectionBackUp, AreaEventType::Deleted, (int)areas.size() + i, shared_from_this())));
				}

				// then we erase the all line
				parentTochildArea.erase(it);

			}

		}
	}
}

void AmusingScene::ApplyFusion(std::shared_ptr<Amusing::CompletePolygon> currentPolygon, std::shared_ptr<Amusing::CompletePolygon> hitPolygon,std::shared_ptr<AreaEvent> singleAreaE)
{
	std::shared_ptr<AreaEvent> deleteE1;
	std::shared_ptr<AreaEvent> deleteE2;
	//std::shared_ptr<MultiAreaEvent> multiE;
	auto selectedAreaBackup = selectedArea;
	switch (singleAreaE->GetType())
	{
	case AreaEventType::Added:
		// Fusion : need to add this area and to delete the 2 others
		DBG("hitP->GetId()" + (String)hitPolygon->GetId());
		DBG("draggedArea->GetId()" + (String)currentPolygon->GetId());
		//multiE = std::shared_ptr<MultiAreaEvent>(new MultiAreaEvent());
		//multiE->AddAreaEvent(AddArea(std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea())));

		if (selectedArea)
		{
			//auto selectedAreaBackup = selectedArea;
			SetSelectedArea(nullptr);
		}
		DeleteIntersections(hitPolygon);
		DeleteIntersections(currentPolygon);
		deleteE1 = deleteAreaByUniqueId(hitPolygon->GetId());
		deleteE2 = deleteAreaByUniqueId(currentPolygon->GetId());
		//multiE->AddAreaEvent(deleteE1);
		//multiE->AddAreaEvent(deleteE2);

		// j'ai l'impression que �a reste extra laid, il y a d�j� les fonctions toutes faites pour add et delete, 
		// mais il faut les refaire juste pour pvr appeler le graphicSessionManager � chaque fois :/
		if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
		{
			manager->OnInteraction(deleteE1);
			manager->OnInteraction(deleteE2);
			manager->OnInteraction(AddArea(std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea())));
			//return  std::shared_ptr<AreaEvent>(new AreaEvent());

		}
		//canvasManager.lock()->handleAsyncUpdate 
		DBG("Fusion applied");
		//return multiE;
		//return std::shared_ptr<AreaEvent>(new AreaEvent());
	case AreaEventType::NothingHappened:
		// Intersection
		break;
	default:
		// wrong
		break;
	}
}

bool AmusingScene::getParents(std::shared_ptr<IEditableArea> child, std::shared_ptr<IEditableArea> &parent1, std::shared_ptr<IEditableArea> &parent2)
{
	if (auto childC = std::dynamic_pointer_cast<CompletePolygon>(child))
	{
		std::map<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>::iterator it;
		for (it = parentTochildArea.begin(); it != parentTochildArea.end(); it++)
		{
			for (int i = 0; i < it->second.size(); i++)
			{
				if (childC == it->second.at(i))
				{
					parent1 = it->first.first;
					parent2 = it->first.second;
					return true;
				}
			}
		}
		return false;
	}
	else
		return false;
}

bool AmusingScene::lookForAreasInteractions(std::shared_ptr<CompletePolygon> currentPolygon)
{
	// the function look for interactions, it returns true if the currentPolygon still exist at the end of the dunction or not
	bool parentAreaStillExisting = true;
	bool order = true; // for the storage of the intersections, the pair of parent must respect the order -> true if the order constraint is fulfil, false if not
	for (int i = 0; i < (int)areas.size(); ++i)
	{
		if (auto hitP = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
		{
			if (currentPolygon != hitP)
			{
				

				// compute intersections or the fusion
				if (auto singleAreaE = std::shared_ptr<AreaEvent>(currentPolygon->intersection(hitP, currentPolygon->GetId(), (int)areas.size())))
				{
					if (auto multiE = std::dynamic_pointer_cast<MultiAreaEvent>(singleAreaE))
					{

						// verify the order of the parents :
						if(order)
							AddAllIntersections(currentPolygon, hitP, multiE);
						else
							AddAllIntersections(hitP, currentPolygon, multiE);

					}
					else
					{
						ApplyFusion(currentPolygon, hitP, singleAreaE);
						parentAreaStillExisting = false;
					}

				}


			}
			else
				order = false; // order is no more respected
		}
	}
	return parentAreaStillExisting;
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
			if (lookForAreasInteractions(draggedArea))
				return graphicE;//std::shared_ptr<AreaEvent>(new AreaEvent());
			else
				return std::shared_ptr<AreaEvent>(new AreaEvent());

		}
		return graphicE;//std::shared_ptr<AreaEvent>(new AreaEvent(areaE->GetConcernedArea(), areaE->GetType(), areaE->GetAreaIdInScene(), shared_from_this()));
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
	areaE = std::shared_ptr<Miam::MultiAreaEvent>(new Miam::MultiAreaEvent());
	bool first = true;
	for (int i = 0; i < areas.size(); ++i)
	{
		//areaE = std::shared_ptr<Miam::MultiAreaEvent>(new Miam::MultiAreaEvent());
		if (auto completeA = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
		{
			//if (first == true)
			//{
			//	//completeA->setCursorVisible(true);
			//	areaE = std::shared_ptr<Miam::MultiAreaEvent>(new Miam::MultiAreaEvent(areas[i], Miam::AreaEventType::NothingHappened, areas[i]->GetId()));
			//	first = false;
			//}
			//completeA->setCursorVisible(true);
			//completeA->setReadingPosition(position);
			//areaE->AddAreaEvent(std::shared_ptr<AreaEvent>(new AreaEvent(areas[i], Miam::AreaEventType::NothingHappened)));
		}
	}
	/*for (int j = 0; j < currentIntersectionsAreas.size(); j++)
	{
		currentIntersectionsAreas[j]->setReadingPosition(position);
		areaE->AddAreaEvent(std::shared_ptr<AreaEvent>(new AreaEvent(currentIntersectionsAreas[j], Miam::AreaEventType::NothingHappened)));
	}*/
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

std::shared_ptr<AreaEvent> AmusingScene::SetSelectedArea(std::shared_ptr<IEditableArea> selectedArea_, bool changeMode)
{
	DBG("AmusingScene::SetSelectedArea");
	std::shared_ptr<AreaEvent> areaE =  EditableScene::SetSelectedArea(selectedArea_, changeMode);
	if (auto sceneComponent = (AmusingSceneComponent*)canvasComponent)
	{
		if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(selectedArea_))
		{
			sceneComponent->SetAreaOptionsCenter(completeArea->getCenter());
			double currentSpeed, currentVelocity;
			if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
			{
				currentSpeed = manager->getSpeed(completeArea);
				currentVelocity = manager->getVelocity(completeArea);
			}
			sceneComponent->SetAreaOptionsVisible(true,currentSpeed, currentVelocity);
		}
		else
			sceneComponent->SetAreaOptionsVisible(false);
	}
	return areaE;
}

std::shared_ptr<AreaEvent> AmusingScene::SetSelectedAreaCursor(int idx, double newSize)
{
	std::shared_ptr<AreaEvent> areaE(new AreaEvent());
	if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(selectedArea))
	{
		completeArea->setCursorsSpeed(idx, newSize);
		areaE = std::shared_ptr<AreaEvent>(new AreaEvent(completeArea->getCursor(idx),AreaEventType::ShapeChanged,completeArea->GetId(),shared_from_this()));
	}
	return areaE;
}

std::shared_ptr<AreaEvent> AmusingScene::SetSelectedAreaOpacity(double newOpacity)
{
	std::shared_ptr<AreaEvent> areaE(new AreaEvent());
	if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(selectedArea))
	{
		completeArea->SetAlpha(newOpacity);
		areaE = std::shared_ptr<AreaEvent>(new AreaEvent(completeArea, AreaEventType::ShapeChanged, completeArea->GetId(), shared_from_this()));
	}
	return areaE;
}