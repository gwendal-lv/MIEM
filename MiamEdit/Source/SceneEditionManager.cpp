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
    sceneEditionComponent->CompleteInitialization(this);
    
    
    // Création des 4 polygones de test !
    // comme on crée une nouvelle scène : on repart de zéro
    nextAreaId = 0; // plus tard : valeur contenue dans le fichier de sauvegarde
    EditablePolygon currentEditablePolygon = EditablePolygon(0);
    for (int i=0 ; i<4 ; i++)
    {
        currentEditablePolygon = EditablePolygon(nextAreaId,
			Point<double>(0.2f+0.13f*i,0.3f+0.1f*i), 3+2*i, 0.15f+0.04f*(i+1),
			Colour(80*(uint8)i, 0, 255),
			sceneEditionComponent->GetSceneCanvasComponent()->GetRatio());
        addEditablePolygon(currentEditablePolygon);
    }
    
    
    // Finally
    setSelectedAreaUniqueId(-1); // also sets the program mode
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
size_t SceneEditionManager::GetDrawableAreasSize()
{
    return areasOrder.size();
}
DrawableArea& SceneEditionManager::GetDrawableArea(int position)
{
    return GetEditablePolygon(areasOrder[position]); // may throw an exception !
}






// ===== UTILITIES =====

void SceneEditionManager::addEditablePolygon(EditablePolygon &newPolygon, bool selectArea)
{
    // Internal objects modification
    polygons.push_back(newPolygon);
    polygonsVectorIndexes.insert(std::pair<int64_t, int>(nextAreaId,(int)polygons.size()-1));
    
    // Last added drawn on top
    areasOrder.push_back(nextAreaId);
    
    if (selectArea)
        setSelectedAreaUniqueId(nextAreaId);
    
    // Forced graphical updates
    polygons.back().CanvasResized(sceneEditionComponent->GetSceneCanvasComponent()->getWidth(),
                                  sceneEditionComponent->GetSceneCanvasComponent()->getHeight());
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
        for (int i=0 ; i<areasOrder.size() ; i++ )
        {
            if (areasOrder[i] == uniqueId)
                areasOrder.erase(areasOrder.begin() + i);
        }
        // Graphic updates
        setSelectedAreaUniqueId(-1);
        sceneEditionComponent->repaint();
    }
    // If nothing was deleted, there has been a problem...
    else // we throw a std::runtime_error exception
        throw std::range_error("Unique ID doesn't refer to any existing editable area.");
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
        setMode(SceneEditionMode::NoAreaSelected);
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
            
        case SceneEditionMode::NoAreaSelected :
            sceneEditionComponent->disablePolygonEditionControls();
            break;
            
        case SceneEditionMode::PolygonSelected :
            sceneEditionComponent->enablePolygonEditionControls();
            sceneEditionComponent->SetAreaColourValue(GetEditablePolygon(selectedAreaUniqueId).GetFillColour());
            break;
            
        case SceneEditionMode::EditingArea :
            break;
            
        case SceneEditionMode::WaitingForPointCreation :
            sceneEditionComponent->disableAllButtonsBut("Add Point text button");
            break;
            
        case SceneEditionMode::WaitingForPointDeletion :
            sceneEditionComponent->disableAllButtonsBut("Delete Point text button");
            break;
            
        default :
            break;
    }
    
    // Internal order : we finally don't even discuss it
    mode = newMode;
}



// ===== EVENTS from SCENE CANVAS =====

void SceneEditionManager::OnCanvasMouseDown(Point<int> clicLocation)
{
    // When an area is selected
    if (mode == SceneEditionMode::PolygonSelected)
    {
        // did we clic next to a point, or at least inside the area ?
        if(! GetEditablePolygon(selectedAreaUniqueId).TryBeginPointMove(clicLocation.toDouble()))
        {
            /* if not, we are sure that we clicked outside (checked by tryBeginPointMove)
             * => it is a DEselection (maybe selection of another just after this)
             */
            setSelectedAreaUniqueId(-1);
        }
    }
    // While no area is selected : we look for a new one to select,
    // starting from the area on the upper layer (last draw on canvas)
    for (int i=(int)areasOrder.size()-1 ; (i>=0 && mode==SceneEditionMode::NoAreaSelected) ; i--)
    {
        if (GetEditablePolygon(areasOrder[i]).HitTest(clicLocation.toDouble()))
        {
            //std::cout << "poids d'interaction = " << GetEditablePolygon(areasOrder[i]).ComputeInteractionWeight(clicLocation.toDouble()) << std::endl;
            setSelectedAreaUniqueId(areasOrder[i]);
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
void SceneEditionManager::OnCanvasMouseDrag(Point<int> mouseLocation)
{
    if (selectedAreaUniqueId>=0)
        GetEditablePolygon(selectedAreaUniqueId).MovePoint(mouseLocation.toDouble());
    
    
    sceneEditionComponent->repaint();
}
void SceneEditionManager::OnCanvasMouseUp()
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
    areaToCopy = selectedAreaUniqueId;
}
void SceneEditionManager::OnPasteArea()
{
    if (areaToCopy >= 0)
    {
        // WARNING :  MUST CATCH EXCEPTION IF AREA WAS DELETED !!!!!!!!!!!!
        
        // HERE ALSO EXCEPTIONS MAYBE
        EditablePolygon newPolygon = GetEditablePolygon(areaToCopy); // may throw exception !!!!
        newPolygon.SetId(nextAreaId);
        newPolygon.Translate(Point<double>(30,30));
        addEditablePolygon(newPolygon, true);
    }
}
void SceneEditionManager::OnAddArea()
{
    EditablePolygon newPolygon = EditablePolygon(nextAreaId,
                                                 Point<double>(0.5f,0.5f), 4, 0.15f,
                                                 Colours::grey,
                                                 sceneEditionComponent->GetSceneCanvasComponent()->GetRatio());
    addEditablePolygon(newPolygon, true);
}
void SceneEditionManager::OnDeleteArea()
{
    deleteEditableArea(selectedAreaUniqueId);
}

void SceneEditionManager::OnNewColour(Colour colour)
{
    GetEditablePolygon(selectedAreaUniqueId).SetFillColour(colour);
    sceneEditionComponent->GetSceneCanvasComponent()->repaint();
}

void SceneEditionManager::OnSendToBack()
{
    if (areasOrder.size()>1) // We do something only if more than one area
    {
        if (areasOrder[0] != selectedAreaUniqueId) // and if it's not the last yet
        {
            // selectedArea becomes the first within the areasOrder vector.
            // at first, we need to find where the area was before
            int selectedAreaOrderId = -1;
            for (int i=0 ; i<areasOrder.size() ; i++)
            {
                if (areasOrder[i] == selectedAreaUniqueId)
                    selectedAreaOrderId = i;
            }
            // then we udpate all what's necessary and we change the value
            for (int i=selectedAreaOrderId ; i >= 1 ; i--)
                areasOrder[i] = areasOrder[i-1];
            areasOrder[0] = selectedAreaUniqueId;
        }
    }
    sceneEditionComponent->GetSceneCanvasComponent()->repaint();
}
void SceneEditionManager::OnSendBackward()
{
    if (areasOrder.size()>1) // We do something only if more than one area
    {
        if (areasOrder[0] != selectedAreaUniqueId) // and if it's not the last yet
        {
            // selectedArea becomes the last within the areasOrder vector.
            // at first, we need to find where the area was before
            int selectedAreaOrderId = -1;
            for (int i=0 ; i<areasOrder.size() ; i++)
            {
                if (areasOrder[i] == selectedAreaUniqueId)
                    selectedAreaOrderId = i;
            }
            // then we udpate all what's necessary and we change the value
            areasOrder[selectedAreaOrderId] = areasOrder[selectedAreaOrderId-1];
            areasOrder[selectedAreaOrderId-1] = selectedAreaUniqueId;
        }
    }
    sceneEditionComponent->GetSceneCanvasComponent()->repaint();
}
void SceneEditionManager::OnBringToFront()
{
    if (areasOrder.size()>1) // We do something only if more than one area
    {
        if (areasOrder.back() != selectedAreaUniqueId) // and if it's not the first yet
        {
            // selectedArea becomes the last within the areasOrder vector.
            // at first, we need to find where the area was before
            int selectedAreaOrderId = -1;
            for (int i=0 ; i<areasOrder.size() ; i++)
            {
                if (areasOrder[i] == selectedAreaUniqueId)
                    selectedAreaOrderId = i;
            }
            // then we udpate all what's necessary and we change the value
            for (int i=selectedAreaOrderId ; i <= areasOrder.size()-2 ; i++)
                areasOrder[i] = areasOrder[i+1];
            areasOrder[areasOrder.size()-1] = selectedAreaUniqueId;
        }
    }
    sceneEditionComponent->GetSceneCanvasComponent()->repaint();
}
void SceneEditionManager::OnBringForward()
{
    if (areasOrder.size()>1) // We do something only if more than one area
    {
        if (areasOrder.back() != selectedAreaUniqueId) // and if it's not the first yet
        {
            // selectedArea becomes the last within the areasOrder vector.
            // at first, we need to find where the area was before
            int selectedAreaOrderId = -1;
            for (int i=0 ; i<areasOrder.size() ; i++)
            {
                if (areasOrder[i] == selectedAreaUniqueId)
                    selectedAreaOrderId = i;
            }
            // then we udpate all what's necessary and we change the value
            areasOrder[selectedAreaOrderId] = areasOrder[selectedAreaOrderId+1];
            areasOrder[selectedAreaOrderId+1] = selectedAreaUniqueId;
        }
    }
    sceneEditionComponent->GetSceneCanvasComponent()->repaint();
}


