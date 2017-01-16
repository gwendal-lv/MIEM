 /*
  ==============================================================================

    SceneCanvas.cpp
    Created: 8 Oct 2016 1:02:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#include "MultiSceneCanvasComponent.h"
#include "MultiSceneCanvasInteractor.h"


#include "IGraphicSessionManager.h"

#include "EditableScene.h"

#include "SceneEvent.h"


using namespace Miam;



MultiSceneCanvasInteractor::MultiSceneCanvasInteractor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId)
{
    mode = CanvasManagerMode::Loading;
    
    graphicSessionManager = _graphicSessionManager;
    selfId = _selfId;
    
    // CANVAS DOIT DEJA ETRE CRÉÉ BORDEL DE MERDE
    // LE TOUT EST GÉRÉ PAR LE PARENT
    canvasComponent = _canvasComponent;
    canvasComponent->LinkToManager(this);
    canvasComponent->GetCanvas()->CompleteInitialization(this); // auto-reference to the managed obj.

    
    
    // DO NOT auto-call this here : it calls back the edition manager, which calls this canvas, which is not constructed yet...
    // SetMode(CanvasManagerMode::Unselected);
    mode = CanvasManagerMode::Unselected;
}


MultiSceneCanvasInteractor::~MultiSceneCanvasInteractor()
{
}


void MultiSceneCanvasInteractor::CallRepaint()
{
    canvasComponent->repaint();
}




// - - - - - - - - - - running Mode - - - - - - - - - -

// The cases below are to be FORCED by the IGraphicSessionManager !
// Or by the canvasinteractor itself
void MultiSceneCanvasInteractor::SetMode(Miam::CanvasManagerMode newMode)
{
    // We don't do a specific action on every mode change !
    // But a few require checks and action
    switch (newMode) {
        
        case CanvasManagerMode::Unselected:
            // Unselection of a selected area (1 area selected for all canvases...)
            if (selectedScene) // on first scene adding... there would be a problem
                GetSelectedScene()->SetSelectedArea(nullptr, false);
            // Graphical updates
            if (mode != CanvasManagerMode::Unselected)
            {
                canvasComponent->SetIsSelectedForEditing(false);
            }
            break;
            
        case CanvasManagerMode::SceneOnlySelected:
            
            // à quoi servent VRAIMENT les lignes là-dessous ?
            /*if (selectedScene) // on first scene adding... there would be a problem
             GetSelectedScene()->SetSelectedArea(nullptr, false);*/
            // Graphical updates
            if (mode == CanvasManagerMode::Unselected)
            {
                canvasComponent->SetIsSelectedForEditing(true);
            }
            break;
            
            
        // Default case : we just apply the new mode
        default:
            break;
    }
    
    mode = newMode;
    
    
    graphicSessionManager->CanvasModeChanged(mode);
}






// - - - - - - - - - - Getters and Setters - - - - - - - - - -

std::shared_ptr<IDrawableArea> MultiSceneCanvasInteractor::GetDrawableObject(size_t index_)
{
    return selectedScene->GetDrawableObject(index_);
}

size_t MultiSceneCanvasInteractor::GetDrawableObjectsCount()
{
    return selectedScene->GetDrawableObjectsCount();
}

std::vector< std::shared_ptr<InteractiveScene> > MultiSceneCanvasInteractor::GetInteractiveScenes()
{
    std::vector< std::shared_ptr<InteractiveScene>> interactiveScenes;
    for (size_t i=0 ; i<scenes.size();i++)
        interactiveScenes.push_back(scenes[i]);
    return interactiveScenes;
}

int MultiSceneCanvasInteractor::GetSelectedSceneId()
{
    // Return the id if found
    for (size_t i=0 ; i<scenes.size() ; i++)
    {
        if (scenes[i] == selectedScene)
            return (int)i;
    }
    // Or return -1 if not found
    return -1;
}



// - - - - - Selection management (areas and scenes) - - - - -


void MultiSceneCanvasInteractor::SelectScene(int id)
{
    if ( 0 <= id && id < (int)(scenes.size()) )
    {
        // Unselection of any area first
        //SetMode(CanvasManagerMode::NothingSelected); // Something strange here..........
        // and Deactivation of the scene
        if (selectedScene)
            selectedScene->OnUnselection();
        
        // Then : we become the new selected canvas (if we were not before)
        graphicSessionManager->SetSelectedCanvas(this);
        // No specific other check, we just create the informative event before changing
        std::shared_ptr<SceneEvent> graphicE(new SceneEvent(this, selectedScene, scenes[id]));
        selectedScene = std::dynamic_pointer_cast<EditableScene>(scenes[id]);
        selectedScene->OnSelection();
        
        SetMode(CanvasManagerMode::SceneOnlySelected);
        
        // Graphic updates
        canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
        CallRepaint();
        
        // Finally : info sent to the graphic session manager (may not be actually used)
        graphicSessionManager->HandleEventSync(graphicE);
    }
    else
    {
        std::string errorMsg = "Scene id " + std::to_string(id) + " does not exist inside multi-scene canvas " + std::to_string(selfId);
        throw std::runtime_error(errorMsg);
    }
}


// - - - - - - Scenes managing : Add and Delete - - - - - -



void MultiSceneCanvasInteractor::AddScene(std::string name)
{
    std::shared_ptr<EditableScene> newScene(new EditableScene(this, canvasComponent->GetCanvas()));
    newScene->SetName(name);
    AddScene(newScene);
}
void MultiSceneCanvasInteractor::AddScene(std::shared_ptr<EditableScene> newScene)
{
    scenes.push_back( newScene );
    SelectScene((int)(scenes.size())-1);
    // Graphical updates
    canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
}
bool MultiSceneCanvasInteractor::DeleteScene()
{
    if (scenes.size()<=1)
        return false;
    else
    {
        // At first, we select the previous scene (or the next if it was the first...)
        int selectedSceneId = GetSelectedSceneId();
        if (selectedSceneId == 0)
            SelectScene(1);
        else
            SelectScene(selectedSceneId-1);

        // Then we remove this one
        auto it = scenes.begin() + selectedSceneId;
        scenes.erase(it);
        // Graphical updates
        canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
        canvasComponent->repaint();
        
        // Finally we re-select the same scene to force some updates...
        // Optimisation could be made here
        SelectScene(GetSelectedSceneId());
        
        return true;
    }
}




// - - - - - - - - - - Areas Managing : Add and Delete - - - - - - - - - -

void MultiSceneCanvasInteractor::__AddTestAreas()
{
    for (size_t i=0 ; i < scenes.size() ; i++)
    {
        int areasCount = 2+(rand()%3);
        
        auto editableScene = std::dynamic_pointer_cast<EditableScene>(scenes[i]);
        for (int j=0 ; j<areasCount ; j++)
        {
            // Only polygons added for now
            std::shared_ptr<EditablePolygon> currentEditablePolygon(new EditablePolygon(
                                                                                        graphicSessionManager->GetNextAreaId(),
                                                                                        Point<double>(0.2f+0.13f*j,0.3f+0.1f*j), 3+2*j, 0.15f+0.04f*(j+1),
                                                                                        Colour(80*(uint8)j, 0, 255),
                                                                                        canvasComponent->GetCanvas()->GetRatio()));
            editableScene->AddArea(currentEditablePolygon);
        }
    }
}











// - - - - - - - - - - Events from canvas - - - - - - - - - -

void MultiSceneCanvasInteractor::OnCanvasMouseDown(const MouseEvent& mouseE)
{
    // !!!!!!!!!!!!!! ON DOIT DIRE AU PARENT QU'ON SE SÉLECTIONNE SOI-MÊME !!!!!!!!!!!!!!
    graphicSessionManager->SetSelectedCanvas(this);
    
    std::shared_ptr<GraphicEvent> graphicE = selectedScene->OnCanvasMouseDown(mouseE);
    if (! graphicE->GetMessage().empty())
        graphicSessionManager->DisplayInfo(graphicE->GetMessage());
    
    // If we were in a "special waiting mode" we end it after this click
    if (mode == CanvasManagerMode::WaitingForPointCreation || mode == CanvasManagerMode::WaitingForPointDeletion)
    {
        // CHANGEMENT SELON L'ÉVÈNEMENT RETOURNÉ ???
        SetMode(CanvasManagerMode::AreaSelected); // ON GARDE L'AIRE SÉLECTIONNÉE POUR L'INSTANT
    }
    
    // in any case : repaint() (does not waste much computing time...)
    canvasComponent->repaint();
    
    // Event transmission towards the audio interpretation
    graphicSessionManager->HandleEventSync(graphicE);
}
void MultiSceneCanvasInteractor::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
    std::shared_ptr<GraphicEvent> graphicE = selectedScene->OnCanvasMouseDrag(mouseE);
    // If event was an AreaEvent
    if (std::shared_ptr<AreaEvent> areaE = std::dynamic_pointer_cast<AreaEvent>(graphicE))
    {
        if (areaE->GetType() != AreaEventType::NothingHappened)
            canvasComponent->repaint();
    }
    
    // Event transmission towards the audio interpretation
    graphicSessionManager->HandleEventSync(graphicE);
}
void MultiSceneCanvasInteractor::OnCanvasMouseUp(const MouseEvent& mouseE)
{
    std::shared_ptr<GraphicEvent> graphicE = selectedScene->OnCanvasMouseUp(mouseE);
    
    canvasComponent->repaint();
    
    // Event transmission towards the audio interpretation
    graphicSessionManager->HandleEventSync(graphicE);
}

