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
    /* Comportement assez asymétrique concernant les excitateurs.
     * Si on sort d'un mode excitateur, on arrête toutes les transfos (et on vérifie
     * ça hors du switch juste en-dessous, à la dégueu quoi (à améliorer))
     * Si on entre dans le mode excitateur : c'est géré dans le switch proprement
     */
    if (
        (mode == CanvasManagerMode::ExcitersEdition || mode == CanvasManagerMode::ExciterSelected)
        &&
        (mode != CanvasManagerMode::ExcitersEdition && mode != CanvasManagerMode::ExciterSelected)
        )
    {
        selectedScene->StopCurrentTransformations();
    }
    
    // We don't do a specific action on every mode change !
    // But a few require checks and action
    switch (newMode) {
        
        case CanvasManagerMode::Unselected:
            // Unselection of a selected area (1 area selected for all canvases...)
            // And everything becomes dark, for another canvas to become
            if (selectedScene) // on first scene adding... there would be a problem
            {
                auto areaE = selectedScene->SetSelectedArea(nullptr, false);
                selectedScene->EnableExcitersLowOpacity(true);
                selectedScene->EnableAreasLowOpacity(true);
                // Pas d'évènements renvoyés pour les opacités : on update le tout
                recreateAllAsyncDrawableObjects();
                //handleAndSendAreaEventSync(areaE);
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
            
            // Mise en quasi-transparence des excitateurs seulement
            if (selectedScene) // sinon pb à l'initialisation
            {
                selectedScene->EnableExcitersLowOpacity(true);
                selectedScene->EnableAreasLowOpacity(false);
                // Pas d'évènements renvoyés : on update le tout
                recreateAllAsyncDrawableObjects();
            }
            break;
            
        // Quand on passe en mode excitateurs (on y passe forcément avant
        // d'en sélectionner un...), on arrête les transfos en cours
        // !! On doit vérifier qu'on était pas déjà en mode excitateurs !
        case CanvasManagerMode::ExcitersEdition :
            if (mode != CanvasManagerMode::ExciterSelected)
                selectedScene->StopCurrentTransformations();
            
            // Mise en quasi-transparence des aires graphiques à exciter seulement
            selectedScene->EnableExcitersLowOpacity(false);
            selectedScene->EnableAreasLowOpacity(true);
            // Pas d'évènements renvoyés : on update le tout
            recreateAllAsyncDrawableObjects();
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

std::chrono::time_point<std::chrono::steady_clock> MultiSceneCanvasInteractor::GetCommonTimePoint() const
{
    return graphicSessionManager->GetCommonTimePoint();
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

void MultiSceneCanvasInteractor::handleAndSendAreaEventSync(std::shared_ptr<AreaEvent> areaE, bool notifyGraphicSessionManager)
{
    // Re-dispatching if necessary
    if (auto multiAreaE = std::dynamic_pointer_cast<MultiAreaEvent>(areaE))
    {
        handleAndSendMultiAreaEventSync(multiAreaE);
    }
    
    // On ne traite que si c'est un pur évènement simple
    else
    {
        // - - - Internal update of the necessary thread-safe drawable objects - - -
        // (for OpenGL rendering on background thread)
        switch(areaE->GetType())
        {
            case AreaEventType::Added :
                // ATTENTION : ici, on traite le côté graphique de l'évènement.
                // On ne traite donc rien si l'aire ajoutée ne provient pas de la scène sélectionnée !
                if (selectedScene && selectedScene == areaE->GetConcernedScene())
                {
                    // Internally checks the given idInScene
                    addAsyncDrawableObject(areaE->GetAreaIdInScene(), areaE->GetConcernedArea());
                }
                break;
                
            case AreaEventType::Deleted :
                // Idem que l'ajout :
                if (selectedScene && selectedScene == areaE->GetConcernedScene())
                {
                    // The object's index is not needed anymore for deletion
                    deleteAsyncDrawableObject(/*areaE->GetAreaIdInScene(), */
                                              areaE->GetConcernedArea());
                }
                break;
                
            default : // Any movement : update of the concerned area (if any)
                // Idem que l'ajout :
                if (selectedScene && selectedScene == areaE->GetConcernedScene())
                {
                    if (areaE->GetConcernedArea())
                    {
                        updateAsyncDrawableObject(areaE->GetConcernedArea());
                    }
                }
                break;
        }
        
        // - - - Transmission of the event to the parent graphic session manager - - -
        if (notifyGraphicSessionManager)
            graphicSessionManager->HandleEventSync(areaE);
    }
}

void MultiSceneCanvasInteractor::handleAndSendMultiAreaEventSync(std::shared_ptr<MultiAreaEvent> multiAreaE)
{
    // Cast de l'évènement principal vers la classe mère
    auto mainAreaE = std::make_shared<AreaEvent>( multiAreaE.get() );
    handleAndSendAreaEventSync(mainAreaE, false); // pas de notif
    
    // Dispatching des sous-évènements
    for (size_t i=0 ; i<multiAreaE->GetOtherEventsCount() ; i++)
    {
        handleAndSendAreaEventSync(multiAreaE->GetOtherEvent(i), false); // pas de notif
    }
    
    // Ensuite on envoie le gros multiarea event au GraphicSessionManager en 1 paquet
    // (plutôt que chacun des petits séparément, pour lesquels la notification a été supprimée)
    graphicSessionManager->HandleEventSync(multiAreaE);
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
void MultiSceneCanvasInteractor::deleteAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToDelete)
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
    std::shared_ptr<SceneEvent> graphicE;
    // For storing possible events that may happen on unselection
    std::vector<std::shared_ptr<GraphicEvent>> unselectionEvents;
    // Unselection of any area first
    //SetMode(CanvasManagerMode::NothingSelected); // Something strange here..........
    // and Deactivation of the scene
    if (selectedScene)
        unselectionEvents = selectedScene->OnUnselection();
    
    if ( 0 <= id && id < (int)(scenes.size()) )
    {
        // Then : we become the new selected canvas (if we were not before)
        graphicSessionManager->SetSelectedCanvas(shared_from_this());
        // No specific other check, we just create the informative event before changing
		
        graphicE = std::make_shared<SceneEvent>(shared_from_this(), selectedScene, scenes[id],SceneEventType::SceneChanged);
        selectedScene = scenes[id];
        selectedScene->OnSelection();
        
        SetMode(CanvasManagerMode::SceneOnlySelected);
        
        // Graphic updates
        canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
        
    }
    else if (id != -1) // -1 is "tolerated"....
    {
        std::string errorMsg = "Scene id " + std::to_string(id) + " does not exist inside multi-scene canvas " + std::to_string(selfId);
        throw std::runtime_error(errorMsg);
    }
    
    // Unselection events transmission here, at the end
    for (size_t i=0 ; i<unselectionEvents.size() ; i++)
        handleAndSendEventSync(unselectionEvents[i]);
    // Finally : info sent to this itself, and the graphic session manager
    handleAndSendEventSync(graphicE);
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
    if (scenes.size()<=1)
        return false;
    else
    {
        // At first, we select the previous scene (or the next if it was the first...)
        int previousSelectedSceneId = GetSelectedSceneId();
        if (previousSelectedSceneId == 0)
            SelectScene(1);
        else
            SelectScene(previousSelectedSceneId-1);

        // Then we remove this one
        forceDeleteScene(previousSelectedSceneId);
        
        // Graphical updates
        canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
        canvasComponent->repaint();
        
        // Finally we re-select the same scene to force some updates...
        // Optimisation could be made here
        SelectScene(GetSelectedSceneId());
        
        return true;
    }
}
void MultiSceneCanvasInteractor::forceDeleteScene(size_t sceneIndexToDelete)
{
    auto it = scenes.begin() + sceneIndexToDelete;
    std::shared_ptr<SceneEvent> sceneE(new SceneEvent(shared_from_this(), *it, SceneEventType::Deleted));
    handleAndSendEventSync(sceneE);
    scenes.erase(it);
}




// - - - - - - - - - - Areas Managing : Add and Delete - - - - - - - - - -

void MultiSceneCanvasInteractor::addAreaToScene(std::shared_ptr<EditableScene> scene_, std::shared_ptr<IInteractiveArea> area_)
{
    std::shared_ptr<AreaEvent> areaE;
    
    if (auto exciter = std::dynamic_pointer_cast<Exciter>(area_))
        areaE = scene_->AddExciter(exciter, false); // sans sélection
    else
        areaE = scene_->AddArea(area_);
    
    // Attention ici : on ne traite graphiquement l'évènement que si la scène est la scène sélectionnée...
    handleAndSendAreaEventSync(areaE);
}
void MultiSceneCanvasInteractor::AddAreaToScene(size_t sceneIndex, std::shared_ptr<IInteractiveArea> area_)
{
    addAreaToScene(scenes[sceneIndex], area_);
}

// - - - - - - - - - - Exciters management - - - - - - - - - -
void MultiSceneCanvasInteractor::OnAddExciter()
{
    if (selectedScene)
    {
        auto areaE = selectedScene->AddDefaultExciter();
        // Après coup, on analyse l'état de sélection d'un excitateur
        // Comme ça c'est la scène qui choisit si elle sélectionne le nouvellement créé, ou non...
        if (selectedScene->GetSelectedExciter())
            SetMode(CanvasManagerMode::ExciterSelected);
        else
            SetMode(CanvasManagerMode::ExcitersEdition);

        
        handleAndSendAreaEventSync(areaE);
    }
    else throw std::runtime_error("Cannot add an exciter : no scene selected");
}
void MultiSceneCanvasInteractor::OnDeleteExciter()
{
    if (selectedScene)
    {
        auto areaE = selectedScene->DeleteSelectedExciter();
        
        handleAndSendAreaEventSync(areaE);
        
        // Changement de mode : plus d'excitateur sélectionné.
        SetMode(CanvasManagerMode::ExcitersEdition);
    }
    else throw std::runtime_error("Cannot delete an exciter : no scene selected");
}




// - - - - - - - - - - Events from canvas - - - - - - - - - -

void MultiSceneCanvasInteractor::OnCanvasMouseDown(const MouseEvent& mouseE)
{
    std::shared_ptr<GraphicEvent> graphicE;
    
    // !!! ON DOIT DIRE AU PARENT QU'ON SE SÉLECTIONNE SOI-MÊME !!!
    graphicSessionManager->SetSelectedCanvas(shared_from_this());
    
    // Action selon le mode
    switch (mode)
    {
        // Cas où l'on utilise les fonctionnalités "Editable" de la scène
        case CanvasManagerMode::SceneOnlySelected :
        case CanvasManagerMode::AreaSelected :
        case CanvasManagerMode::EditingArea :
            
            graphicE = selectedScene->OnCanvasMouseDown(mouseE);
            if (! graphicE->GetMessage().empty())
                graphicSessionManager->DisplayInfo(graphicE->GetMessage()); // TO DO : CORRIGEEEEEERRRR !!!!!
            break;
            
        // Cas où l'on utilise seulement les fonctionnalités "Interactive"
        // de la scène (les excitateurs, en gros)
        case CanvasManagerMode::ExcitersEdition :
            graphicE = selectedScene->InteractiveScene::OnCanvasMouseDown(mouseE);
            // Après coup, on analyse l'état de sélection d'un excitateur
            if (selectedScene->GetSelectedExciter())
                SetMode(CanvasManagerMode::ExciterSelected);
            break;
            
        case CanvasManagerMode::ExciterSelected :
            graphicE = selectedScene->InteractiveScene::OnCanvasMouseDown(mouseE);
            // Après coup, on analyse l'état de sélection d'un excitateur
            if (selectedScene->GetSelectedExciter() == nullptr)
                SetMode(CanvasManagerMode::ExcitersEdition);
            break;
            
        // Cas particuliers : modes d'attente d'évènement
        case CanvasManagerMode::WaitingForPointCreation :
        case CanvasManagerMode::WaitingForPointDeletion :
            
            graphicE = selectedScene->OnCanvasMouseDown(mouseE);
            if (! graphicE->GetMessage().empty())
                graphicSessionManager->DisplayInfo(graphicE->GetMessage()); // TO DO : CORRIGEEEEEERRRR !!!!!
            // ON GARDE L'AIRE SÉLECTIONNÉE POUR L'INSTANT
            SetMode(CanvasManagerMode::AreaSelected);
            break;
            
        // Default : bizzare de se retrouver là non ?
        default:
            break;
    }
    
    // Event transmission towards the audio interpretation
    handleAndSendEventSync(graphicE);
}
void MultiSceneCanvasInteractor::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
    std::shared_ptr<GraphicEvent> graphicE;
    
    // Action selon le mode
    switch (mode)
    {
        // Cas où l'on utilise seulement les fonctionnalités "Interactive"
            // de la scène (les excitateurs, en gros)
        case CanvasManagerMode::ExcitersEdition :
        case CanvasManagerMode::ExciterSelected :
            graphicE = selectedScene->InteractiveScene::OnCanvasMouseDrag(mouseE);
            break;
            
            // Default : tous les autres modes
        default:
            graphicE = selectedScene->OnCanvasMouseDrag(mouseE);
            break;
    }
    
    // Event transmission towards the audio interpretation
    handleAndSendEventSync(graphicE);
}
void MultiSceneCanvasInteractor::OnCanvasMouseUp(const MouseEvent& mouseE)
{
    std::shared_ptr<GraphicEvent> graphicE;
    // Action selon le mode
    switch (mode)
    {
            // Cas où l'on utilise seulement les fonctionnalités "Interactive"
            // de la scène (les excitateurs, en gros)
        case CanvasManagerMode::ExcitersEdition :
        case CanvasManagerMode::ExciterSelected :
            graphicE = selectedScene->InteractiveScene::OnCanvasMouseUp(mouseE);
            break;
            
            // Default : tous les autres modes
        default:
            graphicE = selectedScene->OnCanvasMouseUp(mouseE);
            break;
    }
    // Event transmission towards the audio interpretation
    handleAndSendEventSync(graphicE);
}

void MultiSceneCanvasInteractor::OnResized()
{
    recreateAllAsyncDrawableObjects();
}


// = = = = = = = = = = XML import/export = = = = = = = = = =
void MultiSceneCanvasInteractor::OnXmlLoadFinished()
{
    // Sauvegarde des excitateurs courants ajoutés, pour TOUTES les scènes
    for (size_t i =0 ; i<scenes.size() ; i++)
    {
        scenes[i]->SaveCurrentExcitersToInitialExciters();
    }
    
    // Le canevas se sélectionne lui-même (un peu comme
    // s'il y avait eu un clic de souris)
    SelectScene(0);
}

std::shared_ptr<bptree::ptree> MultiSceneCanvasInteractor::GetTree()
{
    bptree::ptree scenesInnerTree;
    // Internal properties. Some others may be written, this function can be overriden
    auto canvasInnerTree = std::make_shared<bptree::ptree>();
    canvasInnerTree->put("<xmlattr>.index", selfId);
    // Scenes writing
    for (size_t i=0 ; i<scenes.size() ; i++)
    {
        // On sauvegarde les excitateurs courants dans les initiaux,
        scenes[i]->SaveCurrentExcitersToInitialExciters();
        // Avant de chopper les initiaux :
        // Getting of the scene tree, then actual addition to the canvas tree
        auto sceneTree = scenes[i]->GetTree();
        sceneTree->put("<xmlattr>.index", i);
        scenesInnerTree.add_child("scene", *sceneTree);
    }
    canvasInnerTree->add_child("scenes", scenesInnerTree);
    return canvasInnerTree;
}






