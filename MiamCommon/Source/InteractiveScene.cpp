/*
  ==============================================================================

    InteractiveScene.cpp
    Created: 17 Nov 2016 4:53:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InteractiveScene.h"

#include "DrawableArea.h"

#include "MultiSceneCanvasInteractor.h"
#include "InteractiveArea.h"
#include "Exciter.h"

#include "MultiAreaEvent.h"

#ifdef _MSC_VER
	#include "Windows.h" // for OutputDebugString(), std cout does not work...
#endif

using namespace Miam;


// - - - - - Construction and Destruction (and helpers) - - - - -

InteractiveScene::InteractiveScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_, ExcitersBehaviorType excitersBehavior_)
:
canvasManager(canvasManager_),
canvasComponent(canvasComponent_),
excitersBehavior(excitersBehavior_)
{
    name = "Default Scene";
    
    // DEBUG/test
    // DEBUG/test
    // DEBUG/test
    // DEBUG/test
    // DEBUG/test
    
    
    // DEBUG/test
    // DEBUG/test
    // DEBUG/test
    // DEBUG/test
    // DEBUG/test
}


InteractiveScene::~InteractiveScene()
{
    
}






// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

size_t InteractiveScene::GetDrawableObjectsCount()
{
    return areas.size() + currentExciters.size();
}
std::shared_ptr<IDrawableArea> InteractiveScene::GetDrawableObject(size_t i)
{
    if (i < areas.size())
        return areas[i];
    else
        return currentExciters[i-areas.size()];
}


size_t InteractiveScene::GetInteractiveAreasCount()
{
    return areas.size();
}
std::shared_ptr<IInteractiveArea> InteractiveScene::GetInteractiveArea(size_t i)
{
    return areas[i];
}

std::shared_ptr<MultiAreaEvent> InteractiveScene::setSelectedExciter(std::shared_ptr<Exciter> exciterToSelect)
{
    auto areaE = std::make_shared<MultiAreaEvent>();
    // Désélection si nécessaire
    if (selectedExciter)
    {
        areaE = std::make_shared<MultiAreaEvent>(selectedExciter, AreaEventType::Unselected, shared_from_this());
        selectedExciter->Highlight(false);
        selectedExciter = nullptr;
    }
    // Sélection du nouvel excitateur
    if (exciterToSelect)
    {
        selectedExciter = exciterToSelect;
        selectedExciter->Highlight(true);
        // Si rien ne s'est encore passé : on re-crée un event simple
        if (areaE->GetType() == AreaEventType::NothingHappened)
            areaE = std::make_shared<MultiAreaEvent>(selectedExciter, AreaEventType::Selected, shared_from_this());
        // Sinon, on crée un nouveau multi area event
        else
        {
            auto multiAreaE_temp = std::make_shared<MultiAreaEvent>(selectedExciter, AreaEventType::Selected, shared_from_this());
            multiAreaE_temp->AddAreaEvent(areaE); // areaE en tant que AreaEvent simple : uniquement partie principale sera traitée par la suite
            areaE = multiAreaE_temp; // échange, les shared_ptr gardent les 2 events proprement
        }
    }
    
    return areaE;
}

void InteractiveScene::SetName(std::string _name)
{
    name = _name;
    
    // graphic updates called from parent (all scenes at the same time...)
}

void InteractiveScene::EnableExcitersLowOpacity(bool enable)
{
    for (size_t i=0 ; i < currentExciters.size() ; i++)
        currentExciters[i]->EnableLowOpacityMode(enable);
}
void InteractiveScene::EnableAreasLowOpacity(bool enable)
{
    for (size_t i=0 ; i < areas.size() ; i++)
        areas[i]->EnableLowOpacityMode(enable);
}



// = = = = = = = = = = METHODS = = = = = = = = = =




// - - - - - Areas Managing : Add and Delete - - - - -


std::shared_ptr<AreaEvent> InteractiveScene::AddArea(std::shared_ptr<IInteractiveArea> newArea)
{
    areas.push_back(newArea);
    
    // Forced graphical updates
    newArea->CanvasResized(canvasComponent);
    
    return std::make_shared<AreaEvent>(newArea, AreaEventType::Added, (int)areas.size()-1, shared_from_this());
}



// - - - - - Exciters Managing - - - - -

std::shared_ptr<AreaEvent> InteractiveScene::AddDefaultExciter()
{
    auto canvasManagerLocked = canvasManager.lock();
    if (! canvasManagerLocked)
        throw std::logic_error("Cannot add a new current exciter : cannot get a Unique ID from the canvas manager (not linked to this)");
        
    auto exciter = std::make_shared<Exciter>(canvasManagerLocked->GetNextAreaId(), canvasManagerLocked->GetCommonTimePoint());
        
    return AddExciter(exciter);
}
std::shared_ptr<AreaEvent> InteractiveScene::AddExciter(std::shared_ptr<Exciter> newExciter)
{
    // Configuration particulière des excitateurs
    newExciter->Highlight(false);
    
    currentExciters.push_back(newExciter);
    
    // Forced graphical updates
    newExciter->CanvasResized(canvasComponent);
    
    // WARNING
    // The vector index is the one relative to all drawable objects....
    auto multiAreaE = std::make_shared<MultiAreaEvent>(newExciter, AreaEventType::Added, (int) getExciterDrawingIndex(currentExciters.size()-1), shared_from_this());
    
    // !!! Sélection forcée !!!
    multiAreaE->AddAreaEvent( setSelectedExciter(newExciter) );
    
    return multiAreaE;
}

std::shared_ptr<AreaEvent> InteractiveScene::DeleteCurrentExciterByIndex(size_t excitersVectorIndex)
{
    auto deletedExciter = currentExciters[excitersVectorIndex];
    currentExciters.erase(currentExciters.begin() + excitersVectorIndex);
    return std::make_shared<AreaEvent>(deletedExciter, AreaEventType::Deleted, shared_from_this());
}
std::shared_ptr<AreaEvent> InteractiveScene::DeleteSelectedExciter()
{
    auto areaE = std::make_shared<AreaEvent>();
    
    // Recherche inverse de l'index, et dé-sélection et suppression directement
    for (size_t i=0 ; i<currentExciters.size() && selectedExciter ; i++)
    {
        if (currentExciters[i] == selectedExciter)
        {
            std::shared_ptr<MultiAreaEvent> multiAreaE_temp = setSelectedExciter(nullptr);
            multiAreaE_temp->AddAreaEvent( DeleteCurrentExciterByIndex(i) );
            areaE = multiAreaE_temp;
        }
    }
            
    return areaE;
}

std::shared_ptr<MultiAreaEvent> InteractiveScene::ResetCurrentExcitersToInitialExciters()
{
    auto multiAreaE = std::make_shared<MultiAreaEvent>();
    // Création des évènement de suppression des excitateurs courants
    // qui vont être supprimés
    if (currentExciters.size() >= 1)
    {
        // First exciter : creation of the multiareaE (and of the main areaE)
        auto firstAreaDeletedE = DeleteCurrentExciterByIndex(0);
        multiAreaE = std::make_shared<MultiAreaEvent>( firstAreaDeletedE.get() );
        
        // Autres excitateurs à supprimer
        for (size_t i = 1 ; i<currentExciters.size() ; i++)
            multiAreaE->AddAreaEvent(DeleteCurrentExciterByIndex(i));
    }
    
    // Duplication des excitateurs initiaux
    for (size_t i = 0 ; i<initialExciters.size() ; i++)
    {
        Exciter* clonedExciterPtr = dynamic_cast<Exciter*> (currentExciters[i]->Clone());
        if (! clonedExciterPtr)
            throw std::logic_error("Cloned exciter cannot be dynamically casted to a Miam::Exciter...");
        
        auto exciter = std::shared_ptr<Exciter>(clonedExciterPtr);
        
        // Ajout propre du nouvel élément
        auto areaAddedE = AddExciter(exciter);
        
        // Puis gestion de l'évènement
        // Comportement spécial pour le premier... On doit être sûr d'avoir bien déjà
        // bourré un event principal (fait à la construction) dans le multiareaE.
        // À partir de i==1 il n'y a plus de doute, l'event principal est déjà créé
        if (i==0 && multiAreaE->GetType()==AreaEventType::NothingHappened)
            multiAreaE = std::make_shared<MultiAreaEvent>( areaAddedE.get() );
        else
            multiAreaE->AddAreaEvent(areaAddedE);
    }
    
    return multiAreaE;
}

void InteractiveScene::SaveCurrentExcitersToInitialExciters()
{
    // On veut créer une sauvegarde à un instant figé dans le temps
    // donc on fait une copie des excitateurs, et pas seulement
    // des pointeurs sur les excitateurs
    initialExciters.clear();
    for (size_t i = 0 ; i<currentExciters.size() ; i++)
    {
        Exciter* clonedExciterPtr = dynamic_cast<Exciter*> (currentExciters[i]->Clone());
        if (! clonedExciterPtr)
            throw std::logic_error("Cloned exciter cannot be dynamically casted to a Miam::Exciter...");
        
        auto exciter = std::shared_ptr<Exciter>(clonedExciterPtr);
        initialExciters.push_back( exciter );
    }
    
    // Aucun évènement renvoyé

}


// - - - - - Selection events managing (orders from parent manager) - - - - -
void InteractiveScene::OnSelection()
{
}
std::vector<std::shared_ptr<GraphicEvent>> InteractiveScene::OnUnselection()
{
    return StopCurrentTransformations();
}



// - - - - - Canvas (mouse) events managing - - - - -

std::shared_ptr<GraphicEvent> InteractiveScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
	// default : empty AREA event
    auto graphicE = std::make_shared<AreaEvent>();

    switch(excitersBehavior)
    {
        case ExcitersBehaviorType::AppearOnTouch :
            throw std::runtime_error("Exciter appearing on touch not implemented");
            break;
            
        case ExcitersBehaviorType::ManualAddAndDelete :
        case ExcitersBehaviorType::ManualAddAndDelete_AutoDisappear :
        {
            // Security : if the touch source ID already exists... What do we do ?
            
            // In this case, exciters already exist. We try to move one that is not
            // already linked to a touch source

			// While no exciter is selected : we look for a new one to select,
			// starting from the area on the upper layer (last draw on canvas)
            auto eventType = AreaEventType::NothingHappened;
            for (int i=(int)currentExciters.size()-1; (i>=0)&&(eventType != AreaEventType::PointDragBegins) ; i--)
            {
                // Function will check first if the area is not already dragged
                eventType = currentExciters[i]->TryBeginPointMove(mouseE.position.toDouble());
                if (eventType == AreaEventType::PointDragBegins)
                {
                    // Indicates the the move can begin
                    touchSourceToEditableArea[mouseE.source.getIndex()] = currentExciters[i];
					graphicE = std::make_shared<AreaEvent>(currentExciters[i], eventType, shared_from_this());
                    
                    // Le dernier excitateur choisi devient l'excitateur sélectionné !
                    auto multiAreaE_temp = setSelectedExciter(currentExciters[i]);
                    multiAreaE_temp->AddAreaEvent(graphicE);
                    graphicE = multiAreaE_temp;
                }
            }
            
            // Si rien n'a été sélectionné à cet endroit : dé-sélecion de l'excitateur
            if (eventType == AreaEventType::NothingHappened)
                graphicE = setSelectedExciter(nullptr);
            
            break;
        }
            
        default :
            break;
    }
    
	return graphicE;
}
std::shared_ptr<GraphicEvent> InteractiveScene::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
    // Default empty event...
    auto graphicE = std::make_shared<GraphicEvent>();
    
    int touchIndex = mouseE.source.getIndex();
    auto mapIt = touchSourceToEditableArea.find(touchIndex);
    // If the touch is related to an area being moved
    if (mapIt != touchSourceToEditableArea.end())
    {
        AreaEventType eventType = mapIt->second->TryMovePoint(mouseE.position.toDouble());
        graphicE = std::make_shared<AreaEvent>(mapIt->second, eventType, shared_from_this());
    }
    
    return graphicE;
}
std::shared_ptr<GraphicEvent> InteractiveScene::OnCanvasMouseUp(const MouseEvent& mouseE)
{
	std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent());

    switch(excitersBehavior)
    {
        case ExcitersBehaviorType::AppearOnTouch :
            throw std::runtime_error("Exciter appearing on touch not implemented");
            break;
            
        case ExcitersBehaviorType::ManualAddAndDelete :
        case ExcitersBehaviorType::ManualAddAndDelete_AutoDisappear :
        // We end the move, if the touch was actually related to an area
        {
            int touchIndex = mouseE.source.getIndex();
            // May not always exist (suddens touch state reinit on scene change, ...)
            auto mapIt = touchSourceToEditableArea.find(touchIndex);
            if (mapIt != touchSourceToEditableArea.end())
            {
                mapIt->second->EndPointMove();
                touchSourceToEditableArea.erase(mapIt);
				// no event : exciter remains alive
				//graphicE = 
            }
            break;
        }
            
        default :
            break;
    }
    return graphicE;
}

std::vector<std::shared_ptr<GraphicEvent>> InteractiveScene::StopCurrentTransformations()
{
    std::vector<std::shared_ptr<GraphicEvent>> areaEvents;
    
    // We stop all current movements
    // and filter all future undesired touch events
    for (auto it = touchSourceToEditableArea.begin();
         it != touchSourceToEditableArea.end();)
    {
        // filtering at first,
        std::shared_ptr<IEditableArea> editableArea = it->second;
        it = touchSourceToEditableArea.erase(it); // increments to next valid
        // Actual stop after
        AreaEventType eventType = editableArea->EndPointMove();
        
        // Storage of event in vector (for events back sending)
        auto areaE = std::make_shared<AreaEvent>(editableArea, eventType, shared_from_this());
        areaEvents.push_back(areaE);
    }
    
    return areaEvents;
}



// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> InteractiveScene::GetTree()
{
    auto sceneTree = std::make_shared<bptree::ptree>();
    sceneTree->put("name", name);
    bptree::ptree areasTree;
    
    // Ajout des aires interactives
    for (size_t i=0; i < areas.size() ; i++)
    {
        auto areaTree = areas[i]->GetTree();
        areaTree->put("<xmlattr>.index", i);
        areasTree.add_child("area", *areaTree);
    }
    // Ajout des excitateur : on prend les initiaux, pas les actifs ! Choix discutable...
    for (size_t i=0; i < initialExciters.size() ; i++)
    {
        auto areaTree = initialExciters[i]->GetTree();
        areaTree->put("<xmlattr>.index", getExciterDrawingIndex(i));
        areasTree.add_child("area", *areaTree);
    }
    
    sceneTree->add_child("areas", areasTree);
    return sceneTree;
}




