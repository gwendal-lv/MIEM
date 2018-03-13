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
        // On ne change l'opacité que s'il n'était pas libre
        selectedExciter->SetOpacityMode(OpacityMode::Independent);
        selectedExciter = nullptr;
    }
    // Sélection du nouvel excitateur
    if (exciterToSelect)
    {
        selectedExciter = exciterToSelect;
        selectedExciter->SetOpacityMode(OpacityMode::High);
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

void InteractiveScene::SetExcitersOpacityMode(OpacityMode opacityMode)
{
    for (size_t i=0 ; i < currentExciters.size() ; i++)
        currentExciters[i]->SetOpacityMode(opacityMode);
}
void InteractiveScene::SetAreasOpacityMode(OpacityMode opacityMode)
{
    for (size_t i=0 ; i < areas.size() ; i++)
        areas[i]->SetOpacityMode(opacityMode);
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
        
    auto exciter = std::make_shared<Exciter>(canvasManagerLocked->GetNextAreaId(),
                                             canvasManagerLocked->GetCommonTimePoint(),
                                             Exciter::AdditionnalGrabRadius::Medium);
    return AddExciter(exciter);
}
std::shared_ptr<AreaEvent> InteractiveScene::AddExciter(std::shared_ptr<Exciter> newExciter, bool forceSelect)
{
    if (auto canvasManagerLocked = canvasManager.lock())
        newExciter->SetInterpolationType(canvasManagerLocked->GetInterpolatorType());
    else
        assert(0); // on devrait toujours pouvoir acquérir un canvasmanager à ce stade...
    currentExciters.push_back(newExciter);
    
    // Forced graphical updates
    newExciter->CanvasResized(canvasComponent);
    
    // WARNING
    // The vector index is the one relative to all drawable objects....
    auto multiAreaE = std::make_shared<MultiAreaEvent>(newExciter, AreaEventType::Added, (int) getExciterDrawingIndex(currentExciters.size()-1), shared_from_this());
    
    // !!! Sélection forcée, si demandée seulement
    // Demandée automatiquement via paramètre par défaut
    if (forceSelect)
        multiAreaE->AddAreaEvent( setSelectedExciter(newExciter) );
    
    return multiAreaE;
}

std::shared_ptr<AreaEvent> InteractiveScene::DeleteCurrentExciterByIndex(size_t excitersVectorIndex)
{
    auto deletedExciter = currentExciters[excitersVectorIndex];
    
    // petite optimisation : on utilise pop_back sur le vector (censé pas trop le modifier...) si
    // on demande d'enlever le dernier élément
    if ( excitersVectorIndex == currentExciters.size() - 1)
        currentExciters.pop_back();
    // Sinon si c'est un élément au milieu on fait sans rechigner... tout petit vecteur en vrai
    else
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
    auto multiAreaE = setSelectedExciter(nullptr);
    
    // Création des évènement de suppression des excitateurs courants
    // qui vont être supprimés
    while ( currentExciters.size() > 0 )
    {
        multiAreaE->AddAreaEvent(DeleteCurrentExciterByIndex(currentExciters.size()-1));
    }
    // Duplication des excitateurs initiaux
    for (size_t i = 0 ; i<initialExciters.size() ; i++)
    {
        // Et donc : le volume est cloné lui aussi
        std::shared_ptr<IDrawableArea> clonedCopy = initialExciters[i]->Clone();
        std::shared_ptr<Exciter> exciter;
        if ( ! (exciter = std::dynamic_pointer_cast<Exciter>(clonedCopy)) )
            throw std::logic_error("Cloned exciter cannot be dynamically casted to a Miam::Exciter...");
        
        // Ajout propre du nouvel élément
        auto canvasManagerLocked = canvasManager.lock();
        if (! canvasManagerLocked)
            throw std::logic_error("Cannot add a new current exciter : cannot get a Unique ID from the canvas manager (not linked to this)");
        exciter->SetId(canvasManagerLocked->GetNextAreaId());
        auto areaAddedE = AddExciter(exciter);
        
        // Puis gestion de l'évènement : dans tous les cas on ajoute
        // (peu importe que l'évènement principal du multievent soit NothingHappened, ou non
        multiAreaE->AddAreaEvent(areaAddedE);
    }

    return multiAreaE;
}

void InteractiveScene::SaveCurrentExcitersToInitialExciters(bool deleteCurrentExciters)
{
    // On veut créer une sauvegarde à un instant figé dans le temps
    // donc on fait une copie des excitateurs, et pas seulement
    // des pointeurs sur les excitateurs
    initialExciters.clear();
    for (size_t i = 0 ; i<currentExciters.size() ; i++)
    {
        std::shared_ptr<IDrawableArea> clonedCopy = currentExciters[i]->Clone();
        std::shared_ptr<Exciter> exciter;
        if (! (exciter = std::dynamic_pointer_cast<Exciter>(clonedCopy)) )
            throw std::logic_error("Cloned exciter cannot be dynamically casted to a Miam::Exciter...");
        
        initialExciters.push_back( exciter );
    }
    
    // Sans notification....
    if (deleteCurrentExciters)
        currentExciters.clear();
    
    // Aucun évènement renvoyé
}


// - - - - - Selection events managing (orders from parent manager) - - - - -
std::shared_ptr<MultiAreaEvent> InteractiveScene::OnSelection(bool resetExciters)
{
    auto multiAreaE = std::make_shared<MultiAreaEvent>();
    
    if (resetExciters)
        multiAreaE = ResetCurrentExcitersToInitialExciters();

    // On actualise l'influence des excitateurs, dans TOUS les modes de jeu !
    //if (canvasManager.lock()->GetMode() == CanvasManagerMode::PlayingWithExciters)
    {
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        // à l'avenir : transitions douces par Timers !!
        for (auto& exciter : currentExciters)
            exciter->SetVolume(1.0);
        
        // Pour l'instant tout dans une fonction bête et méchante, séparée
        // On remplace bien sauvagement les évènements créés précédemmet !
        // L'idée c'est qu'on oublie les events graphiques à la transition entre scènes...
        multiAreaE = RecomputeAreaExciterInteractions();
    }
    
    return multiAreaE;
}
std::shared_ptr<MultiAreaEvent> InteractiveScene::OnUnselection(bool shutExcitersDown)
{
    // D'abord on arrête les manipulations graphiques
    auto multiAreaE = StopCurrentTransformations();
    
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs
    // Puis on éteint les excitateurs (EXCITATION AVEC VOLUME)
    if (shutExcitersDown)
    {
        for (size_t i=0 ; i<currentExciters.size() ; i++)
            currentExciters[i]->SetVolume(0.0);
        multiAreaE->AddAreaEvent(RecomputeAreaExciterInteractions());
    }
    
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    // à l'avenir : transitions douces par Timers !!
    
    return multiAreaE;
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
        auto exciter = std::dynamic_pointer_cast<Exciter>(mapIt->second);
        if (! exciter)
            throw std::logic_error("An interactive scene can handle Miam::Exciters only");
        
        // Création de l'évènement de l'excitateur seul, pour renvoi (dans un multi event, dans le doute)
        AreaEventType eventType = exciter->TryMovePoint(mouseE.position.toDouble());
        auto simpleAreaE = std::make_shared<AreaEvent>(exciter, eventType, shared_from_this());
        
        // Test sur toutes les aires
        auto multiAreaE = testAreasInteractionsWithExciter(exciter);
        multiAreaE->AddAreaEvent(simpleAreaE); // excitateur lui-même

        graphicE = multiAreaE;
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

std::shared_ptr<MultiAreaEvent> InteractiveScene::StopCurrentTransformations()
{
    auto multiAreaE = std::make_shared<MultiAreaEvent>();
    
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
        multiAreaE->AddAreaEvent(new AreaEvent(editableArea, eventType, shared_from_this()));
    }
    return multiAreaE;
}




// = = = = = = = = = = Quantification, gestion des interactions = = = = = = = = = =

std::shared_ptr<MultiAreaEvent> InteractiveScene::testAreasInteractionsWithExciter(std::shared_ptr<Exciter>& exciter)
{
    auto multiAreaE = std::make_shared<MultiAreaEvent>(); // event de base restera "NothingHappened"...
    
    // PASSE 1
    for (size_t i = 0 ; i < areas.size() ; i++)
    {
        // Si qqchose a changé, on renvoit l'info
        auto areaE = areas[i]->UpdateInteraction(exciter);
        if ( areaE->GetType() == AreaEventType::ExcitementAmountChanged )
        {
            areaE->SetConcernedScene(shared_from_this()); // pas précisé dans l'aire, qui ne connaît pas son parent.
            multiAreaE->AddAreaEvent( areaE );
        }
    }
    // PASSE 2 (à l'intérieur de l'excitateur, car il enregistre les modifs !)
    exciter->NotifyNewExcitationToAreas();
    
    return multiAreaE;
}

std::shared_ptr<MultiAreaEvent> InteractiveScene::RecomputeAreaExciterInteractions()
{
    auto multiAreaE = std::make_shared<MultiAreaEvent>();
    
    for (size_t i=0 ; i<currentExciters.size() ; i++)
        testAreasInteractionsWithExciter(currentExciters[i]);
    
    // Plutôt que de renvoyer des dizaines d'évènements....
    // On recrée juste un évènement pour chaque aire
    for (size_t i=0 ; i<areas.size() ; i++)
        multiAreaE->AddAreaEvent(new AreaEvent(areas[i], AreaEventType::ExcitementAmountChanged, shared_from_this()));
    
    return multiAreaE;
}




// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> InteractiveScene::GetTree() const
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
    // Ajout des excitateurs : on prend les initiaux, pas les actifs ! Choix discutable...
    for (size_t i=0; i < initialExciters.size() ; i++)
    {
        auto areaTree = initialExciters[i]->GetTree();
        areaTree->put("<xmlattr>.index", getExciterDrawingIndex(i));
        areasTree.add_child("area", *areaTree);
    }
    
    sceneTree->add_child("areas", areasTree);
    return sceneTree;
}




