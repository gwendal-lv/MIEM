/*
  ==============================================================================

    GraphicSessionManager.cpp
    Created: 28 Mar 2016 5:27:18pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "GraphicSessionManager.h"
#include "Model.h"

// Other includes

#include "View.h"

using namespace Miam;


// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(View* _view) :
    view(_view)
{

    
    // ICI ON CHARGE DES TRUCS
    // Canvases const count defined here
    // 1 SEUL CANVAS PAR EXEMPLE
    // On doit créer les sous-objets graphiques de canevas (View) avant de
    // les transmettre au sous-module de gestion de canevas (Presenter) que l'on crée
    // d'ailleurs ici aussi.
    canvasManagers.push_back(new AmusingMultiSceneCanvasInteractor(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
	//canvasManagers.push_back(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
	/*
    canvasManagers.push_back(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(),  SceneCanvasComponent::Id::Canvas2));
     */
    
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
    {
        // After canvases are created : scenes creation
        // DEFAULT SCENES, TO BE CHANGED
        canvasManagers[i]->AddScene("Scène 1, pour le plaisir");
        canvasManagers[i]->AddScene("Scène 2 oh ouuiiii");
        canvasManagers[i]->AddScene("Scène jamais 2 sans 3");
    }
    
    // Links to the view module
    view->CompleteInitialization(this, multiCanvasComponent);
    
    
    // And states of the canvases
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
        canvasManagers[i]->SetMode(CanvasManagerMode::Unselected);
    
    
    // SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
    nextAreaId = 0; // plus tard : valeur contenue dans le fichier de sauvegarde
    SetSelectedCanvas(canvasManagers.front());
    
    
}

GraphicSessionManager::~GraphicSessionManager()
{
}

void GraphicSessionManager::CompleteInitialization(Model* _model)
{
	model = _model;
}

// Testing purposes only
void GraphicSessionManager::__LoadDefaultTest()
{
    srand(2016); // GRAINE fixée ici
    
    
    for(int i=0 ; i<canvasManagers.size(); i++)
        canvasManagers[i]->__AddTestAreas();
}




// ===== SETTERS AND GETTERS =====

std::shared_ptr<IEditableArea> GraphicSessionManager::GetSelectedArea()
{
    if (selectedCanvas)
        return getSelectedCanvasAsEditable()->GetSelectedArea();
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
        {
            selectedCanvas->SetMode(CanvasManagerMode::Unselected);
            selectedCanvas->CallRepaint();
        }
    
        /*
        selectedCanvas = dynamic_cast<MultiSceneCanvasEditor*>(_selectedCanvas);
        if (!selectedCanvas)
            throw std::runtime_error(std::string("The canvas to be selected is only an Interactor, and not an Editor (no editing features...)"));
        */
        // Pas besoin du cast en canvas "éditeur", pour le Miam Spat un simple
        // "interacteur" va suffire
        selectedCanvas = _selectedCanvas;
        
        selectedCanvas->SetMode(CanvasManagerMode::NothingSelected);
    
        //setMode(GraphicSessionMode::CanvasSelected);
        
        multiCanvasComponent->resized();
    }
    else
    {
        // rien du tout
    }
}

MultiSceneCanvasEditor* GraphicSessionManager::getSelectedCanvasAsEditable()
{
    MultiSceneCanvasEditor* canvasPtr = dynamic_cast<MultiSceneCanvasEditor*>( selectedCanvas);
    if (canvasPtr)
        return canvasPtr;
    else throw std::runtime_error("Canvas cannot be casted a Miam::MultiSceneCanvasEditor");
}





void GraphicSessionManager::CanvasModeChanged(CanvasManagerMode canvasMode)
{
    
}





// ===== EVENTS FROM THE PRESENTER ITSELF =====
void GraphicSessionManager::OnSceneChange(std::shared_ptr<EditableScene> newSelectedScene)
{
    // À implémenter
}






// ===== EVENTS TO VIEW =====

void GraphicSessionManager::DisplayInfo(String info)
{
    view->DisplayInfo(info);
}


void GraphicSessionManager::OnSurfaceChanged(double newSurface)
{
	DBG("!!!!!!!! GraphicSessionManager::OnSurfaceChanged");
	// transforme l'aire en frequence avant de l'envoyer vers le model
	if(model->audioPlayer->sinus == nullptr)
		model->audioPlayer->addSource();
	model->audioPlayer->setFrequency(0,440);
	model->audioPlayer->setAmplitude(0, 1.25);
}


void GraphicSessionManager::OnAreaCreated(String sceneName, int areaId)
{
	model->audioPlayer->addSource(sceneName,areaId);
}

