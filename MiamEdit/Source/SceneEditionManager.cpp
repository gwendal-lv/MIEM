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


// ========== CONSTRUCTION and DESTRUCTION ==========

SceneEditionManager::SceneEditionManager(View* _view) :
    view(_view)
{
    setMode(SceneEditionMode::Loading);
    
    // Links to the view module
    sceneEditionComponent = view->GetMainContentComponent()->GetSceneEditionComponent();
    
    // Canvases const count defined within the View module...  NOT ANYMORE
    canvasManagers.push_back(new SceneCanvasManager(view, this, sceneEditionComponent, SceneCanvasComponent::Id::Canvas1));
    canvasManagers.push_back(new SceneCanvasManager(view, this, sceneEditionComponent,  SceneCanvasComponent::Id::Canvas2));
    
    // Links to the view module
    sceneEditionComponent->CompleteInitialization(this);
    
    // Finally, state of the presenter
    setMode(SceneEditionMode::Loaded);
    // And states of the canvases
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
        canvasManagers[i]->SetMode(SceneCanvasMode::Unselected);
    
    
    // SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
    nextAreaId = 0; // plus tard : valeur contenue dans le fichier de sauvegarde
    SetSelectedCanvas(canvasManagers.front());
    
    
}

SceneEditionManager::~SceneEditionManager()
{
    for(int i=0 ; i<canvasManagers.size(); i++)
        delete canvasManagers[i];
}


// Testing purposes only
void SceneEditionManager::__LoadDefaultTest()
{
    srand(2016); // GRAINE fixée ici
    
    for(int i=0 ; i<canvasManagers.size(); i++)
        canvasManagers[i]->__AddTestAreas();
}




// ===== SETTERS AND GETTERS =====
uint64_t SceneEditionManager::GetNextAreaId()
{
    uint64_t areaIdBackup = nextAreaId;
    nextAreaId++;
    return areaIdBackup;
}
std::shared_ptr<EditableArea> SceneEditionManager::GetSelectedArea()
{
    if (selectedCanvas)
        return selectedCanvas->GetSelectedArea();
    else
        return nullptr;
}






// ===== UTILITIES =====



void SceneEditionManager::SetSelectedCanvas(SceneCanvasManager* _selectedCanvas)
{
    
    // We do something only if there has been a change
    if (selectedCanvas != _selectedCanvas)
    {
        // At first : unselection of previous canvas...
        if (selectedCanvas)
            selectedCanvas->SetMode(SceneCanvasMode::Unselected);
    
        selectedCanvas = _selectedCanvas;
        selectedCanvas->SetMode(SceneCanvasMode::NothingSelected);
    
        setMode(SceneEditionMode::CanvasSelected);
    }
    else
    {
        // rien du tout
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
            
        case SceneEditionMode::Loaded :
            // nothing to paste when freshly loaded
            sceneEditionComponent->SetPasteEnabled(false);
            
        case SceneEditionMode::NothingSelected :
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupReduced(true);
            sceneEditionComponent->SetSpatGroupReduced(true);
            sceneEditionComponent->SetInitialStateGroupHidden(true);
            sceneEditionComponent->resized(); // right menu update
            break;
            
        case  SceneEditionMode::CanvasSelected :
            sceneEditionComponent->SetCanvasInfo(selectedCanvas->GetId());
            sceneEditionComponent->SetCanvasGroupHidden(false);
            sceneEditionComponent->SetAreaGroupReduced(true);
            if (areaToCopy)
                sceneEditionComponent->SetPasteEnabled(true);
            sceneEditionComponent->SetSpatGroupReduced(true);
            sceneEditionComponent->SetInitialStateGroupHidden(false);
            sceneEditionComponent->resized(); // right menu update
            
            view->DisplayInfo("Editing a Canvas and its Scenes");
            break;
            
        case SceneEditionMode::AreaSelected :
            sceneEditionComponent->SetEnabledAllControls(true, true); // as we may come from "waiting for something creation/deletion"
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupReduced(false);
            sceneEditionComponent->SetSpatGroupReduced(false);
            sceneEditionComponent->SetInitialStateGroupHidden(true);
            sceneEditionComponent->SetAreaColourValue(GetSelectedArea()->GetFillColour());
            sceneEditionComponent->resized(); // right menu update
            
            view->DisplayInfo("Editing an Area");
            break;
            
            /*
         case SceneEditionMode::EditingArea :
            break;*/
            
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


void SceneEditionManager::CanvasModeChanged(SceneCanvasMode canvasMode)
{
    switch (canvasMode)
    {
        case SceneCanvasMode::NothingSelected :
            setMode(SceneEditionMode::CanvasSelected);
            break;
        case SceneCanvasMode::AreaSelected :
            setMode(SceneEditionMode::AreaSelected);
            break;
        case SceneCanvasMode::WaitingForPointCreation :
            setMode(SceneEditionMode::WaitingForPointCreation);
            break;
        case SceneCanvasMode::WaitingForPointDeletion :
            setMode(SceneEditionMode::WaitingForPointDeletion);
            break;
            
        default :
            break;
    }
}



// ===== EVENTS =====

void SceneEditionManager::OnAddPoint()
{
    // Only if the button is not clicked a second time in a row
    if (mode != SceneEditionMode::WaitingForPointCreation)
    {
        if (selectedCanvas != 0)
            selectedCanvas->SetMode(SceneCanvasMode::WaitingForPointCreation);
        else
            throw std::runtime_error("Cannot add a point to an area because no canvas is currently selected");
    }
    // Else, we cancel the point creation (an area was necessarily selected if we were to destroy something)
    else
        selectedCanvas->SetMode(SceneCanvasMode::AreaSelected);
}
void SceneEditionManager::OnDeletePoint()
{
    // Only if the button is not clicked a second time in a row
    if (mode != SceneEditionMode::WaitingForPointDeletion)
    {
        if (selectedCanvas != 0)
            selectedCanvas->SetMode(SceneCanvasMode::WaitingForPointDeletion);
        else
            throw std::runtime_error("Cannot add a point to an area because no canvas is currently selected");
    }
    // Else, we cancel the point deletion (an area was necessarily selected if we were to destroy something)
    else
        selectedCanvas->SetMode(SceneCanvasMode::AreaSelected);
}
void SceneEditionManager::OnCopyArea()
{
    if(selectedCanvas)
    {
        std::shared_ptr<EditableArea> localAreaToCopy = selectedCanvas->GetSelectedArea();
        if (localAreaToCopy)
            areaToCopy = localAreaToCopy;
        else
            throw std::runtime_error(std::string("Cannot copy an area... No area selected in SceneCanvasComponent::Id") + std::to_string(selectedCanvas->GetId())); // C++11
    }
    else
        throw std::runtime_error("Cannot copy an area if no canvas is selected...");
    
    // Graphical Update
    sceneEditionComponent->SetPasteEnabled(true);
}
void SceneEditionManager::OnPasteArea()
{
    if (selectedCanvas)
    {
        if (areaToCopy)
        {
            // On va forcer l'appel au constructeur de copie
            // COPIE DE POLYGONE SEULEMENT
            std::shared_ptr<EditableArea> newArea;
            
            // Casts sans doute inutiles ici....
            // Et le code serait + clair avec des méthode de ** Clonage ** à l'intérieur des classes...
            std::shared_ptr<EditablePolygon> newCastedPolygon = std::dynamic_pointer_cast<EditablePolygon>(areaToCopy);
            // Si le cast vers Polygone a bien fonctionné
            if (newCastedPolygon)
            {
                std::shared_ptr<EditablePolygon> newPolygon(new EditablePolygon(*(newCastedPolygon.get())));
                newArea = std::static_pointer_cast<EditableArea>(newPolygon);
            }
            // Sinon c'est l'alerte au gogole
            else
                throw std::runtime_error("Unable to cast the currently selectedArea to a polygon, in order to duplicate then paste it. Generic code not implemented.");
            
            // Pui : même procédure pour les cas possibles
            // Modification du polygone copié
            newArea->SetId(nextAreaId);
            
            // BESOIN DE SAVOIR SI ON CHANGE DE CANVAS OU NON ?
            // Si on change, besoin d'appeler une fonction du genre :
            // RescaleForCanvas(SceneCanvasComponent* )
            //
            // Juste translation par rapport à l'original, dans tous les cas...
            newArea->Translate(Point<double>(20,20));
            selectedCanvas->AddEditableArea(newArea, true);
        }
        else
            throw std::runtime_error("No valid area can be pasted from the clipboard.");
    }
    else
        throw std::runtime_error("Cannot paste an area : no canvas selected. Paste button should not be clickable at the moment.");
    
    // Graphical Update
    // none
}
void SceneEditionManager::OnAddArea()
{
    if (selectedCanvas)
    {
        selectedCanvas->AddDefaultArea(GetNextAreaId());
    }
    else
        throw std::runtime_error("Cannot add a new area : no canvas selected.");
}
void SceneEditionManager::OnDeleteArea()
{
    if (selectedCanvas)
    {
        selectedCanvas->DeleteSelectedArea();
        sceneEditionComponent->repaint();
    }
    else
        throw std::runtime_error("No canvas selected. Delete Area button should not be clickable at the moment !");
}

void SceneEditionManager::OnNewColour(Colour colour)
{
    std::shared_ptr<DrawableArea> selectedArea = std::dynamic_pointer_cast<DrawableArea>(GetSelectedArea());
    if (selectedArea)
    {
        selectedArea->SetFillColour(colour);
        selectedCanvas->CallRepaint();
    }
    else
        std::runtime_error("The given colour cannot be applied : no area is selected");
}

void SceneEditionManager::OnSendToBack()
{
    if (selectedCanvas)
        selectedCanvas->SendSelectedAreaToBack();
    else throw std::runtime_error("Cannot send an area to back : no canvas selected");
}

void SceneEditionManager::OnSendBackward()
{
    if (selectedCanvas)
        selectedCanvas->SendSelectedAreaBackward();
    else throw std::runtime_error("Cannot send an area backward : no canvas selected");
}
void SceneEditionManager::OnBringForward()
{
    if (selectedCanvas)
        selectedCanvas->SendSelectedAreaForward();
    else throw std::runtime_error("Cannot send an area forward : no canvas selected");
}
void SceneEditionManager::OnBringToFront()
{
    if (selectedCanvas)
        selectedCanvas->SendSelectedAreaToFront();
    else throw std::runtime_error("Cannot send an area to front : no canvas selected");
}


