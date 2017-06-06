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
#include "Presenter.h"

#include "SceneEvent.h"

#include "SpatPolygon.h"

using namespace Miam;


// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(View* _view, Presenter* presenter_) :
    IGraphicSessionManager(presenter_),
    view(_view)
{
    setMode(GraphicSessionMode::Loading);
    
    
    // SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
    nextAreaId = 0; // plus tard : valeur contenue dans le fichier de sauvegarde
    
    // Links to the view module
    sceneEditionComponent = view->GetMainContentComponent()->GetSceneEditionComponent();
    
    // ICI ON CHARGE DES TRUCS
    // Canvases const count defined here PLUS OU MOINS
    // On doit créer les sous-objets graphiques de canevas (View) avant de
    // les transmettre au sous-module de gestion de canevas (Presenter) que l'on crée
    // d'ailleurs ici aussi.
    canvasManagers.push_back(std::shared_ptr<MultiSceneCanvasInteractor>(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1)));    
    canvasManagers.push_back(std::shared_ptr<MultiSceneCanvasInteractor>(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas2)));
    
    
    // Links to the view module
    sceneEditionComponent->CompleteInitialization(this, multiCanvasComponent);

    for (size_t i=0 ; i<canvasManagers.size() ; i++)
    {
        // After canvases are created : scenes creation
        // DEFAULT SCENES, TO BE CHANGED
        canvasManagers[i]->AddScene("Scène 1 quoi");
        canvasManagers[i]->AddScene("Scène 2 quoi");
        canvasManagers[i]->AddScene("Scène 3 quoi");
    }
    
    
    // Finally, state of the presenter
    setMode(GraphicSessionMode::Loaded);
    // And states of the canvases
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
        canvasManagers[i]->SetMode(CanvasManagerMode::Unselected);
    
    
    // SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
    SetSelectedCanvas(canvasManagers.front());
}

void GraphicSessionManager::CompleteInitialisation(std::shared_ptr<SpatInterpolator<double>> _spatInterpolator)
{
    spatInterpolator = _spatInterpolator;
}

GraphicSessionManager::~GraphicSessionManager()
{
}


// Testing purposes only
void GraphicSessionManager::__LoadDefaultTest()
{
    srand(2016); // GRAINE fixée ici
    
    for(size_t i=0 ; i<canvasManagers.size(); i++)
    {
        for (size_t j=0 ; j < canvasManagers[i]->GetScenesCount() ; j++)
        {
            canvasManagers[i]->SelectScene((int)j);
            
            int areasCount = 3+(rand()%3);
            for (int k=0 ; k<areasCount ; k++)
            {
                // Only polygons added for now
                auto currentEditablePolygon =
                std::make_shared<SpatPolygon>(
                            GetNextAreaId(),
                   bpt(0.2f+0.13f*k,0.3f+0.1f*k), 3+2*k, 0.15f+0.04f*(k+1),
                             Colour(80*(uint8)k, 0, 255));
                // On each canvas
                if (auto editableCanvas = std::dynamic_pointer_cast<MultiSceneCanvasEditor>(canvasManagers[i]))
                    editableCanvas->AddArea(currentEditablePolygon);
                else
                    throw std::runtime_error("Canvas not editable.");
            }
        }
    }
}




// ===== SETTERS AND GETTERS =====
uint64_t GraphicSessionManager::GetNextAreaId()
{
    uint64_t areaIdBackup = nextAreaId;
    nextAreaId++;
    return areaIdBackup;
}
std::shared_ptr<SpatArea> GraphicSessionManager::GetSelectedArea()
{
    if (selectedCanvas)
    {
        if (auto spatArea = std::dynamic_pointer_cast<SpatArea>(getSelectedCanvasAsEditable()->GetSelectedArea()))
            return spatArea;
        else
            throw std::runtime_error("Currently selected area cannot be casted to Miam::ISpatArea");
    }
    else
        return nullptr;
}






// ===== UTILITIES =====



void GraphicSessionManager::SetSelectedCanvas(std::shared_ptr<MultiSceneCanvasInteractor> _selectedCanvas)
{
    
    // We do something only if there has been a change
    if (selectedCanvas != _selectedCanvas)
    {
        // At first : unselection of previous canvas...
        if (selectedCanvas)
        {
            selectedCanvas->SetMode(CanvasManagerMode::Unselected);
            selectedCanvas->CallRepaint();
        }
    
        
        selectedCanvas = _selectedCanvas;
        getSelectedCanvasAsEditable(); // just for the internal test of "editability"
        
        
        selectedCanvas->SetMode(CanvasManagerMode::SceneOnlySelected);
    
        setMode(GraphicSessionMode::CanvasSelected);
    }
    else
    {
        // rien du tout
    }
}

std::shared_ptr<MultiSceneCanvasEditor> GraphicSessionManager::getSelectedCanvasAsEditable()
{
    std::shared_ptr<MultiSceneCanvasEditor> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasEditor>( selectedCanvas);
    if (canvasPtr)
        return canvasPtr;
    else throw std::runtime_error("Canvas not selected, or canvas cannot be casted a Miam::MultiSceneCanvasEditor");
}



/* Regroups all necessary actions on a mode change,
 * moreover : does not always necessarily allow the mode change requested !
 */
void GraphicSessionManager::setMode(GraphicSessionMode newMode)
{
    // OLD mode managing
    switch (mode)
    {
        case GraphicSessionMode::Null :
            break; // direct break, we don't do anything for now
            
        case GraphicSessionMode::Loading :
            // bypass of everything if the session is still loading
            if (newMode != GraphicSessionMode::Loaded)
                return;
            break;
            
        default:
            // Closing of anything pop-up-like displayed on a mode change
            sceneEditionComponent->CloseTemporaryDisplayedObjects();
    }
    
    // NEW mode managing
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
            sceneEditionComponent->SetDeleteSceneButtonEnabled(getSelectedCanvasAsEditable()->GetScenesCount() > 1);
            sceneEditionComponent->SetAreaGroupReduced(true);
            if (areaToCopy)
                sceneEditionComponent->SetPasteEnabled(true);
            sceneEditionComponent->SetSpatGroupReduced(true);
            sceneEditionComponent->SetInitialStateGroupHidden(false);
            sceneEditionComponent->resized(); // right menu update
            
            view->DisplayInfo("Editing a Canvas and its Scenes");
            break;
            
        case GraphicSessionMode::AreaSelected :
            if (GetSelectedArea() == nullptr)
                throw std::runtime_error("No area selected, mode cannot be AreaSelected");
            
            sceneEditionComponent->SetEnabledAllControls(true, true); // as we may come from "waiting for something creation/deletion"
            sceneEditionComponent->SetCanvasGroupHidden(true);
            sceneEditionComponent->SetAreaGroupReduced(false);
            if (areaToCopy)
                sceneEditionComponent->SetPasteEnabled(true);
            sceneEditionComponent->SetSpatGroupReduced(false);
            sceneEditionComponent->SelectSpatState(GetSelectedArea()->GetSpatStateIndex());
            sceneEditionComponent->SetInitialStateGroupHidden(true);
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
        case CanvasManagerMode::SceneOnlySelected :
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





// ===== EVENTS FROM THE PRESENTER ITSELF =====

void GraphicSessionManager::OnEnterSpatScenesEdition()
{
    auto tempStates = spatInterpolator->GetSpatStates();
    sceneEditionComponent->UpdateStatesList(tempStates);
    
    // Forced update of canvases
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
        canvasManagers[i]->OnResized();
}

void GraphicSessionManager::HandleEventSync(std::shared_ptr<GraphicEvent> event_)
{
    
    if (std::shared_ptr<AreaEvent> areaE = std::dynamic_pointer_cast<AreaEvent>(event_))
    {
    }
    
    else if (std::shared_ptr<SceneEvent> sceneE = std::dynamic_pointer_cast<SceneEvent>(event_))
    {
        sceneEditionComponent->SetSceneName(sceneE->GetNewScene()->GetName());
    }
    
}






// ===== EVENTS TO VIEW =====

void GraphicSessionManager::DisplayInfo(String info)
{
    view->DisplayInfo(info);
}






// = = = = = = = = = = Events from View, transmitted the a canvas  = = = = = = = = = =

void GraphicSessionManager::OnAddScene()
{
    if (selectedCanvas)
        selectedCanvas->AddScene("Scene " + std::to_string(selectedCanvas->GetScenesCount()+1));
    else throw std::runtime_error("No canvas selected : cannot add a scene (no canvas should be selected at this point");
}
void GraphicSessionManager::OnDeleteScene()
{
    if (selectedCanvas)
    {
        if(! selectedCanvas->DeleteScene())
            throw std::runtime_error("Cannot delete a scene, only 1 is left (the delete scene button should not have been clicked");
        
    }
    else throw std::runtime_error("No canvas selected : cannot add a scene (no canvas should be selected at this point)");
}
void GraphicSessionManager::OnSceneLeft()
{
    getSelectedCanvasAsEditable()->MoveSelectedSceneTowardsFirst();
}
void GraphicSessionManager::OnSceneRight()
{
    getSelectedCanvasAsEditable()->MoveSelectedSceneTowardsLast();
}
void GraphicSessionManager::OnSceneNameChange(std::string _name)
{
    if (mode != GraphicSessionMode::Loaded && mode != GraphicSessionMode::Loading)
    {
        if (_name.length() > 0) // we do not consider empty strings
            getSelectedCanvasAsEditable()->SetSelectedSceneName(_name);
    }
}



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
void GraphicSessionManager::OnAddArea()
{
    if (selectedCanvas)
    {
        getSelectedCanvasAsEditable()->AddDefaultArea(GetNextAreaId());
        selectedCanvas->CallRepaint();
    }
    else
        throw std::runtime_error("Cannot add a new area : no canvas selected.");
}
void GraphicSessionManager::OnDeleteArea()
{
    if (selectedCanvas)
    {
        getSelectedCanvasAsEditable()->DeleteSelectedArea();
    }
    else
        throw std::runtime_error("No canvas selected. Delete Area button should not be clickable at the moment !");
}

void GraphicSessionManager::OnNewColour(Colour colour)
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnNewColour(colour);
    else throw std::runtime_error("Cannot set a new colour : no canvas selected");
}


void GraphicSessionManager::OnSendToBack()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnSendToBack();
    else throw std::runtime_error("Cannot send something to back : no canvas selected");
}

void GraphicSessionManager::OnSendBackward()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnSendBackward();
    else throw std::runtime_error("Cannot send something backward : no canvas selected");
}
void GraphicSessionManager::OnBringForward()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnBringForward();
    else throw std::runtime_error("Cannot bring something forward : no canvas selected");
}
void GraphicSessionManager::OnBringToFront()
{
    if (selectedCanvas)
        getSelectedCanvasAsEditable()->OnBringToFront();
    else throw std::runtime_error("Cannot bring something to front : no canvas selected");
}



// = = = = = = = = = = Events from View, internally processed  = = = = = = = = = =

void GraphicSessionManager::OnCopyArea()
{
    if(selectedCanvas)
    {
        auto localAreaToCopy = getSelectedCanvasAsEditable()->GetSelectedArea();
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
            std::shared_ptr<IDrawableArea> newDrawbleArea(areaToCopy->Clone());
            std::shared_ptr<IEditableArea> newArea;
            // If cast does not work...
            if (!(newArea = std::dynamic_pointer_cast<IEditableArea>(newDrawbleArea)))
                throw std::runtime_error("Area to copy canot be casted to an editable type");
            
            // Puis : même procédure pour les cas possibles
            // Modification du polygone copié
            newArea->SetId(GetNextAreaId());
            
            // BESOIN DE SAVOIR SI ON CHANGE DE CANVAS OU NON ?
            // Si on change, besoin d'appeler une fonction du genre :
            // RescaleForCanvas(SceneCanvasComponent* )
            //
            // Juste translation par rapport à l'original, dans tous les cas...
            newArea->Translate(Point<double>(20,20));
            getSelectedCanvasAsEditable()->AddArea(newArea);
            getSelectedCanvasAsEditable()->SetSelectedArea(newArea);
            
            // Graphical Update
            selectedCanvas->CallRepaint();
        }
        else
            throw std::runtime_error("No valid area can be pasted from the clipboard.");
    }
    else
        throw std::runtime_error("Cannot paste an area : no canvas selected. Paste button should not be clickable at the moment.");
    
    
}

void GraphicSessionManager::OnSpatStateChanged(int spatStateIdx)
{
    // We just don't process the "void" combo box callback for now
    if (spatStateIdx < 0)
        return;
    if (auto selectedArea = GetSelectedArea())
    {
        // No check on index (view data is supposed to be up-to-date)
        selectedArea->LinkToSpatState(spatInterpolator->GetSpatState((size_t)spatStateIdx));
        // Forced unoptimized update of everything
        getSelectedCanvasAsEditable()->OnResized();
    }
}

