/*
  ==============================================================================

    EditableScene.cpp
    Created: 17 Nov 2016 4:54:53pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "EditableScene.h"

#include "EditablePolygon.h"
#include "Exciter.h"

#include "MultiSceneCanvasInteractor.h"

#include "SceneCanvasComponent.h"

#include "MultiAreaEvent.h"


// - - - - - Construction and Destruction (and helpers) - - - - -

EditableScene::EditableScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_, bool allowAreaSelection_) :
    InteractiveScene(canvasManager_, canvasComponent_)
{
    allowAreaSelection = allowAreaSelection_;
}

EditableScene::~EditableScene()
{
    
}




// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

std::shared_ptr<IEditableArea> EditableScene::GetEditableArea(size_t i)
{
    return std::dynamic_pointer_cast<EditableArea>(areas[i]);
}



std::shared_ptr<AreaEvent> EditableScene::SetSelectedArea(std::shared_ptr<IEditableArea> selectedArea_, bool changeMode)
{
    // Default empty area event
    std::shared_ptr<AreaEvent> areaE(new AreaEvent());
    
    // Exception throw if the command is not "unselect selected area"
    if (!allowAreaSelection)
    {
        if (selectedArea_ != nullptr)
            throw std::runtime_error("Area selection is not allowed with this EditableScene");
        else
            return areaE;
    }
    
    // Null pointer <=> deselection command
    if (selectedArea_ == nullptr)
    {
        // If one was selected before the deselection
        if (selectedArea)
        {
            selectedArea->SetActive(false);
            // Area event concerning 1 area only
            areaE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, AreaEventType::Unselected, shared_from_this()));
        }
        // If nothing was selected : event stays empty
        
        // In any case, we actually deselect
        selectedArea = nullptr;
        if (changeMode)
            canvasManager.lock()->SetMode(CanvasManagerMode::SceneOnlySelected);
    }
    
    // Else : we select a new existing EditableArea
    else
    {
        // If one was selected, we deselect it first
        if (selectedArea)
            selectedArea->SetActive(false);
        
        // Actual change
        auto previouslySelectedArea = selectedArea;
        selectedArea = selectedArea_;
        selectedArea->SetActive(true);
        if (changeMode)
            canvasManager.lock()->SetMode(CanvasManagerMode::AreaSelected);
        
        // Creation of the (multi) area event (main for selected, other for unselected)
        if (previouslySelectedArea)
        {
            auto multiAreaE = std::make_shared<MultiAreaEvent>(selectedArea, AreaEventType::Selected, shared_from_this());
            multiAreaE->AddAreaEvent(std::make_shared<AreaEvent>(previouslySelectedArea, AreaEventType::Unselected, shared_from_this()));
            areaE = multiAreaE;
        }
        else
            areaE = std::make_shared<AreaEvent>(selectedArea, AreaEventType::Selected, shared_from_this());
    }
    
    return areaE;
}

int EditableScene::GetSelectedAreaSceneId()
{
    int areaSceneId = -1; // indicates "not found"
    for (int i = 0;
         i < (int)(areas.size()) && areaSceneId == -1 ;
         i++)
    {
        if (selectedArea->GetId() == areas[i]->GetId())
            areaSceneId = i;
    }
    return areaSceneId;
}



// = = = = = = = = = = METHODS = = = = = = = = = =


// - - - - - Areas Managing : Add and Delete - - - - -



std::shared_ptr<AreaEvent> EditableScene::AddDefaultArea(uint64_t nextAreaId)
{
    // centered grey Hexagon !...
    std::shared_ptr<EditablePolygon> newPolygon(new EditablePolygon(nextAreaId,
                                                                    bpt(0.5f,0.5f), 6, 0.15f,
                                                                    Colours::grey,
                                                                    canvasComponent->GetRatio() ));
    
    // Actual adding of this new polygon
    return AddArea(newPolygon);
}

std::shared_ptr<AreaEvent> EditableScene::DeleteSelectedArea()
{
    if (selectedArea)
    {
        auto selectedAreaBackup = selectedArea;
        SetSelectedArea(nullptr);
        return deleteAreaByUniqueId(selectedAreaBackup->GetId());
    }
    else throw std::runtime_error("Impossible to delete the selected area (no area selected");
}

std::shared_ptr<AreaEvent> EditableScene::deleteAreaByUniqueId(uint64_t uidToDelete)
{
    std::shared_ptr<AreaEvent> areaE(new AreaEvent());
    
    for (size_t i = 0 ;
         (areaE->GetType()==AreaEventType::NothingHappened) && (i<areas.size()) ;
         i++)
    {
        if ((uint64_t)(areas[i]->GetId()) == uidToDelete)
        {
            // Will also update the type of the event (to go out of the loop)
            areaE = std::shared_ptr<AreaEvent>(new AreaEvent(areas[i], AreaEventType::Deleted, (int)i, shared_from_this()));
            // Actual deletion, iterator-based
            areas.erase(areas.begin() + i);
        }
    }
    
    return areaE;
}




// ------ areas managing : graphical attributes ------

std::shared_ptr<AreaEvent> EditableScene::ChangeSelectedAreaColour(Colour& colour)
{
    if (selectedArea)
    {
        selectedArea->SetFillColour(colour);
        return std::make_shared<AreaEvent>(selectedArea, AreaEventType::ColorChanged, shared_from_this());
    }
    else
        throw std::runtime_error("The given colour cannot be applied : no area is selected");
}


// - - - - - -  areas managing : Z-Order - - - - - -

void EditableScene::SendSelectedAreaToBack()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            int selectedAreaOrder = -1; // the current drawing order (not known yet)
            
            // Do something if it's not the last yet (last to be drawn)
            if (areas[0] != selectedArea)             {
                // selectedArea becomes the first within the areasOrder vector.
                // at first, we need to find where the area was before
                for (size_t i=0 ; i<areas.size() ; i++)
                {
                    if (areas[i] == selectedArea)
                        selectedAreaOrder = (int)i;
                }
                // then we udpate all what's necessary and we change the value
                // the selected area will not die because it has always the "selectedArea" shared pointer poiting to it !...
                for (int i=selectedAreaOrder ; i >= 1 ; i--)
                    areas[i] = areas[i-1];
                areas[0] = selectedArea;
            }
        }
        DBG("!!!!!!!!! Area modified without sending an Event !!!!!!!!!");
        canvasComponent->repaint();
    }
    else throw std::runtime_error("Cannot send and area to back : no area selected");
}

void EditableScene::SendSelectedAreaBackward()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            if (areas[0] != selectedArea) // and if it's not the last yet
            {
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (size_t i=0 ; i<areas.size() ; i++)
                {
                    if (areas[i] == selectedArea)
                        selectedAreaOrder = (int)i;
                }
                // then we udpate all what's necessary and we change the value
                areas[selectedAreaOrder] = areas[selectedAreaOrder-1];
                areas[selectedAreaOrder-1] = selectedArea;
            }
        }
        DBG("!!!!!!!!! Area modified without sending an Event !!!!!!!!!");
        canvasComponent->repaint();
    }
    else throw std::runtime_error("Cannot send and area forward : no area selected");
}

void EditableScene::BringSelectedAreaForward()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            if (areas.back() != selectedArea) // and if it's not the first yet
            {
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (size_t i=0 ; i<areas.size() ; i++)
                {
                    if (areas[i] == selectedArea)
                        selectedAreaOrder = (int)i;
                }
                // then we udpate all what's necessary and we change the value
                areas[selectedAreaOrder] = areas[selectedAreaOrder+1];
                areas[selectedAreaOrder+1] = selectedArea;
            }
        }
        DBG("!!!!!!!!! Area modified without sending an Event !!!!!!!!!");
        canvasComponent->repaint();
    }
    else throw std::runtime_error("Cannot send and area forward : no area selected");
}

void EditableScene::BringSelectedAreaToFront()
{
    if (selectedArea)
    {
        if (areas.size()>1) // We do something only if more than one area
        {
            if (areas.back() != selectedArea) // and if it's not the first yet
            {
                // selectedArea becomes the last within the areasOrder vector.
                // at first, we need to find where the area was before
                int selectedAreaOrder = -1;
                for (size_t i=0 ; i<areas.size() ; i++)
                {
                    if (areas[i] == selectedArea)
                        selectedAreaOrder = (int)i;
                }
                // then we udpate all what's necessary and we change the value
                for (size_t i=selectedAreaOrder ; i <= areas.size()-2 ; i++)
                    areas[i] = areas[i+1];
                areas.back() = selectedArea;
            }
            DBG("!!!!!!!!! Area modified without sending an Event !!!!!!!!!");
            canvasComponent->repaint();
        }
    }
    else throw std::runtime_error("Cannot send an area to front : no area selected");
}



// - - - - - Selection events managing (orders from parent manager) - - - - -
void EditableScene::OnSelection()
{
}

std::vector<std::shared_ptr<GraphicEvent>> EditableScene::OnUnselection()
{
    // Absolutely needed (vector copy contructor won't be much time-consuming)
    auto returnedVector = InteractiveScene::OnUnselection();
    
    // Own code for Editable features (and possible event getting)
    if (selectedArea)
        returnedVector.push_back(SetSelectedArea(nullptr));
    
    return returnedVector;
}


// - - - - - Canvas (mouse) events managing - - - - -

std::shared_ptr<GraphicEvent> EditableScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
    // default : empty AREA event (TO DO : events may happen on exciters, etc, etc, ...)
    auto graphicE = std::make_shared<GraphicEvent>();
    // float position (more accurate) converted to double
    Point<double> clicLocation = mouseE.position.toDouble();
 
    // If an area can be selected
    if (allowAreaSelection)
    {
        if (mouseE.source.getIndex() == 0)
        {
            // - - - - - When an area is already selected - - - - -
            // - - - - - Check for UNselection - - - - -
            if (selectedArea)
            {
                // did we clic next to a point, or at least inside the area ?
				AreaEventType lastEventType = selectedArea->TryBeginPointMove(clicLocation); // !! starts a point dragging !
                if (lastEventType == AreaEventType::NothingHappened)
                {
                    /* if not, we are sure that we clicked outside (checked by tryBeginPointMove)
                     * => it is a DEselection (maybe selection of another area, just after this)
                     */
                    graphicE = SetSelectedArea(nullptr);
                }
                else // special points which are not point dragging
                {
                    // we must stop the dragging that was not actually wanted
                    if ( canvasManager.lock()->GetMode() == CanvasManagerMode::WaitingForPointCreation
                        || canvasManager.lock()->GetMode() == CanvasManagerMode::WaitingForPointDeletion )
                        selectedArea->EndPointMove();
                }
            }
            
            // - - - - - Then, check for Selection - - - - -
            // While no area is selected : we look for a new one to select,
            // starting from the area on the upper layer (last draw on canvas)
            for (int i=(int)(areas.size())-1 ; // int and not the *unsigned* size_t
                 (i>=0 && (!selectedArea)) ;
                 i--)
            {
                if (areas[i]->HitTest(clicLocation.x,clicLocation.y))
                {
                    // !!!!!!!!!!!!! TEST DES POIDS D'INTERACTION !!!!!!!!!!!
                    //std::cout << "poids d'interaction = " << areasOrderedForDrawing[i]->ComputeInteractionWeight(clicLocation.toDouble()) << std::endl;
                    
                    // On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
                    std::shared_ptr<EditableArea> areaToSelect = std::dynamic_pointer_cast<EditableArea>(areas[i]);
					if (areaToSelect)
					{
                        // Ici il faut regarder si on avait déjà un évènement
                        // parce que ça devient un évènement combiné sélection/déselection
						auto lastAreaE = SetSelectedArea(areaToSelect);
                        if (auto castTry = std::dynamic_pointer_cast<AreaEvent>(graphicE))
                        {
                            std::shared_ptr<MultiAreaEvent> multiAreaE = std::make_shared<MultiAreaEvent>(areaToSelect, AreaEventType::Selected, shared_from_this());
                            multiAreaE->AddAreaEvent(lastAreaE);
                        }
                    }
                    else
                        throw std::runtime_error(std::string("Cannot cast the area that should be selected to a Miam::EditableArea"));
                }
            }
            
            // - - - - - Special behaviors depending on canvas state - - - - -
            
            // New point creation POLYGONE SEULEMENT, PAS LES AUTRES FORMES !!!!!
            // Le bouton doit d'ailleurs apparaître de manière dynamique....
            if (canvasManager.lock()->GetMode() == CanvasManagerMode::WaitingForPointCreation)
            {
                // On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
                std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
                if (selectedPolygon)
                {
                    bypassMouseEventsUntilMouseUp = true; // if successful or not
                    if(! selectedPolygon->TryCreatePoint(clicLocation))
                    {
                        // Here : the point wasn't created
                        std::string returnMsg = "No point created : please click closer to an existing point !";
                        graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, AreaEventType::NothingHappened, shared_from_this()));
                        graphicE->SetMessage(returnMsg);
                    }
                    else
                    {
                        // Event : shape has changed due to point creation
                        graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, AreaEventType::ShapeChanged, shared_from_this()));
                    }
                }
                else
                    throw std::runtime_error(std::string("Cannot create a point to the selected area (must be a polygon !)"));
            }
            // Existing point deletion
            else if (canvasManager.lock()->GetMode() == CanvasManagerMode::WaitingForPointDeletion)
            {
                // On lancera une exception si le cast n'a pas fonctionné
                std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
                if (selectedPolygon)
                {
                    std::string returnMsg = (selectedPolygon->TryDeletePoint(clicLocation)).toStdString();
                    bypassMouseEventsUntilMouseUp = true; // if successful or not
                    // no issue
                    if (returnMsg.empty())
                    {
                        graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, AreaEventType::ShapeChanged, shared_from_this()));
                    }
                    // error message contains something
                    else
                    {
                        graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, AreaEventType::NothingHappened, shared_from_this()));
                        graphicE->SetMessage(returnMsg);
                    }
                }
                else
                    throw std::runtime_error(std::string("Cannot create a point to the selected area (must be a polygon !)"));
            }
        }
    }
    
    
    // Else if any area can be edited anytime
    else // if (!allowAreaSelection)
    {
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // - - - - - - - - - ALERTE GUILLAUME - - - - - - - - -
        // - - - - - - - - - ALERTE GUILLAUME - - - - - - - - -
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // C'est là que tu dois mettre ton code de déplacement/ajout de point, pour
        // n'importe quelle aire
        //
        // Tu peux t'inspirer fortement du code de l' "interactive scene"
		// OU ALORS JE LE FAIS DIRECTEMENT EN PRECISANT SI LA SCENE INTERACTIVE AGIT SUR
		// TOUT = areas + excitateurs, OU JUSTE SUR LES AREAS
		//
		// Je viens de tester, ça marche en multi-touch pour mes excitateurs... Avec 1
		// point de touch par zone par contre (sinon ça fout la merde)
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    }
    
    return graphicE;
}


std::shared_ptr<GraphicEvent> EditableScene::OnCanvasMouseDrag(const MouseEvent& mouseE)
{
    std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent()); // default empty event
    // float position (more accurate) converted to double
    Point<double> mouseLocation = mouseE.position.toDouble();
    
    if (allowAreaSelection)
    {
        if (mouseE.source.getIndex() == 0)
        {
            if (selectedArea && !bypassMouseEventsUntilMouseUp)
            {
                // The area will create the event type itself, then we build the AreaEvent
                AreaEventType areaEventType = selectedArea->TryMovePoint(mouseLocation);
                graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, areaEventType, shared_from_this()));
            }
        }
    }
    
    return graphicE;
}


std::shared_ptr<GraphicEvent> EditableScene::OnCanvasMouseUp(const MouseEvent& mouseE)
{
    std::shared_ptr<GraphicEvent> graphicE(new GraphicEvent()); // default empty event
    Point<float> clicLocation = mouseE.position; // float position (more accurate)
    
    if (allowAreaSelection)
    {
        if (mouseE.source.getIndex() == 0)
        {
            bypassMouseEventsUntilMouseUp = false;
            if (selectedArea)
            {
                graphicE = std::make_shared<AreaEvent>(selectedArea,
                                                       selectedArea->EndPointMove(),
                                                       GetSelectedAreaSceneId(),
                                                       shared_from_this());
            }
        }
    }
    
    return graphicE;
}




