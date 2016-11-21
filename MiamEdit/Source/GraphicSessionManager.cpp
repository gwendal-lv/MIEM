/*
  ==============================================================================

    GraphicSessionManager.cpp
    Created: 28 Mar 2016 5:27:18pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "GraphicSessionManager.h"


// Other includes

#include "View.h"

using namespace Miam;


// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(View* _view) :
    view(_view)
{
    setMode(GraphicSessionMode::Loading);
    
    // Links to the view module
    sceneEditionComponent = view->GetMainContentComponent()->GetSceneEditionComponent();
    
    // Canvases const count defined here PLUS OU MOINS
    // On doit créer les sous-objets de canevas avant de les transmettre au sous-module
    canvasManagers.push_back(new MultiSceneCanvasEditor(this, sceneEditionComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
    canvasManagers.push_back(new MultiSceneCanvasEditor(this, sceneEditionComponent->AddCanvas(),  SceneCanvasComponent::Id::Canvas2));
    
    // Links to the view module
    sceneEditionComponent->CompleteInitialization(this);
    
    // Finally, state of the presenter
    setMode(GraphicSessionMode::Loaded);
    // And states of the canvases
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
        canvasManagers[i]->SetMode(CanvasManagerMode::Unselected);
    
    
    // SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
    nextAreaId = 0; // plus tard : valeur contenue dans le fichier de sauvegarde
    SetSelectedCanvas(canvasManagers.front());
    
    
}

GraphicSessionManager::~GraphicSessionManager()
{
    for(int i=0 ; i<canvasManagers.size(); i++)
        delete canvasManagers[i];
}


// Testing purposes only
void GraphicSessionManager::__LoadDefaultTest()
{
    srand(2016); // GRAINE fixée ici
    
    for(int i=0 ; i<canvasManagers.size(); i++)
        canvasManagers[i]->__AddTestAreas();
}




// ===== SETTERS AND GETTERS =====
uint64_t GraphicSessionManager::GetNextAreaId()
{
    uint64_t areaIdBackup = nextAreaId;
    nextAreaId++;
    return areaIdBackup;
}
std::shared_ptr<IEditableArea> GraphicSessionManager::GetSelectedArea()
{
    if (selectedCanvas)
        return selectedCanvas->GetSelectedArea();
    else
        return nullptr;
}






// ===== UTILITIES =====



void GraphicSessionManager::SetSelectedCanvas(MultiSceneCanvasInteractor* _selectedCanvas)
{
    
    // We do something only if there has been a change
    if (selectedCanvas != _selectedCanvas)
    {
        // At first : unselection of previous canvas...
        if (selectedCanvas)
            selectedCanvas->SetMode(CanvasManagerMode::Unselected);
    
        
        selectedCanvas = dynamic_cast<MultiSceneCanvasEditor*>(_selectedCanvas);
        if (!selectedCanvas)
            throw std::runtime_error(std::string("The canvas to be selected is only an Interactor, and not an Editor (no editing features...)"));
        
        
        selectedCanvas->SetMode(CanvasManagerMode::NothingSelected);
    
        setMode(GraphicSessionMode::CanvasSelected);
    }
    else
    {
        // rien du tout
    }
}



/* Regroups all necessary actions on a mode change,
 * moreover : does not always necessarily allow the mode change requested !
 */
void GraphicSessionManager::setMode(GraphicSessionMode newMode)
{
    switch(newMode)
    {
        case GraphicSessionMode::Null :
            std::cout << "Presenter:: (GraphicSessionManager) : mode d'édition \"Null\" non-implémenté" << std::endl;
            break;
            
        case GraphicSessionMode::Loading :
            std::cout << "Presenter:: (GraphicSessionManager) : mode d'édition \"Loading\" non-implémenté" << std::endl;
            break;
            
        case GraphicSessionMode::Loaded :
            // nothing to paste when freshly loaded
            sceneEditionComponent->SetPasteEnabled(false);
            
        case GraphicSessionMode::NothingSelected :
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupReduced(true);
            sceneEditionComponent->SetSpatGroupReduced(true);
            sceneEditionComponent->SetInitialStateGroupHidden(true);
            sceneEditionComponent->resized(); // right menu update
            break;
            
        case  GraphicSessionMode::CanvasSelected :
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
            
        case GraphicSessionMode::AreaSelected :
            sceneEditionComponent->SetEnabledAllControls(true, true); // as we may come from "waiting for something creation/deletion"
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupReduced(false);
            if (areaToCopy)
                sceneEditionComponent->SetPasteEnabled(true);
            sceneEditionComponent->SetSpatGroupReduced(false);
            sceneEditionComponent->SetInitialStateGroupHidden(true);
            if (GetSelectedArea() == nullptr)
                throw std::runtime_error("No area selected, mode cannot be AreaSelected");
            sceneEditionComponent->SetAreaColourValue(GetSelectedArea()->GetFillColour());
            sceneEditionComponent->resized(); // right menu update
            
            view->DisplayInfo("Editing an Area");
            break;
            
            /*
         case GraphicSessionMode::EditingArea :
            break;*/
            
        case GraphicSessionMode::WaitingForPointCreation :
            sceneEditionComponent->DisableAllButtonsBut("Add Point text button");
            break;
            
        case GraphicSessionMode::WaitingForPointDeletion :
            sceneEditionComponent->DisableAllButtonsBut("Delete Point text button");
            break;
            
        default :
            break;
    }
    
    
    // Internal order : we finally don't even discuss it
    mode = newMode;
}


void GraphicSessionManager::CanvasModeChanged(CanvasManagerMode canvasMode)
{
    switch (canvasMode)
    {
        case CanvasManagerMode::NothingSelected :
            setMode(GraphicSessionMode::CanvasSelected);
            break;
        case CanvasManagerMode::AreaSelected :
            setMode(GraphicSessionMode::AreaSelected);
            break;
        case CanvasManagerMode::WaitingForPointCreation :
            setMode(GraphicSessionMode::WaitingForPointCreation);
            break;
        case CanvasManagerMode::WaitingForPointDeletion :
            setMode(GraphicSessionMode::WaitingForPointDeletion);
            break;
            
        default :
            break;
    }
}



// ===== EVENTS TO VIEW =====

void GraphicSessionManager::DisplayInfo(String info)
{
    view->DisplayInfo(info);
}






// ===== EVENTS FROM VIEW =====

void GraphicSessionManager::OnAddPoint()
{
    // Only if the button is not clicked a second time in a row
    if (mode != GraphicSessionMode::WaitingForPointCreation)
    {
        if (selectedCanvas != 0)
            selectedCanvas->SetMode(CanvasManagerMode::WaitingForPointCreation);
        else
            throw std::runtime_error("Cannot add a point to an area because no canvas is currently selected");
    }
    // Else, we cancel the point creation (an area was necessarily selected if we were to destroy something)
    else
        selectedCanvas->SetMode(CanvasManagerMode::AreaSelected);
}
void GraphicSessionManager::OnDeletePoint()
{
    // Only if the button is not clicked a second time in a row
    if (mode != GraphicSessionMode::WaitingForPointDeletion)
    {
        if (selectedCanvas != 0)
            selectedCanvas->SetMode(CanvasManagerMode::WaitingForPointDeletion);
        else
            throw std::runtime_error("Cannot add a point to an area because no canvas is currently selected");
    }
    // Else, we cancel the point deletion (an area was necessarily selected if we were to destroy something)
    else
        selectedCanvas->SetMode(CanvasManagerMode::AreaSelected);
}
void GraphicSessionManager::OnCopyArea()
{
    if(selectedCanvas)
    {
        auto localAreaToCopy = selectedCanvas->GetSelectedArea();
        if (localAreaToCopy)
            areaToCopy = localAreaToCopy;
        else
            throw std::runtime_error("Cannot copy an area... No area selected in SceneCanvasComponent::Id" + std::to_string(selectedCanvas->GetId()));
    }
    else
        throw std::runtime_error("Cannot copy an area if no canvas is selected...");
    
    // Graphical Update
    sceneEditionComponent->SetPasteEnabled(true);
}
void GraphicSessionManager::OnPasteArea()
{
    if (selectedCanvas)
    {
        if (areaToCopy)
        {
            // On va forcer l'appel au constructeur de copie
            // COPIE DE POLYGONE SEULEMENT
            std::shared_ptr<IEditableArea> newArea;
            
            // Casts sans doute inutiles ici....
            // Et le code serait + clair avec des méthode de ** Clonage ** à l'intérieur des classes...
            std::shared_ptr<EditablePolygon> newCastedPolygon = std::dynamic_pointer_cast<EditablePolygon>(areaToCopy);
            // Si le cast vers Polygone a bien fonctionné
            if (newCastedPolygon)
            {
                std::shared_ptr<EditablePolygon> newPolygon(new EditablePolygon(*(newCastedPolygon.get())));
                
                // 
                newArea = newPolygon;
            }
            // Sinon c'est l'alerte au gogole
            else
                throw std::runtime_error("Unable to cast the currently selectedArea to a polygon, in order to duplicate then paste it. Generic code not implemented.");
            
            // Puis : même procédure pour les cas possibles
            // Modification du polygone copié
            newArea->SetId(nextAreaId);
            
            // BESOIN DE SAVOIR SI ON CHANGE DE CANVAS OU NON ?
            // Si on change, besoin d'appeler une fonction du genre :
            // RescaleForCanvas(SceneCanvasComponent* )
            //
            // Juste translation par rapport à l'original, dans tous les cas...
            newArea->Translate(Point<double>(20,20));
            selectedCanvas->GetSelectedScene()->AddArea(newArea);
            selectedCanvas->GetSelectedScene()->SetSelectedArea(newArea);
            
            // Graphical Update
            selectedCanvas->CallRepaint();
        }
        else
            throw std::runtime_error("No valid area can be pasted from the clipboard.");
    }
    else
        throw std::runtime_error("Cannot paste an area : no canvas selected. Paste button should not be clickable at the moment.");
    
    
}
void GraphicSessionManager::OnAddArea()
{
    if (selectedCanvas)
    {
        selectedCanvas->GetSelectedScene()->AddDefaultArea(GetNextAreaId());
    }
    else
        throw std::runtime_error("Cannot add a new area : no canvas selected.");
}
void GraphicSessionManager::OnDeleteArea()
{
    if (selectedCanvas)
    {
        selectedCanvas->DeleteSelectedArea();
    }
    else
        throw std::runtime_error("No canvas selected. Delete Area button should not be clickable at the moment !");
}

void GraphicSessionManager::OnNewColour(Colour colour)
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


void GraphicSessionManager::OnSendToBack()
{
    if (selectedCanvas)
        selectedCanvas->OnSendToBack();
    else throw std::runtime_error("Cannot send something to back : no canvas selected");
}

void GraphicSessionManager::OnSendBackward()
{
    if (selectedCanvas)
        selectedCanvas->OnSendBackward();
    else throw std::runtime_error("Cannot send something backward : no canvas selected");
}
void GraphicSessionManager::OnBringForward()
{
    if (selectedCanvas)
        selectedCanvas->OnBringForward();
    else throw std::runtime_error("Cannot bring something forward : no canvas selected");
}
void GraphicSessionManager::OnBringToFront()
{
    if (selectedCanvas)
        selectedCanvas->OnBringToFront();
    else throw std::runtime_error("Cannot bring something to front : no canvas selected");
}


