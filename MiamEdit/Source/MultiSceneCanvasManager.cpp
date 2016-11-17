/*
  ==============================================================================

    SceneCanvas.cpp
    Created: 8 Oct 2016 1:02:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MultiSceneCanvasManager.h"

#include "SceneEditionManager.h"

#include "View.h"

using namespace Miam;

MultiSceneCanvasManager::MultiSceneCanvasManager(View* _view, SceneEditionManager* _sceneEditionManager, SceneEditionComponent* _sceneEditionComponent, SceneCanvasComponent::Id _selfId)
{
    view = _view;
    
    sceneEditionManager = _sceneEditionManager;
    sceneEditionComponent = _sceneEditionComponent;
    selfId = _selfId;
    
    canvasComponent = sceneEditionComponent->AddCanvas();
    canvasComponent->GetCanvas()->CompleteInitialization(this); // auto-reference to the managed obj.
    
    // do not auto-call this here : it calls back the edition manager, which calls this canvas, which is not constructed yet...
    //SetMode(SceneCanvasMode::Unselected);
}


MultiSceneCanvasManager::~MultiSceneCanvasManager()
{
    
}




// - - - - - - - - - - running Mode - - - - - - - - - -

void MultiSceneCanvasManager::SetMode(Miam::SceneCanvasMode newMode)
{
    // We don't do a specific action on every mode change !
    // But a few require checks and action
    switch (newMode) {
            
        case SceneCanvasMode::Unselected:
            if (mode != SceneCanvasMode::Unselected)
            {
                canvasComponent->SetIsSelectedForEditing(false);
                setSelectedArea(nullptr, false);
            }
            break;
            
        case SceneCanvasMode::NothingSelected:
            if (mode == SceneCanvasMode::Unselected)
            {
                canvasComponent->SetIsSelectedForEditing(true);
            }
            break;
            
            
        // Default case : we just apply the new mode
        default:
            break;
    }
    
    mode = newMode;
    
    
    sceneEditionManager->CanvasModeChanged(mode);
}






// - - - - - - - - - - Getters and Setters - - - - - - - - - -

std::shared_ptr<DrawableArea> MultiSceneCanvasManager::GetDrawableArea(int _index)
{
    std::shared_ptr<DrawableArea> drawableArea = std::dynamic_pointer_cast<DrawableArea>(areasOrderedForDrawing[_index]);
    return drawableArea;
}

void MultiSceneCanvasManager::setSelectedArea(std::shared_ptr<EditableArea> _selectedArea, bool changeMode)
{
    // Null pointer <=> deselection command
    if (_selectedArea == nullptr)
    {
        // If one was selected before the deselection
        if (selectedArea)
        {
            selectedArea->SetActive(false);
        }
        
        // In any, we actually deselect
        selectedArea = nullptr;
        if (changeMode)
            SetMode(SceneCanvasMode::NothingSelected);
    }
    
    // Else : we select a new existing EditableArea
    else
    {
        // If one was selected, we deselect it first
        if (selectedArea)
            selectedArea->SetActive(false);
        
        selectedArea = _selectedArea;
        selectedArea->SetActive(true);
        if (changeMode)
            SetMode(SceneCanvasMode::AreaSelected);
    }
}


// - - - - - Orders from the parent manager - - - - -
/*
void MultiSceneCanvasManager::SetUnselected()
{
    canvasComponent->SetIsSelectedForEditing()
}

void MultiSceneCanvasManager::SetSelected()
{
    
}*/





// - - - - - - - - - - Areas Managing : Add and Delete - - - - - - - - - -

void MultiSceneCanvasManager::__AddTestAreas()
{
    int areasCount = 3+(rand()%3);
    for (int i=0 ; i<areasCount ; i++)
    {
        // Only polygons added for now
        std::shared_ptr<EditablePolygon> currentEditablePolygon(new EditablePolygon(
                                                                                    sceneEditionManager->GetNextAreaId(),
                                                                                    Point<double>(0.2f+0.13f*i,0.3f+0.1f*i), 3+2*i, 0.15f+0.04f*(i+1),
                                                                                    Colour(80*(uint8)i, 0, 255),
                                                                                    canvasComponent->GetCanvas()->GetRatio()));
        AddEditableArea(currentEditablePolygon);
    }
}

void MultiSceneCanvasManager::AddEditableArea(std::shared_ptr<EditableArea> newArea, bool selectArea)
{
    // Internal area objects modification
    areas.push_back(newArea);
    areasOrderedForDrawing.push_back(areas.back());
    
    if (selectArea)
        setSelectedArea(areas.back());
    
    
    // Forced graphical updates
    areas.back()->CanvasResized(this->canvasComponent->GetCanvas());
    sceneEditionComponent->repaint();
    
    // We really go now for the next iteration
    //nextAreaId++; // useless because a special unique ID was given to this canvas... ok?
}

bool MultiSceneCanvasManager::DeleteSelectedArea()
{
    if (selectedArea)
    {
        uint64_t uidToDelete = selectedArea->GetId();
        // We first unselect the area
        setSelectedArea(nullptr);
        // Then actually delete it
        deleteAreaByUniqueId(uidToDelete);
        return true;
    }
    else
        return false;
}

void MultiSceneCanvasManager::AddDefaultArea(uint64_t _nextAreaId)
{
    // centered grey Hexagon !...
    std::shared_ptr<EditablePolygon> newPolygon(new EditablePolygon(_nextAreaId,
        Point<double>(0.5f,0.5f), 6, 0.15f,
        Colours::grey,
        canvasComponent->GetCanvas()->GetRatio() ));
    
    // Actual adding of this new polygon
    AddEditableArea(newPolygon, true);
}

void MultiSceneCanvasManager::deleteAreaByUniqueId(uint64_t uidToDelete)
{
    bool areaOrderedDeleted = false, baseAreaDeleted = false;
    for (auto it = areasOrderedForDrawing.begin() ;
         (!areaOrderedDeleted && it!=areasOrderedForDrawing.end()) ; it++)
    {
        if ((*it)->GetId() == uidToDelete)
        {
            it = areasOrderedForDrawing.erase(it);
            areaOrderedDeleted = true;
        }
    }
    for (auto it = areas.begin() ;
         (!baseAreaDeleted && it!=areas.end()) ; it++)
    {
        if ((*it)->GetId() == uidToDelete)
        {
            it = areas.erase(it);
            baseAreaDeleted = true;
        }
    }
}





// - - - - - -  areas managing : Z-Order - - - - - -

void MultiSceneCanvasManager::SendSelectedAreaToBack()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            int selectedAreaOrder = -1; // the current drawing order (not known yet)
            
            // Do something if it's not the last yet (last to be drawn)
            if (areasOrderedForDrawing[0] != selectedArea)             {
                // selectedArea becomes the first within the areasOrder vector.
                // at first, we need to find where the area was before
                for (size_t i=0 ; i<areasOrderedForDrawing.size() ; i++)
                {
                    if (areasOrderedForDrawing[i] == selectedArea)
                        selectedAreaOrder = i;
                }
                // then we udpate all what's necessary and we change the value
                // the selected area will not die because it has always the "selectedArea" shared pointer poiting to it !...
                for (int i=selectedAreaOrder ; i >= 1 ; i--)
                    areasOrderedForDrawing[i] = areasOrderedForDrawing[i-1];
                areasOrderedForDrawing[0] = selectedArea;
            }
        }
        canvasComponent->repaint();
    }
    else throw std::runtime_error("Cannot send and area to back : no area selected");
}

void MultiSceneCanvasManager::SendSelectedAreaBackward()
{
    if (selectedArea)
    {
        if (areasOrderedForDrawing.size()>1) // We do something only if more than one area
        {
            if (areasOrderedForDrawing[0] != selectedArea) // and if it's not the last yet
            {
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (int i=0 ; i<areasOrderedForDrawing.size() ; i++)
                {
                    if (areasOrderedForDrawing[i] == selectedArea)
                        selectedAreaOrder = i;
                }
                // then we udpate all what's necessary and we change the value
                areasOrderedForDrawing[selectedAreaOrder] = areasOrderedForDrawing[selectedAreaOrder-1];
                areasOrderedForDrawing[selectedAreaOrder-1] = selectedArea;
            }
        }
        canvasComponent->repaint();
    }
    else throw std::runtime_error("Cannot send and area forward : no area selected");
}

void MultiSceneCanvasManager::SendSelectedAreaForward()
{
    if (selectedArea)
    {
        if (areasOrderedForDrawing.size()>1) // We do something only if more than one area
        {
            if (areasOrderedForDrawing.back() != selectedArea) // and if it's not the first yet
            {
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (int i=0 ; i<areasOrderedForDrawing.size() ; i++)
                {
                    if (areasOrderedForDrawing[i] == selectedArea)
                        selectedAreaOrder = i;
                }
                // then we udpate all what's necessary and we change the value
                areasOrderedForDrawing[selectedAreaOrder] = areasOrderedForDrawing[selectedAreaOrder+1];
                areasOrderedForDrawing[selectedAreaOrder+1] = selectedArea;
            }
        }
        canvasComponent->repaint();
    }
    else throw std::runtime_error("Cannot send and area forward : no area selected");
}

void MultiSceneCanvasManager::SendSelectedAreaToFront()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            if (areasOrderedForDrawing.back() != selectedArea) // and if it's not the first yet
            {
                // selectedArea becomes the last within the areasOrder vector.
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (size_t i=0 ; i<areasOrderedForDrawing.size() ; i++)
                {
                    if (areasOrderedForDrawing[i] == selectedArea)
                        selectedAreaOrder = i;
                }
                // then we udpate all what's necessary and we change the value
                for (int i=selectedAreaOrder ; i <= areasOrderedForDrawing.size()-2 ; i++)
                    areasOrderedForDrawing[i] = areasOrderedForDrawing[i+1];
                areasOrderedForDrawing.back() = selectedArea;
            }
            canvasComponent->repaint();
        }
    }
    else throw std::runtime_error("Cannot send an area to front : no area selected");
}





// - - - - - - - - - - Events from canvas - - - - - - - - - -

void MultiSceneCanvasManager::OnCanvasMouseDown(Point<int> clicLocation)
{
    // !!!!!!!!!!!!!! ON DOIT DIRE AU PARENT QU'ON SE SÉLECTIONNE SOI-MÊME !!!!!!!!!!!!!!
    // !!!!!!!!!!!!!! ON DOIT DIRE AU PARENT QU'ON SE SÉLECTIONNE SOI-MÊME !!!!!!!!!!!!!!
    // !!!!!!!!!!!!!! ON DOIT DIRE AU PARENT QU'ON SE SÉLECTIONNE SOI-MÊME !!!!!!!!!!!!!!
    sceneEditionManager->SetSelectedCanvas(this);
    
    // When an area is already selected
    if (mode == SceneCanvasMode::AreaSelected)
    {
        // did we clic next to a point, or at least inside the area ?
        if(! selectedArea->TryBeginPointMove(clicLocation.toDouble()))
        {
            /* if not, we are sure that we clicked outside (checked by tryBeginPointMove)
             * => it is a DEselection (maybe selection of another just after this)
             */
            setSelectedArea(nullptr);
        }
    }
    // While no area is selected : we look for a new one to select,
    // starting from the area on the upper layer (last draw on canvas)
    for (int i=areasOrderedForDrawing.size()-1 ; // int and not the *unsigned* size_t
         (i>=0 && mode==SceneCanvasMode::NothingSelected) ; i--)
    {
        if (areasOrderedForDrawing[i]->HitTest(clicLocation.toDouble()))
        {
            // !!!!!!!!!!!!! TEST DES POIDS D'INTERACTION !!!!!!!!!!!
            //std::cout << "poids d'interaction = " << areasOrderedForDrawing[i]->ComputeInteractionWeight(clicLocation.toDouble()) << std::endl;
            setSelectedArea(areasOrderedForDrawing[i]);
        }
    }
    // New point creation POLYGONE SEULEMENT, PAS LES AUTRES FORMES !!!!!
    // Le bouton doit d'ailleurs apparaître de manière dynamique....
    if (mode == SceneCanvasMode::WaitingForPointCreation)
    {
        // On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
        std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
        if (selectedPolygon)
        {
            if(! selectedPolygon->TryCreatePoint(clicLocation.toDouble()))
                view->DisplayInfo("No point created : please click closer to an existing point !");
        }
        else
            throw std::runtime_error(std::string("Cannot create a point to the selected area (must be a polygon !)"));
        
        // No matter wether the point was created or not, we quit the creation and return
        // to the normal selected mode
        SetMode(SceneCanvasMode::AreaSelected);
    }
    // Existing point deletion
    else if (mode == SceneCanvasMode::WaitingForPointDeletion)
    {
        // On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
        std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
        if (selectedPolygon)
        {
            String returnMsg
            = selectedPolygon->TryDeletePoint(clicLocation.toDouble());
            if(returnMsg != "")
                view->DisplayInfo(returnMsg);
        }
        else
            throw std::runtime_error(std::string("Cannot create a point to the selected area (must be a polygon !)"));
        
        SetMode(SceneCanvasMode::AreaSelected);
    }
    
    // in any case (does not waste much computing time...)
    sceneEditionComponent->repaint();
}
void MultiSceneCanvasManager::OnCanvasMouseDrag(Point<int> mouseLocation)
{
    if (selectedArea)
    {
        selectedArea->TryMovePoint(mouseLocation.toDouble());
    }
    
    sceneEditionComponent->repaint();
}
void MultiSceneCanvasManager::OnCanvasMouseUp()
{
    if (selectedArea)
        selectedArea->EndPointMove();
    
    sceneEditionComponent->repaint();
}

