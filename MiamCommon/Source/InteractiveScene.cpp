/*
  ==============================================================================

    InteractiveScene.cpp
    Created: 17 Nov 2016 4:53:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InteractiveScene.h"

#include "DrawableArea.h"

#include "MultiSceneCanvasInteractor.h"
#include "InteractiveArea.h"
#include "Exciter.h"

#ifdef _MSC_VER
	#include "Windows.h" // for OutputDebugString(), std cout does not work...
#endif

using namespace Miam;


// - - - - - Construction and Destruction (and helpers) - - - - -

InteractiveScene::InteractiveScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_, ExcitersBehaviorType excitersBehavior_)
:
canvasManager(canvasManager_),
canvasComponent(canvasComponent_),
excitersBehavior(excitersBehavior_)
{
    name = "Default Scene";
}


InteractiveScene::~InteractiveScene()
{
    
}






// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

size_t InteractiveScene::GetDrawableObjectsCount()
{
    return areas.size() + currentExciters.size();
}
std::shared_ptr<IDrawableArea> InteractiveScene::GetDrawableObject(size_t i)
{
    if (i < areas.size())
        return areas[i];
    else
        return currentExciters[i-areas.size()];
}
size_t InteractiveScene::GetInteractiveAreasCount()
{
    return areas.size();
}
std::shared_ptr<IInteractiveArea> InteractiveScene::GetInteractiveArea(size_t i)
{
    return areas[i];
}

void InteractiveScene::SetName(std::string _name)
{
    name = _name;
    
    // graphic updates called from parent (all scenes at the same time...)
}



// = = = = = = = = = = METHODS = = = = = = = = = =




// - - - - - Areas Managing : Add and Delete - - - - -


std::shared_ptr<AreaEvent> InteractiveScene::AddArea(std::shared_ptr<IInteractiveArea> newArea)
{
    areas.push_back(newArea);
    
    // Forced graphical updates
    newArea->CanvasResized(canvasComponent);
    
    // Warning : does not contain the shared_ptr to the scene
    return std::shared_ptr<AreaEvent>(new AreaEvent(newArea,
                                                    AreaEventType::Added,
                                                    (int)areas.size()-1));
}


// - - - - - Selection events managing (orders from parent manager) - - - - -
void InteractiveScene::OnSelection()
{
}
void InteractiveScene::OnUnselection()
{
	// We stop all current movements
    // and filter all future undesired touch events
	for (auto it = touchSourceToEditableArea.begin();
		it != touchSourceToEditableArea.end();)
	{
		// filtering at first, actual stop then
		std::shared_ptr<IEditableArea> editableArea = it->second;
		it = touchSourceToEditableArea.erase(it); // increments to next valid
		AreaEventType eventType = editableArea->EndPointMove();
		// EVENT TO SEND ------------------------------------------------------------
        
        // FAIRE UN PUTAIN DE VECTEUR QUI SERA COPIÉ SAUVAGEMENT PAR CONSTRUCTEUR DE
        // COPIE DE CHAQUE ÉLÉMENT
        //
        // ÇA FAIT DU CALCUL MAIS ON S'EN BAT LES COUILLES, C'EST UN ÉVÈNEMENT PAS
        // TROP FRÉQUENT
	}
}



// - - - - - Canvas (mouse) events managing - - - - -

std::shared_ptr<GraphicEvent> InteractiveScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
	// default : empty AREA event (TO DO : events may happen on exciters, etc, etc, ...)
	std::shared_ptr<GraphicEvent> graphicE(new AreaEvent(nullptr, AreaEventType::NothingHappened));

    switch(excitersBehavior)
    {
        case ExcitersBehaviorType::AppearOnTouch :
            throw std::runtime_error("Exciter appearing on touch not implemented");
            break;
            
        case ExcitersBehaviorType::ManualAddAndDelete :
        case ExcitersBehaviorType::ManualAddAndDelete_AutoDisappear :
        {
            // Security : if the touch source ID already exists... What do we do ?
            
            // In this case, exciters already exist. We try to move one that is not
            // already linked to a touch source

			// While no exciter is selected : we look for a new one to select,
			// starting from the area on the upper layer (last draw on canvas)
            auto eventType = AreaEventType::NothingHappened;
            for (int i=(int)currentExciters.size()-1; (i>=0)&&(eventType != AreaEventType::PointDragBegins) ; i--)
            {
                // Function will check first if the area is not already dragged
                eventType = currentExciters[i]->TryBeginPointMove(mouseE.position.toDouble());
                if (eventType == AreaEventType::PointDragBegins)
                {
                    // Indicates the the move can begin
                    touchSourceToEditableArea[mouseE.source.getIndex()] = currentExciters[i];
					graphicE = std::shared_ptr<GraphicEvent>(new AreaEvent(currentExciters[i], eventType));
                }
            }
            break;
        }
            
        default :
            break;
    }
    
	return graphicE;
}
std::shared_ptr<GraphicEvent> InteractiveScene::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
    // Default empty event...
    std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent());
    
    int touchIndex = mouseE.source.getIndex();
    auto mapIt = touchSourceToEditableArea.find(touchIndex);
    // If the touch is related to an area being moved
    if (mapIt != touchSourceToEditableArea.end())
    {
        AreaEventType eventType = mapIt->second->TryMovePoint(mouseE.position.toDouble());
        graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(mapIt->second, eventType));
    }
    
    return graphicE;
}
std::shared_ptr<GraphicEvent> InteractiveScene::OnCanvasMouseUp(const MouseEvent& mouseE)
{
	std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent());

    switch(excitersBehavior)
    {
        case ExcitersBehaviorType::AppearOnTouch :
            throw std::runtime_error("Exciter appearing on touch not implemented");
            break;
            
        case ExcitersBehaviorType::ManualAddAndDelete :
        case ExcitersBehaviorType::ManualAddAndDelete_AutoDisappear :
        // We end the move, if the touch was actually related to an area
        {
            int touchIndex = mouseE.source.getIndex();
            // May not always exist (suddens touch state reinit on scene change, ...)
            auto mapIt = touchSourceToEditableArea.find(touchIndex);
            if (mapIt != touchSourceToEditableArea.end())
            {
                mapIt->second->EndPointMove();
                touchSourceToEditableArea.erase(mapIt);
				// no event : exciter remains alive
				//graphicE = 
            }
            break;
        }
            
        default :
            break;
    }
    return graphicE;
}




