/*
  ==============================================================================

    SceneEditionManager.cpp
    Created: 28 Mar 2016 5:27:18pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SceneEditionManager.h"


// Other includes

#include "View.h"

using namespace Miam;




SceneEditionManager::SceneEditionManager(View* _view) :
    view(_view)
{
    setMode(SceneEditionMode::Loading);
    
    // Links to the view module
    sceneEditionComponent = view->GetMainContentComponent()->GetSceneEditionComponent();
    
    // Canvases const count defined within the View module...
    areasOrder.resize(SceneCanvasComponent::SceneCanvasesCount);
    
    // Links to the view module
    sceneEditionComponent->CompleteInitialization(this);
    
    // Finally, state of the presenter
    nextAreaId = 0; // plus tard : valeur contenue dans le fichier de sauvegarde
    setAreaToCopy(-1);
    setSelectedAreaUniqueId(-1); // also sets the program mode
    setSelectedCanvasId(SceneCanvasComponent::FixedScene);
}


// Testing purposes only
void SceneEditionManager::__AddTestPolygons()
{
    EditablePolygon currentEditablePolygon = EditablePolygon(0); // because no empty constructor...
    for (int i=0 ; i<4 ; i++)
    {
        currentEditablePolygon = EditablePolygon(nextAreaId,
                                                 Point<double>(0.2f+0.13f*i,0.3f+0.1f*i), 3+2*i, 0.15f+0.04f*(i+1),
                                                 Colour(80*(uint8)i, 0, 255),
                                                 sceneEditionComponent->GetSceneCanvasComponent(selectedCanvasId)->GetRatio());
        addEditablePolygon(currentEditablePolygon);
    }
}




// ===== SETTERS AND GETTERS =====

EditablePolygon& SceneEditionManager::GetEditablePolygon(int64_t uniqueId)
{
    // If the area is a polygon
    std::map< int64_t, int >::iterator mapIt = polygonsVectorIndexes.find(uniqueId);
    if (mapIt != polygonsVectorIndexes.end())
        return polygons[mapIt->second];
    // else : nothing found...
    else
        throw std::range_error("Unique ID doesn't refer to any existing editable polygon.");
}
size_t SceneEditionManager::GetDrawableAreasCount(SceneCanvasComponent::Id _id)
{
    if (_id >= 0)
        return areasOrder[_id].size();
    else
        return 0;
}
DrawableArea& SceneEditionManager::GetDrawableArea(SceneCanvasComponent::Id _id, size_t position)
{
    return GetEditablePolygon(areasOrder[_id][position]); // may throw an exception !
}






// ===== UTILITIES =====

void SceneEditionManager::addEditablePolygon(EditablePolygon &newPolygon, bool selectArea)
{
    // Internal area objects modification
    polygons.push_back(newPolygon);
    polygonsVectorIndexes.insert(std::pair<int64_t, int>(nextAreaId,(int)polygons.size()-1));
    
    
    // Canvas drawing info
    areaCanvasId.insert(std::pair<int64_t,int>(nextAreaId,selectedCanvasId));
    areasOrder[selectedCanvasId].push_back(nextAreaId);// Last added drawn on top
    
    if (selectArea)
        setSelectedAreaUniqueId(nextAreaId);
    
    // Forced graphical updates
    polygons.back().CanvasResized(
                        sceneEditionComponent->GetSceneCanvasComponent(areaCanvasId[nextAreaId]));
    sceneEditionComponent->repaint();
    
    // We really go now for the next iteration
    nextAreaId++;
}
void SceneEditionManager::deleteEditableArea(int64_t uniqueId)
{
    bool somethingDeleted = false;
    // If the area is a polygon
    std::map< int64_t, int >::iterator mapIt = polygonsVectorIndexes.find(uniqueId);
    if (mapIt != polygonsVectorIndexes.end())
    {
        int polygonId = mapIt->second;
        // References to the polygon to delete
        polygonsVectorIndexes.erase(mapIt);
        // References to the polygon to update
        for (mapIt = polygonsVectorIndexes.begin() ;
             mapIt != polygonsVectorIndexes.end() ; mapIt++)
        {
            if (mapIt->second > polygonId)
                (mapIt->second)--;
        }
        // Actual deletion of the polygon
        polygons.erase(polygons.begin() + polygonId);
        somethingDeleted = true;
    }
    
    // Else if the area is something else... we'll do something else !
    
    
    // Common deletion code for all kinds of areas
    if (somethingDeleted)
    {
        // Update of areas order
        for (int i=0 ; i<areasOrder[selectedCanvasId].size() ; i++ )
        {
            if (areasOrder[selectedCanvasId][i] == uniqueId)
                areasOrder[selectedCanvasId].erase(areasOrder[selectedCanvasId].begin() + i);
        }
        // Graphic updates
        setSelectedAreaUniqueId(-1);
        sceneEditionComponent->repaint();
    }
    // If nothing was deleted, there has been a problem...
    else // we throw a std::runtime_error exception
        throw std::range_error("Unique ID doesn't refer to any existing editable area.");
}

void SceneEditionManager::setSelectedCanvasId(SceneCanvasComponent::Id _id)
{
    // Default : unselection of all areas...
    setSelectedAreaUniqueId(-1);
    
    // Graphical updates
    for (int i=0 ; i<SceneCanvasComponent::SceneCanvasesCount ; i++)
    {
        if (i == _id)
            sceneEditionComponent->GetSceneCanvasComponent(i)->SetIsSelectedForEditing(true);
        else
            sceneEditionComponent->GetSceneCanvasComponent(i)->SetIsSelectedForEditing(false);
    }
    
    // Internal state actualisation
    selectedCanvasId = _id;
    setMode(SceneEditionMode::CanvasSelected);
}

void SceneEditionManager::setSelectedAreaUniqueId(int64_t newIndex)
{
    // TODO
    // make this function more generic (able to select any kind of area)
    
    
    // -1 received <=> deselection command
    if (newIndex == -1)
    {
        // If one was selected
        if (selectedAreaUniqueId >= 0)
        {
            // The selected may be the one that has just been deleted
            try {
                GetEditablePolygon(selectedAreaUniqueId).SetActive(false);
            }
            catch (const std::range_error& /*e*/) {}
        }
        
        selectedAreaUniqueId = -1;
        
        if (mode == SceneEditionMode::CanvasSelected
            || mode == SceneEditionMode::PolygonSelected)
            setMode(SceneEditionMode::CanvasSelected);
        else
            setMode(SceneEditionMode::NothingSelected);
    }
    
    
    // Else : we select a new polygon
    else if (newIndex >= 0)
    {
        // If one was selected
        if (selectedAreaUniqueId >= 0)
            GetEditablePolygon(selectedAreaUniqueId).SetActive(false);
        
        selectedAreaUniqueId = newIndex;
        GetEditablePolygon(selectedAreaUniqueId).SetActive(true);
        setMode(SceneEditionMode::PolygonSelected); // TO CHANGE, NOT GENERIC
    }
}

void SceneEditionManager::setAreaToCopy(int64_t newUniqueId)
{
    areaToCopy = newUniqueId;
    
    sceneEditionComponent->SetPasteEnabled(areaToCopy >= 0);
}


/* Regroups all necessary actions on a mode change,
 * moreover : does not always necessarily allow the mode change requested !
 */
void SceneEditionManager::setMode(SceneEditionMode newMode)
{
    switch(newMode)
    {
        case SceneEditionMode::Null :
            std::cout << "Presenter:: (SceneEditionManager) : mode d'édition \"Null\" non-implémenté" << std::endl;
            break;
            
        case SceneEditionMode::Loading :
            std::cout << "Presenter:: (SceneEditionManager) : mode d'édition \"Loading\" non-implémenté" << std::endl;
            break;
            
        case SceneEditionMode::NothingSelected :
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupReduced(true);
            sceneEditionComponent->SetSpatGroupReduced(true);
            sceneEditionComponent->resized(); // right menu update
            break;
            
        case  SceneEditionMode::CanvasSelected :
            sceneEditionComponent->SetCanvasGroupHidden(false);
            sceneEditionComponent->SetAreaGroupReduced(true);
            sceneEditionComponent->SetSpatGroupReduced(true);
            sceneEditionComponent->resized(); // right menu update
            break;
            
        case SceneEditionMode::PolygonSelected :
            sceneEditionComponent->SetEnabledAllControls(true, true); // as we may come from "waiting for something creation/deletion"
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupReduced(false);
            sceneEditionComponent->SetSpatGroupReduced(false);
            sceneEditionComponent->SetAreaColourValue(GetEditablePolygon(selectedAreaUniqueId).GetFillColour());
            sceneEditionComponent->resized(); // right menu update
            break;
            
        case SceneEditionMode::EditingArea :
            break;
            
        case SceneEditionMode::WaitingForPointCreation :
            sceneEditionComponent->DisableAllButtonsBut("Add Point text button");
            break;
            
        case SceneEditionMode::WaitingForPointDeletion :
            sceneEditionComponent->DisableAllButtonsBut("Delete Point text button");
            break;
            
        default :
            break;
    }
    
    
    // Internal order : we finally don't even discuss it
    mode = newMode;
}



// ===== EVENTS from SCENE CANVAS =====

void SceneEditionManager::OnCanvasMouseDown(SceneCanvasComponent::Id canvasId, Point<int> clicLocation)
{
    /* Did we click inside the previous canvas ?
     * If Yes, we just continue...
     * If No, we change the canvas... And all the rest will be the same (polygon will be
     * automatically unselected)
     */
    if (canvasId != selectedCanvasId)
        setSelectedCanvasId(canvasId);
    
    
    // When an area is already selected
    if (mode == SceneEditionMode::PolygonSelected)
    {
        // did we clic next to a point, or at least inside the area ?
        if(! GetEditablePolygon(selectedAreaUniqueId).TryBeginPointMove(clicLocation.toDouble()))
        {
            /* if not, we are sure that we clicked outside (checked by tryBeginPointMove)
             * => it is a DEselection (maybe selection of another just after this)
             */
            setSelectedAreaUniqueId(-1);
            // We clicked inside a canvas :
        }
    }
    // While no area is selected : we look for a new one to select,
    // starting from the area on the upper layer (last draw on canvas)
    for (int i=(int)areasOrder[selectedCanvasId].size()-1 ; (i>=0 && mode==SceneEditionMode::CanvasSelected) ; i--)
    {
        if (GetEditablePolygon(areasOrder[selectedCanvasId][i]).HitTest(clicLocation.toDouble()))
        {
            //std::cout << "poids d'interaction = " << GetEditablePolygon(areasOrder[i]).ComputeInteractionWeight(clicLocation.toDouble()) << std::endl;
            setSelectedAreaUniqueId(areasOrder[selectedCanvasId][i]);
        }
    }
    // New point creation
    if (mode == SceneEditionMode::WaitingForPointCreation)
    {
        if(! GetEditablePolygon(selectedAreaUniqueId).TryCreatePoint(clicLocation.toDouble()))
            view->DisplayInfo("No point created : please click closer to an existing point !");
            
        // No matter wether the point was created or not
        setMode(SceneEditionMode::PolygonSelected);
    }
    // Existing point deletion
    else if (mode == SceneEditionMode::WaitingForPointDeletion)
    {
        String returnMsg
        = GetEditablePolygon(selectedAreaUniqueId).TryDeletePoint(clicLocation.toDouble());
        if(returnMsg != "")
            view->DisplayInfo(returnMsg);
        
        setMode(SceneEditionMode::PolygonSelected);
    }
    
    // in any case (does not waste much computing time...)
    sceneEditionComponent->repaint();
}
void SceneEditionManager::OnCanvasMouseDrag(SceneCanvasComponent::Id /*canvasId*/, Point<int> mouseLocation)
{
    if (selectedAreaUniqueId>=0)
        GetEditablePolygon(selectedAreaUniqueId).TryMovePoint(mouseLocation.toDouble());
    
    
    sceneEditionComponent->repaint();
}
void SceneEditionManager::OnCanvasMouseUp(SceneCanvasComponent::Id /*canvasId*/)
{
    if (selectedAreaUniqueId>=0)
        GetEditablePolygon(selectedAreaUniqueId).EndPointMove();
    
    sceneEditionComponent->repaint();
}

void SceneEditionManager::OnAddPoint()
{
    // Only if the button is not clicked a second time in a row
    if (mode != SceneEditionMode::WaitingForPointCreation)
        setMode(SceneEditionMode::WaitingForPointCreation);
    // Else, we cancel the point creation
    else
        setMode(SceneEditionMode::PolygonSelected);
}
void SceneEditionManager::OnDeletePoint()
{
    // Only if the button is not clicked a second time in a row
    if (mode != SceneEditionMode::WaitingForPointDeletion)
        setMode(SceneEditionMode::WaitingForPointDeletion);
    // Else, we cancel the point deletion
    else
        setMode(SceneEditionMode::PolygonSelected);
}
void SceneEditionManager::OnCopyArea()
{
    setAreaToCopy(selectedAreaUniqueId);
}
void SceneEditionManager::OnPasteArea()
{
    EditablePolygon newPolygon = GetEditablePolygon(areaToCopy); // exceptions shoudn't happen...
    newPolygon.SetId(nextAreaId);
    // !!!!!  BESOIN DE SAVOIR SI ON CHANGE DE CANVAS OU NON !!!!!
    // Si on change, besoin d'appeler une fonction du genre :
    // RescaleForCanvas(SceneCanvasComponent* )
     //
    newPolygon.Translate(Point<double>(20,20));
    addEditablePolygon(newPolygon, true);
}
void SceneEditionManager::OnAddArea()
{
    // centered grey Hexagon !...
    EditablePolygon newPolygon = EditablePolygon(nextAreaId,
        Point<double>(0.5f,0.5f), 6, 0.15f,
        Colours::grey,
        sceneEditionComponent->GetSceneCanvasComponent(selectedCanvasId)->GetRatio());
    
    // Actual adding of this new polygon
    addEditablePolygon(newPolygon, true);
}
void SceneEditionManager::OnDeleteArea()
{
    deleteEditableArea(selectedAreaUniqueId);
}

void SceneEditionManager::OnNewColour(Colour colour)
{
    GetEditablePolygon(selectedAreaUniqueId).SetFillColour(colour);
    sceneEditionComponent->GetSceneCanvasComponent(areaCanvasId[selectedAreaUniqueId])->repaint();
}

void SceneEditionManager::OnSendToBack()
{
    if (areasOrder[selectedCanvasId].size()>1) // We do something only if more than one area
    {
        if (areasOrder[selectedCanvasId][0] != selectedAreaUniqueId) // and if it's not the last yet
        {
            // selectedArea becomes the first within the areasOrder vector.
            // at first, we need to find where the area was before
            int selectedAreaOrderId = -1;
            for (int i=0 ; i<areasOrder[selectedCanvasId].size() ; i++)
            {
                if (areasOrder[selectedCanvasId][i] == selectedAreaUniqueId)
                    selectedAreaOrderId = i;
            }
            // then we udpate all what's necessary and we change the value
            for (int i=selectedAreaOrderId ; i >= 1 ; i--)
                areasOrder[selectedCanvasId][i] = areasOrder[selectedCanvasId][i-1];
            areasOrder[selectedCanvasId][0] = selectedAreaUniqueId;
        }
    }
    sceneEditionComponent->
        GetSceneCanvasComponent(areaCanvasId[selectedAreaUniqueId])->repaint();
}
void SceneEditionManager::OnSendBackward()
{
    if (areasOrder[selectedCanvasId].size()>1) // We do something only if more than one area
    {
        if (areasOrder[selectedCanvasId][0] != selectedAreaUniqueId) // and if it's not the last yet
        {
            // selectedArea becomes the last within the areasOrder vector.
            // at first, we need to find where the area was before
            int selectedAreaOrderId = -1;
            for (int i=0 ; i<areasOrder[selectedCanvasId].size() ; i++)
            {
                if (areasOrder[selectedCanvasId][i] == selectedAreaUniqueId)
                    selectedAreaOrderId = i;
            }
            // then we udpate all what's necessary and we change the value
            areasOrder[selectedCanvasId][selectedAreaOrderId] = areasOrder[selectedCanvasId][selectedAreaOrderId-1];
            areasOrder[selectedCanvasId][selectedAreaOrderId-1] = selectedAreaUniqueId;
        }
    }
    sceneEditionComponent->
        GetSceneCanvasComponent(areaCanvasId[selectedAreaUniqueId])->repaint();
}
void SceneEditionManager::OnBringToFront()
{
    if (areasOrder[selectedCanvasId].size()>1) // We do something only if more than one area
    {
        if (areasOrder[selectedCanvasId].back() != selectedAreaUniqueId) // and if it's not the first yet
        {
            // selectedArea becomes the last within the areasOrder vector.
            // at first, we need to find where the area was before
            int selectedAreaOrderId = -1;
            for (int i=0 ; i<areasOrder[selectedCanvasId].size() ; i++)
            {
                if (areasOrder[selectedCanvasId][i] == selectedAreaUniqueId)
                    selectedAreaOrderId = i;
            }
            // then we udpate all what's necessary and we change the value
            for (int i=selectedAreaOrderId ; i <= areasOrder[selectedCanvasId].size()-2 ; i++)
                areasOrder[selectedCanvasId][i] = areasOrder[selectedCanvasId][i+1];
            areasOrder[selectedCanvasId][areasOrder[selectedCanvasId].size()-1] = selectedAreaUniqueId;
        }
    }
    sceneEditionComponent->
        GetSceneCanvasComponent(areaCanvasId[selectedAreaUniqueId])->repaint();
}
void SceneEditionManager::OnBringForward()
{
    if (areasOrder[selectedCanvasId].size()>1) // We do something only if more than one area
    {
        if (areasOrder[selectedCanvasId].back() != selectedAreaUniqueId) // and if it's not the first yet
        {
            // selectedArea becomes the last within the areasOrder vector.
            // at first, we need to find where the area was before
            int selectedAreaOrderId = -1;
            for (int i=0 ; i<areasOrder[selectedCanvasId].size() ; i++)
            {
                if (areasOrder[selectedCanvasId][i] == selectedAreaUniqueId)
                    selectedAreaOrderId = i;
            }
            // then we udpate all what's necessary and we change the value
            areasOrder[selectedCanvasId][selectedAreaOrderId] = areasOrder[selectedCanvasId][selectedAreaOrderId+1];
            areasOrder[selectedCanvasId][selectedAreaOrderId+1] = selectedAreaUniqueId;
        }
    }
    sceneEditionComponent->
        GetSceneCanvasComponent(areaCanvasId[selectedAreaUniqueId])->repaint();
}


