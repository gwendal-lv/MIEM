/*
  ==============================================================================

    EditableScene.cpp
    Created: 17 Nov 2016 4:54:53pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "EditableScene.h"

#include "EditablePolygon.h"

#include "MultiSceneCanvasInteractor.h"

#include "SceneCanvasComponent.h"



// - - - - - Construction and Destruction (and helpers) - - - - -

EditableScene::EditableScene(MultiSceneCanvasInteractor* _canvasManager, SceneCanvasComponent* _canvasComponent) :
    InteractiveScene(_canvasManager, _canvasComponent)
{
    
}




// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

std::shared_ptr<IEditableArea> EditableScene::GetEditableArea(size_t i)
{
    return std::dynamic_pointer_cast<EditableArea>(areas[i]);
}



void EditableScene::SetSelectedArea(std::shared_ptr<IEditableArea> _selectedArea, bool changeMode)
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
            canvasManager->SetMode(CanvasManagerMode::NothingSelected);
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
            canvasManager->SetMode(CanvasManagerMode::AreaSelected);
        
    }
}




// = = = = = = = = = = METHODS = = = = = = = = = =


// - - - - - Areas Managing : Add and Delete - - - - -



void EditableScene::AddDefaultArea(uint64_t _nextAreaId)
{
    // centered grey Hexagon !...
    std::shared_ptr<EditablePolygon> newPolygon(new EditablePolygon(_nextAreaId,
                                                                    Point<double>(0.5f,0.5f), 6, 0.15f,
                                                                    Colours::grey,
                                                                    canvasComponent->GetRatio() ));
    
    // Actual adding of this new polygon
    AddArea(newPolygon);
}

void EditableScene::DeleteSelectedArea()
{
    if (selectedArea)
    {
        deleteAreaByUniqueId(selectedArea->GetId());
        SetSelectedArea(nullptr); // actual deletion should happen here
    }
    else throw std::runtime_error("Impossible to delete the selected area (no area selected");
}

void EditableScene::deleteAreaByUniqueId(uint64_t uidToDelete)
{
    bool areaDeleted;
    for (auto it = areas.begin() ;
         (!areaDeleted && it!=areas.end()) ; it++)
    {
        if ((*it)->GetId() == uidToDelete)
        {
            it = areas.erase(it);
            areaDeleted = true;
        }
    }
}




// ------ areas managing : graphical attributes ------

void EditableScene::ChangeSelectedAreaColour(Colour& colour)
{
    if (selectedArea)
    {
        selectedArea->SetFillColour(colour);
        canvasComponent->repaint();
    }
    else
        std::runtime_error("The given colour cannot be applied : no area is selected");
}


// - - - - - -  areas managing : Z-Order - - - - - -

void EditableScene::SendSelectedAreaToBack()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            int selectedAreaOrder = -1; // the current drawing order (not known yet)
            
            // Do something if it's not the last yet (last to be drawn)
            if (areas[0] != selectedArea)             {
                // selectedArea becomes the first within the areasOrder vector.
                // at first, we need to find where the area was before
                for (size_t i=0 ; i<areas.size() ; i++)
                {
                    if (areas[i] == selectedArea)
                        selectedAreaOrder = i;
                }
                // then we udpate all what's necessary and we change the value
                // the selected area will not die because it has always the "selectedArea" shared pointer poiting to it !...
                for (int i=selectedAreaOrder ; i >= 1 ; i--)
                    areas[i] = areas[i-1];
                areas[0] = selectedArea;
            }
        }
        canvasComponent->repaint();
    }
    else throw std::runtime_error("Cannot send and area to back : no area selected");
}

void EditableScene::SendSelectedAreaBackward()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            if (areas[0] != selectedArea) // and if it's not the last yet
            {
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (int i=0 ; i<areas.size() ; i++)
                {
                    if (areas[i] == selectedArea)
                        selectedAreaOrder = i;
                }
                // then we udpate all what's necessary and we change the value
                areas[selectedAreaOrder] = areas[selectedAreaOrder-1];
                areas[selectedAreaOrder-1] = selectedArea;
            }
        }
        canvasComponent->repaint();
    }
    else throw std::runtime_error("Cannot send and area forward : no area selected");
}

void EditableScene::BringSelectedAreaForward()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            if (areas.back() != selectedArea) // and if it's not the first yet
            {
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (int i=0 ; i<areas.size() ; i++)
                {
                    if (areas[i] == selectedArea)
                        selectedAreaOrder = i;
                }
                // then we udpate all what's necessary and we change the value
                areas[selectedAreaOrder] = areas[selectedAreaOrder+1];
                areas[selectedAreaOrder+1] = selectedArea;
            }
        }
        canvasComponent->repaint();
    }
    else throw std::runtime_error("Cannot send and area forward : no area selected");
}

void EditableScene::BringSelectedAreaToFront()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            if (areas.back() != selectedArea) // and if it's not the first yet
            {
                // selectedArea becomes the last within the areasOrder vector.
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (size_t i=0 ; i<areas.size() ; i++)
                {
                    if (areas[i] == selectedArea)
                        selectedAreaOrder = i;
                }
                // then we udpate all what's necessary and we change the value
                for (int i=selectedAreaOrder ; i <= areas.size()-2 ; i++)
                    areas[i] = areas[i+1];
                areas.back() = selectedArea;
            }
            canvasComponent->repaint();
        }
    }
    else throw std::runtime_error("Cannot send an area to front : no area selected");
}




// - - - - - Canvas (mouse) events managing - - - - -

std::string EditableScene::OnCanvasMouseDown(Point<int>& clicLocation)
{
    std::string returnMsg;
    
    // When an area is already selected
    if (canvasManager->GetMode() == CanvasManagerMode::AreaSelected)
    {
        // did we clic next to a point, or at least inside the area ?
        if(! selectedArea->TryBeginPointMove(clicLocation.toDouble()))
        {
            /* if not, we are sure that we clicked outside (checked by tryBeginPointMove)
             * => it is a DEselection (maybe selection of another just after this)
             */
            SetSelectedArea(nullptr);
        }
    }
    // While no area is selected : we look for a new one to select,
    // starting from the area on the upper layer (last draw on canvas)
    for (int i=areas.size()-1 ; // int and not the *unsigned* size_t
         (i>=0 && canvasManager->GetMode()==CanvasManagerMode::NothingSelected) ;
         i--)
    {
        if (areas[i]->HitTest(clicLocation.toDouble()))
        {
            // !!!!!!!!!!!!! TEST DES POIDS D'INTERACTION !!!!!!!!!!!
            //std::cout << "poids d'interaction = " << areasOrderedForDrawing[i]->ComputeInteractionWeight(clicLocation.toDouble()) << std::endl;
            
            // On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
            std::shared_ptr<EditableArea> areaToSelect = std::dynamic_pointer_cast<EditableArea>(areas[i]);
            if (areaToSelect)
                SetSelectedArea(areaToSelect);
            else
                throw std::runtime_error(std::string("Cannot cast the area that should be selected to a Miam::EditableArea"));
        }
    }
    // New point creation POLYGONE SEULEMENT, PAS LES AUTRES FORMES !!!!!
    // Le bouton doit d'ailleurs apparaître de manière dynamique....
    if (canvasManager->GetMode() == CanvasManagerMode::WaitingForPointCreation)
    {
        // On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
        std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
        if (selectedPolygon)
        {
            if(! selectedPolygon->TryCreatePoint(clicLocation.toDouble()))
                returnMsg = "No point created : please click closer to an existing point !";
        }
        else
            throw std::runtime_error(std::string("Cannot create a point to the selected area (must be a polygon !)"));
        
        // No matter wether the point was created or not, we quit the creation and return
        // to the normal selected mode
        canvasManager->SetMode(CanvasManagerMode::AreaSelected);
    }
    // Existing point deletion
    else if (canvasManager->GetMode() == CanvasManagerMode::WaitingForPointDeletion)
    {
        // On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
        std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
        if (selectedPolygon)
        {
            returnMsg = (selectedPolygon->TryDeletePoint(clicLocation.toDouble())).toStdString();
        }
        else
            throw std::runtime_error(std::string("Cannot create a point to the selected area (must be a polygon !)"));
        
        canvasManager->SetMode(CanvasManagerMode::AreaSelected);
    }
    
    return returnMsg;
}
bool EditableScene::OnCanvasMouseDrag(Point<int>& mouseLocation)
{
    if (selectedArea)
    {
        selectedArea->TryMovePoint(mouseLocation.toDouble());
        canvasComponent->repaint();
        return true;
    }
    else
        return false;
}
bool EditableScene::OnCanvasMouseUp()
{
    if (selectedArea)
    {
        selectedArea->EndPointMove();
        return true;
    }
    else
        return false;
}

