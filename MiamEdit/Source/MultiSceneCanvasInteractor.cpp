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



using namespace Miam;

MultiSceneCanvasInteractor::MultiSceneCanvasInteractor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId)
{
    graphicSessionManager = _graphicSessionManager;
    selfId = _selfId;
    
    // CANVAS DOIT DEJA ETRE CRÉÉ BORDEL DE MERDE
    // LE TOUT EST GÉRÉ PAR LE PARENT
    canvasComponent = _canvasComponent;
    canvasComponent->GetCanvas()->CompleteInitialization(this); // auto-reference to the managed obj.

    
    
    // DO NOT auto-call this here : it calls back the edition manager, which calls this canvas, which is not constructed yet...
    // SetMode(CanvasManagerMode::Unselected);
}


MultiSceneCanvasInteractor::~MultiSceneCanvasInteractor()
{
}


void MultiSceneCanvasInteractor::CallRepaint()
{
    canvasComponent->repaint();
}




// - - - - - - - - - - running Mode - - - - - - - - - -

void MultiSceneCanvasInteractor::SetMode(Miam::CanvasManagerMode newMode)
{
    // We don't do a specific action on every mode change !
    // But a few require checks and action
    switch (newMode) {
            
        case CanvasManagerMode::Unselected:
            if (mode != CanvasManagerMode::Unselected)
            {
                canvasComponent->SetIsSelectedForEditing(false);
                GetSelectedScene()->SetSelectedArea(nullptr, false);
            }
            break;
            
        case CanvasManagerMode::NothingSelected:
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

std::shared_ptr<IDrawableArea> MultiSceneCanvasInteractor::GetDrawableArea(int _index)
{
    return selectedScene->GetDrawableArea(_index);
}

size_t MultiSceneCanvasInteractor::GetDrawableAreasCount()
{
    return selectedScene->GetAreasCount();
}






// - - - - - Selection management (areas and scenes) - - - - -


void MultiSceneCanvasInteractor::SelectScene(int id)
{
    if ( 0 <= id && id < scenes.size() )
    {
        // No specific other check
        selectedScene = std::dynamic_pointer_cast<EditableScene>(scenes[id]);
        
        // Graphic updates
        
        CallRepaint();
    }
    else
    {
        std::string errorMsg = "Scene id " + std::to_string(id) + " does not exist inside multi-scene canvas " + std::to_string(selfId);
        throw std::runtime_error(errorMsg);
    }
}




// - - - - - - - - - - Areas Managing : Add and Delete - - - - - - - - - -

void MultiSceneCanvasInteractor::__AddTestAreas()
{
    for (int i=0 ; i < scenes.size() ; i++)
    {
        int areasCount = 3+(rand()%3);
        
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

void MultiSceneCanvasInteractor::OnCanvasMouseDown(Point<int>& clicLocation)
{
    // !!!!!!!!!!!!!! ON DOIT DIRE AU PARENT QU'ON SE SÉLECTIONNE SOI-MÊME !!!!!!!!!!!!!!
    graphicSessionManager->SetSelectedCanvas(this);
    
    std::string returnMsg = selectedScene->OnCanvasMouseDown(clicLocation);
    if (!returnMsg.empty())
        graphicSessionManager->DisplayInfo(returnMsg);
    
    // in any case (does not waste much computing time...)
    canvasComponent->repaint();
}
void MultiSceneCanvasInteractor::OnCanvasMouseDrag(Point<int>& mouseLocation)
{
    if (selectedScene->OnCanvasMouseDrag(mouseLocation))
        canvasComponent->repaint();
}
void MultiSceneCanvasInteractor::OnCanvasMouseUp()
{
    if (selectedScene->OnCanvasMouseUp())
        canvasComponent->repaint();
}

