/*
  ==============================================================================

    SceneCanvas.cpp
    Created: 8 Oct 2016 1:02:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SceneCanvasManager.h"

#include "SceneEditionManager.h"

#include "View.h"

using namespace Miam;

SceneCanvasManager::SceneCanvasManager(View* _view, SceneEditionManager* _sceneEditionManager, SceneEditionComponent* _sceneEditionComponent, SceneCanvasComponent::Id _selfId)
{
    view = _view;
    
    sceneEditionManager = _sceneEditionManager;
    sceneEditionComponent = _sceneEditionComponent;
    selfId = _selfId;
    
    canvasComponent = sceneEditionComponent->AddCanvas(selfId);
    canvasComponent->CompleteInitialization(this); // auto-reference to the managed obj.
}






// - - - - - - - - - - running Mode - - - - - - - - - -

void SceneCanvasManager::SetMode(Miam::SceneCanvasMode newMode)
{
    // We don't do a specific action on every mode change !
    // But a few require checks and action
    switch (newMode) {
            
        case SceneCanvasMode::Unselected:
            if (mode != SceneCanvasMode::Unselected)
            {
                canvasComponent->SetIsSelectedForEditing(false);
                setSelectedArea(nullptr);
            }
            break;
            
        case SceneCanvasMode::NothingSelected:
            if (mode != SceneCanvasMode::NothingSelected)
                canvasComponent->SetIsSelectedForEditing(true);
            break;
            
        default:
            break;
    }
    
    // Actual mode change after checks ; then we alert our parent manager of what just happened
    mode = newMode;
    
    sceneEditionManager->CanvasModeChanged(mode);
}






// - - - - - - - - - - Getters and Setters - - - - - - - - - -

std::shared_ptr<DrawableArea> SceneCanvasManager::GetDrawableArea(int _index)
{
    std::shared_ptr<DrawableArea> drawableArea = std::dynamic_pointer_cast<DrawableArea>(areasOrderedForDrawing[_index]);
    return drawableArea;
}

void SceneCanvasManager::setSelectedArea(std::shared_ptr<EditableArea> _selectedArea)
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
        SetMode(SceneCanvasMode::AreaSelected);
    }
}


// - - - - - Orders from the parent manager - - - - -
/*
void SceneCanvasManager::SetUnselected()
{
    canvasComponent->SetIsSelectedForEditing(<#bool isSelected#>)
}

void SceneCanvasManager::SetSelected()
{
    
}*/





// - - - - - - - - - - Areas Managing : Add and Delete - - - - - - - - - -

void SceneCanvasManager::__AddTestAreas()
{
    int areasCount = 3+(rand()%3);
    for (int i=0 ; i<areasCount ; i++)
    {
        // Only polygons added for now
        std::shared_ptr<EditablePolygon> currentEditablePolygon(new EditablePolygon(
                                                                                    sceneEditionManager->GetNextAreaId(),
                                                                                    Point<double>(0.2f+0.13f*i,0.3f+0.1f*i), 3+2*i, 0.15f+0.04f*(i+1),
                                                                                    Colour(80*(uint8)i, 0, 255),
                                                                                    canvasComponent->GetRatio()));
        AddEditableArea(currentEditablePolygon);
    }
}

void SceneCanvasManager::AddEditableArea(std::shared_ptr<EditableArea> newArea, bool selectArea)
{
    // Internal area objects modification
    areas.push_back(newArea);
    areasOrderedForDrawing.push_back(areas.back());
    
    if (selectArea)
        setSelectedArea(areas.back());
    
    
    // Forced graphical updates
    areas.back()->CanvasResized(this->canvasComponent);
    sceneEditionComponent->repaint();
    
    // We really go now for the next iteration
    //nextAreaId++; // useless because a special unique ID was given to this canvas... ok?
}

bool SceneCanvasManager::DeleteSelectedArea()
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

void SceneCanvasManager::AddDefaultArea(uint64_t _nextAreaId)
{
    // centered grey Hexagon !...
    std::shared_ptr<EditablePolygon> newPolygon(new EditablePolygon(_nextAreaId,
        Point<double>(0.5f,0.5f), 6, 0.15f,
        Colours::grey,
        canvasComponent->GetRatio() ));
    
    // Actual adding of this new polygon
    AddEditableArea(newPolygon, true);
}

void SceneCanvasManager::deleteAreaByUniqueId(uint64_t uidToDelete)
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

void SceneCanvasManager::SendSelectedAreaToBack()
{
    if (selectedArea)
    {
        uint64_t selectedAreaUniqueId = selectedArea->GetId();
        if (areas.size()>1) // We do something only if more than one area
        {
            int selectedAreaOrder = -1; // the current drawing order (not known yet)
            
            // Do something if it's not the last yet (last to be drawn)
            if (areasOrderedForDrawing[0]->GetId() != selectedAreaUniqueId)             {
                // selectedArea becomes the first within the areasOrder vector.
                // at first, we need to find where the area was before
                for (size_t i=0 ; i<areasOrderedForDrawing.size() ; i++)
                {
                    if (areasOrderedForDrawing[i]->GetId() == selectedAreaUniqueId)
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

void SceneCanvasManager::SendSelectedAreaBackward()
{
    if (selectedArea)
    {
        if (areasOrderedForDrawing.size()>1) // We do something only if more than one area
        {
            uint64_t selectedAreaUniqueId = -1; // the current drawing order (not known yet)
            
            if (areasOrderedForDrawing[0]->GetId() != selectedAreaUniqueId) // and if it's not the last yet
            {
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (int i=0 ; i<areasOrderedForDrawing.size() ; i++)
                {
                    if (areasOrderedForDrawing[i]->GetId() == selectedAreaUniqueId)
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

void SceneCanvasManager::SendSelectedAreaForward()
{
    if (selectedArea)
    {
        if (areasOrderedForDrawing.size()>1) // We do something only if more than one area
        {
            uint64_t selectedAreaUniqueId = -1; // the current drawing order (not known yet)
            
            if (areasOrderedForDrawing.back()->GetId() != selectedAreaUniqueId) // and if it's not the first yet
            {
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (int i=0 ; i<areasOrderedForDrawing.size() ; i++)
                {
                    if (areasOrderedForDrawing[i]->GetId() == selectedAreaUniqueId)
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

void SceneCanvasManager::SendSelectedAreaToFront()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            uint64_t selectedAreaUniqueId = selectedArea->GetId();
            if (areasOrderedForDrawing.back()->GetId() != selectedAreaUniqueId) // and if it's not the first yet
            {
                // selectedArea becomes the last within the areasOrder vector.
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (size_t i=0 ; i<areasOrderedForDrawing.size() ; i++)
                {
                    if (areasOrderedForDrawing[i]->GetId() == selectedAreaUniqueId)
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

void SceneCanvasManager::OnCanvasMouseDown(Point<int> clicLocation)
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
            //std::cout << "poids d'interaction = " << GetEditablePolygon(areasOrder[i]).ComputeInteractionWeight(clicLocation.toDouble()) << std::endl;
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
void SceneCanvasManager::OnCanvasMouseDrag(Point<int> mouseLocation)
{
    if (selectedArea)
        selectedArea->TryMovePoint(mouseLocation.toDouble());
    
    sceneEditionComponent->repaint();
}
void SceneCanvasManager::OnCanvasMouseUp()
{
    if (selectedArea)
        selectedArea->EndPointMove();
    
    sceneEditionComponent->repaint();
}

