 /*
  ==============================================================================

    SceneCanvas.cpp
    Created: 8 Oct 2016 1:02:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#include <iterator>     // std::advance


#include "MultiSceneCanvasComponent.h"
#include "MultiSceneCanvasInteractor.h"


#include "IGraphicSessionManager.h"

#include "EditableScene.h"

#include "SceneEvent.h"
#include "MultiAreaEvent.h"


using namespace Miam;



MultiSceneCanvasInteractor::MultiSceneCanvasInteractor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId)
{
    mode = CanvasManagerMode::Loading;
    
    graphicSessionManager = _graphicSessionManager;
    selfId = _selfId;
    
    // CANVAS DOIT DEJA ETRE CRÉÉ 
    canvasComponent = _canvasComponent;
    

    
    
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


void MultiSceneCanvasInteractor::handleAsyncUpdate()
{
    graphicSessionManager->CallPresenterUpdate();
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
            {
                auto areaE = selectedScene->SetSelectedArea(nullptr, false);
                handleAndSendAreaEventSync(areaE);
            }
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

uint64_t MultiSceneCanvasInteractor::GetNextAreaId()
{
    return graphicSessionManager->GetNextAreaId();
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



// - - - - - Internal events management - - - - -

void MultiSceneCanvasInteractor::handleAndSendEventSync(std::shared_ptr<GraphicEvent> graphicE)
{
    // - - - At first : dispatching if necessary - - -
    // Event about an Area
    if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(graphicE))
    {
        handleAndSendAreaEventSync(areaE);
    }
    
    // - - - Generic event (no dispatching done) - - -
    else
    {
        // - Internal update of the necessary thread-safe drawable objects -
        // (for OpenGL rendering on background thread)
        
        // Event about a Scene : we may recreate all multi-threaded data
        if (auto sceneE = std::dynamic_pointer_cast<SceneEvent>(graphicE))
        {
            switch(sceneE->GetType())
            {
                    // Scene Change : we update all objects
                case SceneEventType::SceneChanged :
                    recreateAllAsyncDrawableObjects();
                    break;
                    
                // We break if nothing happened (empty event)
                    // or if event does not require areas update
                case SceneEventType::NothingHappened :
                case SceneEventType::Added :
                case SceneEventType::Deleted :
                    break;
                    
                    
                default :
                    break;
            }
        }
        
        // -  Transmission of the event to the parent graphic session manager -
        graphicSessionManager->HandleEventSync(graphicE);
    }
}
void MultiSceneCanvasInteractor::handleAndSendAreaEventSync(std::shared_ptr<AreaEvent> areaE)
{
    // - - - Internal update of the necessary thread-safe drawable objects - - -
    // (for OpenGL rendering on background thread)
    switch(areaE->GetType())
    {
        case AreaEventType::Added :
            // Internally checks the given idInScene
            addAsyncDrawableObject(areaE->GetAreaIdInScene(), areaE->GetConcernedArea());
            break;
            
        case AreaEventType::Deleted :
            if (areaE->GetAreaIdInScene() < 0)
                throw std::runtime_error("A deletion Miam::AreaEvent must always contain the id of the object to delete.");
            else
                deleteAsyncDrawableObject(areaE->GetAreaIdInScene(), areaE->GetConcernedArea());
            break;
            
        default : // Any movement : update of the concerned area (if any)
            if (areaE->GetConcernedArea())
            {
                updateAsyncDrawableObject(areaE->GetConcernedArea());
                if (auto multiAreaE = std::dynamic_pointer_cast<MultiAreaEvent>(areaE))
                {
                    for (int i=0 ; i<(int)multiAreaE->GetOtherEventsCount() ; i++)
                    {
                        updateAsyncDrawableObject(multiAreaE->GetOtherEvent(i)->GetConcernedArea());
                    }
                }
            }
            break;
    }
    
    // - - - Transmission of the event to the parent graphic session manager - - -
    graphicSessionManager->HandleEventSync(areaE);
}




// - - - - - Thread-safe methods (for OpenGL async drawing) - - - - -
// internal routines

void MultiSceneCanvasInteractor::recreateAllAsyncDrawableObjects()
{
    // Cloning of all drawable areas (memory allocations) happen before locking
    // the mutex
    std::vector<std::shared_ptr<IDrawableArea>> syncAllocatedAreaCopies;
    syncAllocatedAreaCopies.reserve(selectedScene->GetDrawableObjectsCount());
    for (size_t i=0 ; i<selectedScene->GetDrawableObjectsCount() ; i++)
    {
        syncAllocatedAreaCopies.push_back(std::shared_ptr<IDrawableArea>(selectedScene->GetDrawableObject(i)->Clone()));
    }
    
    // Actual assignation (OpenGL renderers get now the freshest data)
    asyncDrawableObjectsMutex.lock();
    
    originalToAsyncObject.clear();
    asyncDrawableObjects.clear();
    for (size_t i=0 ; i<selectedScene->GetDrawableObjectsCount() ; i++)
    {
        asyncDrawableObjects.push_back(syncAllocatedAreaCopies[i]);
        originalToAsyncObject[selectedScene->GetDrawableObject(i)] = std::prev(asyncDrawableObjects.end()); // actual insertion here
    }
    
    asyncDrawableObjectsMutex.unlock();
}
void MultiSceneCanvasInteractor::addAsyncDrawableObject(int insertionIdInScene, std::shared_ptr<IDrawableArea> originalAreaToAdd)
{
    /* Memory allocation done outside of the lock (may change almost nothing... and
     * cost 2 memory allocation of std::shared_ptr ? To be tested for optimization...
     */
    std::shared_ptr<IDrawableArea> areaCopy = std::shared_ptr<IDrawableArea>(originalAreaToAdd->Clone());
    
    // Actual addition then
    asyncDrawableObjectsMutex.lock();
    if (insertionIdInScene == (int)asyncDrawableObjects.size())
    {
        asyncDrawableObjects.push_back(areaCopy);
        originalToAsyncObject[originalAreaToAdd] = std::prev(asyncDrawableObjects.end());
        asyncDrawableObjectsMutex.unlock();
    }
    else if (insertionIdInScene < (int)asyncDrawableObjects.size())
    {
        auto oldAsyncObjectIt = asyncDrawableObjects.begin();
        std::advance(oldAsyncObjectIt, insertionIdInScene);
        auto newIt = asyncDrawableObjects.insert(oldAsyncObjectIt, areaCopy);
        originalToAsyncObject[originalAreaToAdd] = newIt; // insertion in map here
        asyncDrawableObjectsMutex.unlock();
    }
    else
    {
        // unlock before the exception, in case it is catched and the program continues
        asyncDrawableObjectsMutex.unlock();
        throw std::runtime_error("Id of new area to draw asynchronously is not coherent");
    }
}
void MultiSceneCanvasInteractor::updateAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToUpdate)
{
    // Done outside a mutex lock, as it can take some time...
    // make_shared not possible here
    std::shared_ptr<IDrawableArea> newAsyncObjectSharedPtr = std::shared_ptr<IDrawableArea>(originalAreaToUpdate->Clone());
    
    std::list<std::shared_ptr<IDrawableArea>>::iterator asyncObjectIt;
    
    // Locked change within the MT-unsafe std::vector and associated structures
    asyncDrawableObjectsMutex.lock();
    try {
        // Map search without key/value pair automatic insertion
        asyncObjectIt = originalToAsyncObject.at(originalAreaToUpdate);
    } catch (std::out_of_range e) {
        throw std::runtime_error(std::string("Cannot update an IDrawableArea for OpenGL (not referenced yet). std::out_of_range : ") + e.what());
    }
    *asyncObjectIt = newAsyncObjectSharedPtr;
    asyncDrawableObjectsMutex.unlock();
}
void MultiSceneCanvasInteractor::deleteAsyncDrawableObject(int idInScene, std::shared_ptr<IDrawableArea> originalAreaToDelete)
{
    // At first : deletion of the pointer from the map
    auto mapIt = originalToAsyncObject.find(originalAreaToDelete);
    auto asyncObjectsIt = mapIt->second;
    originalToAsyncObject.erase(mapIt);
    
    // Then, actual deletion of the area
    asyncDrawableObjectsMutex.lock();
    asyncDrawableObjects.erase(asyncObjectsIt);
    asyncDrawableObjectsMutex.unlock();
}




// - - - - - Selection management (areas and scenes) - - - - -


void MultiSceneCanvasInteractor::SelectScene(int id)
{
    if ( 0 <= id && id < (int)(scenes.size()) )
    {
        // For storing possible events that may happen on unselection
        std::vector<std::shared_ptr<GraphicEvent>> unselectionEvents;
        
        // Unselection of any area first
        //SetMode(CanvasManagerMode::NothingSelected); // Something strange here..........
        // and Deactivation of the scene
        if (selectedScene)
            unselectionEvents = selectedScene->OnUnselection();
        
        // Then : we become the new selected canvas (if we were not before)
        graphicSessionManager->SetSelectedCanvas(shared_from_this());
        // No specific other check, we just create the informative event before changing
		
        auto graphicE = std::make_shared<SceneEvent>(shared_from_this(), selectedScene, scenes[id],SceneEventType::SceneChanged);
        selectedScene = std::dynamic_pointer_cast<EditableScene>(scenes[id]);
        selectedScene->OnSelection();
        
        SetMode(CanvasManagerMode::SceneOnlySelected);
        
        // Graphic updates
        canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
        
        // Unselection events transmission here
        for (size_t i=0 ; i<unselectionEvents.size() ; i++)
            handleAndSendEventSync(unselectionEvents[i]);
        // Finally : info sent to this itself, and the graphic session manager
        handleAndSendEventSync(graphicE);
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
    auto newScene = std::make_shared<EditableScene>(shared_from_this(), canvasComponent->GetCanvas());
    newScene->SetName(name);
    AddScene(newScene);
}
void MultiSceneCanvasInteractor::AddScene(std::shared_ptr<EditableScene> newScene)
{
    scenes.push_back( newScene );
    auto sceneE = std::make_shared<SceneEvent>(shared_from_this(), newScene, SceneEventType::Added);
    handleAndSendEventSync(sceneE);

    SelectScene((int)(scenes.size())-1);
    
    // Graphical updates
    canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
}
bool MultiSceneCanvasInteractor::DeleteScene()
{
	DBG("Deleeeeete scene");
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
		std::shared_ptr<SceneEvent> sceneE(new SceneEvent(shared_from_this(), *it, SceneEventType::Deleted));
		handleAndSendEventSync(sceneE);

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

void MultiSceneCanvasInteractor::addAreaToScene(std::shared_ptr<EditableScene> scene_, std::shared_ptr<IInteractiveArea> area_)
{
    auto areaE = scene_->AddArea(area_);
    handleAndSendAreaEventSync(areaE);
}





// - - - - - - - - - - Events from canvas - - - - - - - - - -

void MultiSceneCanvasInteractor::OnCanvasMouseDown(const MouseEvent& mouseE)
{
    // !!!!!!!!!!!!!! ON DOIT DIRE AU PARENT QU'ON SE SÉLECTIONNE SOI-MÊME !!!!!!!!!!!!!!
    graphicSessionManager->SetSelectedCanvas(shared_from_this());
    
    std::shared_ptr<GraphicEvent> graphicE = selectedScene->OnCanvasMouseDown(mouseE);
    if (! graphicE->GetMessage().empty())
        graphicSessionManager->DisplayInfo(graphicE->GetMessage()); // TO DO : CORRIGEEEEEERRRR !!!!!
    
    // If we were in a "special waiting mode" we end it after this click
    if (mode == CanvasManagerMode::WaitingForPointCreation || mode == CanvasManagerMode::WaitingForPointDeletion)
    {
        // CHANGEMENT SELON L'ÉVÈNEMENT RETOURNÉ ???
        SetMode(CanvasManagerMode::AreaSelected); // ON GARDE L'AIRE SÉLECTIONNÉE POUR L'INSTANT
    }
    
    // in any case : repaint() (does not waste much computing time...)
    //canvasComponent->repaint(); // useless with OpenGL
    
    // Event transmission towards the audio interpretation
    handleAndSendEventSync(graphicE);
}
void MultiSceneCanvasInteractor::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
    std::shared_ptr<GraphicEvent> graphicE = selectedScene->OnCanvasMouseDrag(mouseE);
    
    // Event transmission towards the audio interpretation
    handleAndSendEventSync(graphicE);
}
void MultiSceneCanvasInteractor::OnCanvasMouseUp(const MouseEvent& mouseE)
{
    std::shared_ptr<GraphicEvent> graphicE = selectedScene->OnCanvasMouseUp(mouseE);
    
    // canvasComponent->repaint(); // useless with OpenGL
    
    // Event transmission towards the audio interpretation
    handleAndSendEventSync(graphicE);
}

void MultiSceneCanvasInteractor::OnResized()
{
    recreateAllAsyncDrawableObjects();
}

