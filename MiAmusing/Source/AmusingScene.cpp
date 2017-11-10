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



#include <vector>

#include "AmusingSceneComponent.h"



using namespace Miam;
using namespace Amusing;

AmusingScene::AmusingScene(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager, SceneCanvasComponent* _canvasComponent)
	: EditableScene(_canvasManager, _canvasComponent, true)
{
	Nfollower = 0;
	deleting = false;
	deleteEvent = std::shared_ptr<AreaEvent>(new AreaEvent(nullptr, AreaEventType::NothingHappened));
	alreadyCursorInScene = false;
}

AmusingScene::~AmusingScene()
{
	DBG("areas size" + (String)areas.size());
	//DBG("associate size " + (String)associateArea.size());
	for(int i = 0; i< (int)currentExciters.size();i++)
		if (auto currentCursor = std::dynamic_pointer_cast<Cursor>(currentExciters[i]))
			if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
				manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(currentCursor, AreaEventType::Deleted, (int)areas.size() + i, shared_from_this())));
	for (int i = 0; i < (int)areas.size(); i++)
		if (auto currentArea = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
			currentArea->deleteAllCursors();
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
	newPolygon->KeepRatio(true);
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
	for (int i = 0; i < (int)areas.size(); i++)
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

				parent->resetChords();
				parent2->resetChords();
				
				// to send those update to the model
				manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(parent, AreaEventType::ShapeChanged, shared_from_this())));
				manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(parent2, AreaEventType::ShapeChanged, shared_from_this())));
				
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
			/*for (int i = 0; i < (int)currentExciters.size(); i++)
				if (auto currentCursor = std::dynamic_pointer_cast<Cursor>(currentExciters[i]))
					if (currentCursor->isLinkedTo(selectedCompleteArea))
					{
						currentCursor->LinkTo(nullptr);
						if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
							manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(currentCursor,AreaEventType::Deleted,(int)areas.size()+i,shared_from_this())));
						//associateArea.erase(currentCursor);
						cursorToDeleteID.push_back(i);
					}
			for (int i = 0; i < (int)cursorToDeleteID.size(); i++)
				currentExciters.erase(currentExciters.begin() + cursorToDeleteID[i]);*/
			int currentIdx = 0;
			while (currentIdx != (int)currentExciters.size())
			{
				for (int i = currentIdx; i < (int)currentExciters.size(); i++)
				{
					if (auto currentCursor = std::dynamic_pointer_cast<Cursor>(currentExciters[i]))
						if (currentCursor->isLinkedTo(selectedCompleteArea))
						{
							currentCursor->LinkTo(nullptr,false);
							if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
								manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(currentCursor, AreaEventType::Deleted, (int)areas.size() + i, shared_from_this())));
							currentIdx = i;
							currentExciters.erase(currentExciters.begin() + i);
						}

				}
			}
			selectedCompleteArea->deleteAllCursors();
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
			std::shared_ptr<GraphicEvent> graphE = EditableScene::OnCanvasMouseDown(mouseE);
			if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(graphE))
			{
				return graphE; 
			}
			else
			{
				for (int i = 0; i < (int)currentExciters.size(); i++)
				{
					if (auto currentCursor = std::dynamic_pointer_cast<Cursor>(currentExciters[i]))
					{
						if (currentCursor->isClicked(Point<double>((double)mouseE.x, (double)mouseE.y)))
						{
							graphE = std::shared_ptr<AreaEvent>(new AreaEvent(currentExciters[i], AreaEventType::Selected, shared_from_this()));
							break;
						}
					}
					
					
				}
				return graphE;
				DBG("pas d'interaction avec une aire");
			}
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
	std::shared_ptr<Cursor> newCursor(new Cursor(0, cursorCenter, cursorSize, Colours::grey, 1.47f)); //Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, 1.47f)); //Cursor(0));//Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, 1.47f));

	//ajouter le nouveau curseur à la liste de curseurs
	//cursors.push_back(newCursor);

	// il faut l'associer à une aire : chercher l'aire concernée ou la passer en paramètre
	// - dire à l'aire qu'elle est associée à une forme (voir si toujours nécessaire)
	// - garder une trace pour pvr les gérer dans une map<cursor,aire> ou <aire,cursor>

}

std::shared_ptr<AreaEvent> AmusingScene::AddCursor(std::shared_ptr<IDrawableArea> area)
{
	//if (!alreadyCursorInScene)
	//{
		// creation du curseur
		bpt cursorCenter;
		if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(area))
			cursorCenter = completeArea->computeLinearCursorCenter(0);
		else
			cursorCenter = bpt(0, 0);
		float cursorSize = 0.1f;
		std::shared_ptr<Cursor> newCursor(new Cursor(0, cursorCenter, cursorSize, Colours::grey, canvasComponent->GetRatio())); //Cursor(0));//Cursor(0, cursorCenter, cursorSize, cursorSize, Colours::grey, 1.47f));

		//ajouter le nouveau curseur à la liste de curseurs
		//cursors.push_back(newCursor);
		std::shared_ptr<AreaEvent> areaE = AddExciter(newCursor,false);

		// il faut l'associer à une aire : chercher l'aire concernée ou la passer en paramètre
		// - dire à l'aire qu'elle est associée à une forme (voir si toujours nécessaire -> oui car c'est l'aire qui lui fournit sa vitesse...)
		if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(area))// association à l'aire donnant sa vitesse
		{
			completeArea->linkTo(newCursor);
			newCursor->LinkTo(completeArea,false);
		}
		// - garder une trace pour pvr les gérer dans une map<cursor,aire> ou <aire,cursor>
		//associateArea[newCursor] = area; // association à l'aire qui déterminera sa position et donc le son produit

		newCursor->CanvasResized(canvasComponent);//trouver le nouveau centre

		alreadyCursorInScene = true;
		//std::shared_ptr<AreaEvent> areaE(new AreaEvent(newCursor, AreaEventType::Added, (int)areas.size() + cursors.size() -1, shared_from_this()));
		return areaE;
	//}
	//else
		//return std::shared_ptr<AreaEvent>(new AreaEvent());
}

//std::shared_ptr<IDrawableArea> AmusingScene::getAssociateArea(std::shared_ptr<Cursor> cursor)
//{
//	if (associateArea.find(cursor) != associateArea.end())
//		return associateArea[cursor];
//	else
//		return nullptr;
//}

void AmusingScene::addChords(std::shared_ptr<Amusing::CompletePolygon> parent1, std::shared_ptr<Amusing::CompletePolygon> parent2, std::shared_ptr<MultiAreaEvent> multiE)
{
	if (multiE->GetOtherEventsCount() > 0)
	{
		for (int j = 0; j < (int)multiE->GetOtherEventsCount(); j++)
		{
			if (auto inter = std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(j)->GetConcernedArea()))
			{
				inter->CanvasResized(canvasComponent);
				bpolygon interP = inter->getPolygon();
				bpolygon correctP1 = parent1->getPolygon();
				boost::geometry::correct(correctP1);
				bpolygon correctP2 = parent2->getPolygon();
				boost::geometry::correct(correctP2);
				for (int i = 0; i < (int)interP.outer().size(); ++i)
				{
					if (boost::geometry::within(interP.outer().at(i), correctP1)) // dans P1 --> appartient à P2
					{
						if (parent1 == parent2)
							DBG("impossible");
						parent2->setChordFlag(interP.outer().at(i), true, parent1);
					}
					else if(boost::geometry::within(interP.outer().at(i), correctP2))// dans P2 --> appartient à P1
					{
						if (parent1 == parent2)
							DBG("impossible");
						parent1->setChordFlag(interP.outer().at(i), true, parent2);
					}
					else
					{
						if (parent1 == parent2)
							DBG("impossible");
						parent1->addChordPoints(parent1->getAngularPercentage(interP.outer().at(i)), parent2);
						parent2->addChordPoints(parent2->getAngularPercentage(interP.outer().at(i)), parent1);
					}
				}
			}
		}
	}
}

std::shared_ptr<AreaEvent> AmusingScene::AddIntersectionArea(std::shared_ptr<Amusing::CompletePolygon> parent1, std::shared_ptr<Amusing::CompletePolygon> parent2, std::shared_ptr<CompletePolygon> newIntersection)
{
	// voir s'il faut mettre ça dans AddAllIntersection, pour pas devoir rechercher à chaque fois 

	

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
			//if (it == parentTochildArea.end())
			//{
			if (it != parentTochildArea.end())
			{
				while((int)it->second.size() != 0)
				{
					int i = 0;
					auto intersectionBackUp = it->second.at(i);
					it->second.erase(it->second.begin() + i);
					auto areaE = std::shared_ptr<AreaEvent>(new AreaEvent(intersectionBackUp, AreaEventType::Deleted, (int)areas.size() + i, shared_from_this()));
					areaE->SetMessage("intersection");
					manager->OnInteraction(areaE);
					

				}
			}
				// If there are no intersections yet, add a new vector containing the intersections to the map
				// then add all the intersections to the list of intersections
			std::vector<std::shared_ptr<CompletePolygon>> vec;
			parentTochildArea.insert(std::pair<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2), vec));
			std::map<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>::iterator newIt;
			newIt = parentTochildArea.find(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2));
			for (int j = 0; j < (int)multiE->GetOtherEventsCount(); j++)
			{
				//TO DO : add a size condition
				//vec.push_back(std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(j)->GetConcernedArea()));
				newIt->second.push_back(std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(j)->GetConcernedArea()));
				manager->OnInteraction(AddIntersectionArea(parent1, parent2, std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(j)->GetConcernedArea())));
				//manager->OnInteraction(AddCursor(std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(j)->GetConcernedArea())));
			}
				//parentTochildArea.insert(std::pair<std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>, std::vector<std::shared_ptr<CompletePolygon>>>(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2), vec));
			//}
			/*
			else
			{
				// If there are already intersections between those polygon -> verify if it's just a modification of an existing intersections, or a new one
				//		compute the area of intersections of the previous intersections and the new intersections
				//		we use this area as similarity criterion
				
				std::vector<bool> intersectionStillExist(it->second.size(),false);
				for (int i = 0; i < (int)multiE->GetOtherEventsCount(); i++)
				{
					bool needToAdd = true;
					for (int j = 0; j < (int)it->second.size(); j++)
					{
						bpolygon newIntersection = std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(i)->GetConcernedArea())->getPolygon();
						bpolygon oldIntersection = it->second.at(j)->getPolygon();
						boost::geometry::correct(newIntersection);
						boost::geometry::correct(oldIntersection);
						// move this inside a function "similarityCriterion"
						double newArea = 0;
						std::deque<bpolygon> overlapArea;
						boost::geometry::intersection(newIntersection,oldIntersection, overlapArea);
						for (int k = 0; k < (int)overlapArea.size(); k++)
						{
							newArea += boost::geometry::area(overlapArea[k]);
						}
						
						double oldArea = boost::geometry::area(oldIntersection);
						double ratio = newArea / oldArea;
						if (ratio> (1/10) && ratio < 10)
						{
							// similar enough -> need to modify the already existing intersection
							it->second.at(j)->Copy(std::dynamic_pointer_cast<CompletePolygon>(multiE->GetOtherEvent(i)->GetConcernedArea()));
							std::shared_ptr<AreaEvent> areaEvent(new AreaEvent(it->second.at(j), AreaEventType::NothingHappened, (int)areas.size() + j, shared_from_this()));
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
				for (int i = 0; i < (int)intersectionStillExist.size(); i++)
				{
					if (intersectionStillExist[i] == false)
					{
						auto intersectionBackUp = it->second.at(i);
						it->second.erase(it->second.begin() + i);
						manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(intersectionBackUp, AreaEventType::Deleted, (int)areas.size() + i, shared_from_this())));
					}
						
				}
			}
			*/
			/// chords part
			// update the polygons with the points for the chords
			parent1->resetChords();
			parent2->resetChords();
			addChords(parent1, parent2, multiE);
			// to send those update to the model
			manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(parent1, AreaEventType::ShapeChanged, shared_from_this())));
			manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(parent2, AreaEventType::ShapeChanged, shared_from_this())));

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
				for (int i = 0; i < (int)it->second.size(); i++)
				{
					auto intersectionBackUp = it->second.at(i);
					it->second.erase(it->second.begin() + i);
					auto areaE = std::shared_ptr<AreaEvent>(new AreaEvent(intersectionBackUp, AreaEventType::Deleted, (int)areas.size() + i, shared_from_this()));
					areaE->SetMessage("intersection");
					manager->OnInteraction(areaE);
				}

				// then we erase the all line
				parentTochildArea.erase(it);

				// update chords
				parent1->resetChords();
				parent2->resetChords();
				
				// to send those update to the model
				manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(parent1, AreaEventType::ShapeChanged, shared_from_this())));
				manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(parent2, AreaEventType::ShapeChanged, shared_from_this())));
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
			SetSelectedArea(nullptr,false);
		}

		// j'ai l'impression que �a reste extra laid, il y a d�j� les fonctions toutes faites pour add et delete, 
		// mais il faut les refaire juste pour pvr appeler le graphicSessionManager � chaque fois :/
		if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
		{
			
			if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea()))
			{
				complete->CanvasResized(canvasComponent);
				for (int i = 0; i < currentPolygon->getCursorsCount(); ++i)
				{
					currentPolygon->getCursor(i)->LinkTo(complete,false);
					complete->linkTo(currentPolygon->getCursor(i));
				}
				for (int i = 0; i < hitPolygon->getCursorsCount(); ++i)
				{
					hitPolygon->getCursor(i)->LinkTo(complete,false);
					complete->linkTo(hitPolygon->getCursor(i));
				}
			}

			DeleteIntersections(hitPolygon);
			DeleteIntersections(currentPolygon);
			hitPolygon->deleteAllCursors();
			deleteE1 = deleteAreaByUniqueId(hitPolygon->GetId());
			currentPolygon->deleteAllCursors();
			deleteE2 = deleteAreaByUniqueId(currentPolygon->GetId());
			//multiE->AddAreaEvent(deleteE1);
			//multiE->AddAreaEvent(deleteE2);
					

			

			manager->OnInteraction(deleteE1);
			manager->OnInteraction(deleteE2);
			manager->OnInteraction(AddArea(std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea())));

			if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(singleAreaE->GetConcernedArea()))
			{
				complete->CanvasResized(canvasComponent);
				for (int i = 0; i < complete->getCursorsCount(); ++i)
				{
					
					manager->OnInteraction(std::shared_ptr<AreaEvent>(new AreaEvent(complete->getCursor(i), AreaEventType::ParentChanged, shared_from_this())));
				}
				
			}
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
			for (int i = 0; i < (int)it->second.size(); i++)
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

std::shared_ptr<CompletePolygon> AmusingScene::getConcernedIntersection(std::shared_ptr<CompletePolygon> parent1, std::shared_ptr<CompletePolygon> parent2, bpt hitPoint)
{
	// cherche si une intersection existe entre parent1 et parent2, et renvoie l'intersection contenant le hitPoint
	// d'abord on recherche avec les parents dans le sens 1, 2
	auto it = parentTochildArea.find(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2));
	if (it != parentTochildArea.end())
	{
		if (it->second.size() == 0)
			return nullptr;
		else if (it->second.size() == 1) // s'il n'y a qu'une intersection c'est forcément celle_là
			return it->second[0];
		else
		{
			// trouver la bonne intersection
			int prox = 0;
			double minDistance = 1000;
			for (int i = 0; i < (int)it->second.size(); ++i)
			{
				double distance = boost::geometry::distance(hitPoint, it->second[i]->getPolygon());
				if (distance < minDistance)
				{
					minDistance = distance;
					prox = i;
				}
			}
			return it->second[prox];
		}
	}
	else
	{
		// si on a pas trouvé d'intersections dans le sens 1, 2, on cherche dans le sens 2, 1
		it = parentTochildArea.find(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent2, parent1));
		if (it != parentTochildArea.end())
		{
			if (it->second.size() == 0)
				return nullptr;
			else if (it->second.size() == 1)
				return it->second[0];
			else
			{
				// trouver la bonne intersection
				int prox = 0;
				double minDistance = 1000;
				for (int i = 0; i < (int)it->second.size(); ++i)
				{
					double distance = boost::geometry::distance(hitPoint, it->second[i]->getPolygon());
					if (distance < minDistance)
					{
						minDistance = distance;
						prox = i;
					}
				}
				return it->second[prox];
			}
		}
		else
			return nullptr;
	}
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
		bpt(0.5f, 0.5f), 0.2f, Colours::grey, canvasComponent->GetRatio()));
	newCircle->KeepRatio(true);
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

std::shared_ptr<MultiAreaEvent> AmusingScene::SetAllAudioPositions(double /*position*/)
{
	
	std::shared_ptr<Miam::MultiAreaEvent> areaE;
	areaE = std::shared_ptr<Miam::MultiAreaEvent>(new Miam::MultiAreaEvent());
	bool first = true;
	for (int i = 0; i < (int)areas.size(); ++i)
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


std::shared_ptr<AreaEvent> AmusingScene::SetSelectedArea(std::shared_ptr<IEditableArea> selectedArea_, bool changeMode)
{
	DBG("AmusingScene::SetSelectedArea");
	std::shared_ptr<AreaEvent> areaE =  EditableScene::SetSelectedArea(selectedArea_, changeMode);
	if (auto sceneComponent = (AmusingSceneComponent*)canvasComponent)
	{
		if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(selectedArea_))
		{
			sceneComponent->SetAreaOptionsCenter(completeArea->getCenter());
			double currentSpeed(0), currentVelocity(0);
			int currentOctave(0);
			if (auto manager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManager.lock()))
			{
				currentSpeed = manager->getSpeed(completeArea);
				currentVelocity = manager->getVelocity(completeArea);
				currentOctave = manager->getOctave(completeArea);
				DBG("speed to show = " + (String)currentSpeed);
			}
			sceneComponent->SetAreaOptionsVisible(true,currentSpeed, currentVelocity, currentOctave);
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
		completeArea->SetAlpha((float)newOpacity);
		areaE = std::shared_ptr<AreaEvent>(new AreaEvent(completeArea, AreaEventType::ColorChanged, completeArea->GetId(), shared_from_this()));
	}
	return areaE;
}

bool AmusingScene::isDrew(std::shared_ptr<Cursor> cursor)
{
	for (int i = 0; i < (int)currentExciters.size(); ++i)
		if (currentExciters[i] == cursor)
			return true;
	return false;
}

std::shared_ptr<AreaEvent> AmusingScene::checkCursorPosition(std::shared_ptr<Cursor> cursor, int &areaId)
{
	for (int i = 0; i < (int)areas.size(); i++)
	{
		if (auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(areas[i]))
			if (completeArea->contains(cursor->getPosition()) && !cursor->isLinkedTo(completeArea) && cursor->CanLinkTo(completeArea))// si la position du curseur est dans une autre aire ->
			{
				areaId = i;
				return std::shared_ptr<AreaEvent>(new AreaEvent(completeArea, AreaEventType::ParentChanged, (int)completeArea->GetId(), shared_from_this()));
			}
	}
	return nullptr;
}

//size_t AmusingScene::GetDrawableObjectsCount()
//{
//	int numInter = 0;
//	for (int i = 0; i < areas.size(); i++)
//	{
//		if (auto parent1 = std::dynamic_pointer_cast<CompletePolygon> (areas[i]))
//		{
//			for (int j = i + 1; j < areas.size(); j++)
//			{
//				if (auto parent2 = std::dynamic_pointer_cast<CompletePolygon>(areas[j]))
//				{
//					if (parentTochildArea.find(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2)) != parentTochildArea.end())
//					{
//						numInter += parentTochildArea.at(std::pair<std::shared_ptr<CompletePolygon>, std::shared_ptr<CompletePolygon>>(parent1, parent2)).size();
//					}
//				}
//			}
//		}
//	}
//	return areas.size() + currentExciters.size() + numInter;
//}
//std::shared_ptr<IDrawableArea> AmusingScene::GetDrawableObject(size_t i)
//{
//	if (i < areas.size())
//		return areas[i];
//	else if (i < areas.size() + currentExciters.size())
//		return currentExciters[i - areas.size()];
//	else
//	{
//		return parentTochildArea.begin()->
//	}
//}