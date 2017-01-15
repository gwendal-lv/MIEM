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



// - - - - - Construction and Destruction (and helpers) - - - - -

EditableScene::EditableScene(MultiSceneCanvasInteractor* canvasManager_, SceneCanvasComponent* canvasComponent_, bool allowAreaSelection_) :
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



void EditableScene::SetSelectedArea(std::shared_ptr<IEditableArea> selectedArea_, bool changeMode)
{
    // Exception throw if the command is not "unselect selected area"
    if (!allowAreaSelection)
    {
        if (selectedArea_ != nullptr)
            throw std::runtime_error("Area selection is not allowed with this EditableScene");
        else
            return;
    }
    
    // Null pointer <=> deselection command
    if (selectedArea_ == nullptr)
    {
        // If one was selected before the deselection
        if (selectedArea)
        {
            selectedArea->SetActive(false);
        }
        
        // In any, we actually deselect
        selectedArea = nullptr;
        if (changeMode)
            canvasManager->SetMode(CanvasManagerMode::NothingSelected);
    }
    
    // Else : we select a new existing EditableArea
    else
    {
        // If one was selected, we deselect it first
        if (selectedArea)
            selectedArea->SetActive(false);
        
        selectedArea = selectedArea_;
        selectedArea->SetActive(true);
        if (changeMode)
            canvasManager->SetMode(CanvasManagerMode::AreaSelected);
        
    }
}




// = = = = = = = = = = METHODS = = = = = = = = = =


// - - - - - Areas Managing : Add and Delete - - - - -



void EditableScene::AddDefaultArea(uint64_t nextAreaId)
{
    // centered grey Hexagon !...
    std::shared_ptr<EditablePolygon> newPolygon(new EditablePolygon(nextAreaId,
                                                                    Point<double>(0.5f,0.5f), 6, 0.15f,
                                                                    Colours::grey,
                                                                    canvasComponent->GetRatio() ));
    
    // Actual adding of this new polygon
    AddArea(newPolygon);
}

void EditableScene::DeleteSelectedArea()
{
    if (selectedArea)
    {
        deleteAreaByUniqueId(selectedArea->GetId());
        SetSelectedArea(nullptr); // actual deletion should happen here
    }
    else throw std::runtime_error("Impossible to delete the selected area (no area selected");
}

void EditableScene::deleteAreaByUniqueId(uint64_t uidToDelete)
{
    bool areaDeleted = false;
    for (auto it = areas.begin() ;
         (!areaDeleted && it!=areas.end()) ; it++)
    {
        if ((uint64_t)((*it)->GetId()) == uidToDelete)
        {
            it = areas.erase(it);
            areaDeleted = true;
        }
    }
}




// ------ areas managing : graphical attributes ------

void EditableScene::ChangeSelectedAreaColour(Colour& colour)
{
    if (selectedArea)
    {
        selectedArea->SetFillColour(colour);
        canvasComponent->repaint();
    }
    else
        std::runtime_error("The given colour cannot be applied : no area is selected");
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
            canvasComponent->repaint();
        }
    }
    else throw std::runtime_error("Cannot send an area to front : no area selected");
}




// - - - - - Canvas (mouse) events managing - - - - -

std::shared_ptr<GraphicEvent> EditableScene::OnCanvasMouseDown(const MouseEvent& mouseE)
{
    // default : empty AREA event (TO DO : events may happen on exciters, etc, etc, ...)
    std::shared_ptr<GraphicEvent> graphicE(new AreaEvent(nullptr, AreaEventType::NothingHappened));
    // float position (more accurate) converted to double
    Point<double> clicLocation = mouseE.position.toDouble();
 
    // If an area can be selected
    if (allowAreaSelection)
    {
        if (mouseE.source.getIndex() == 0)
        {
            // When an area is already selected
            if (canvasManager->GetMode() == CanvasManagerMode::AreaSelected)
            {
                // did we clic next to a point, or at least inside the area ?
				AreaEventType lastEventType = selectedArea->TryBeginPointMove(clicLocation);
                if(lastEventType == AreaEventType::NothingHappened)
                {
                    /* if not, we are sure that we clicked outside (checked by tryBeginPointMove)
                     * => it is a DEselection (maybe selection of another area, just after this)
                     */
					graphicE = std::shared_ptr<GraphicEvent>(new AreaEvent(selectedArea, AreaEventType::Unselected));
						// will signal CanvasManager
						// = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
						// = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
						// THE DIRECT SIGNAL TO CANVAS MANAGER SHOULD STOP
						// ONLY EVENTS SHOULD INFORM PARENT MANAGERS
						// = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
						// = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
                    SetSelectedArea(nullptr); 
                }
            }
            // While no area is selected : we look for a new one to select,
            // starting from the area on the upper layer (last draw on canvas)
            for (int i=(int)(areas.size())-1 ; // int and not the *unsigned* size_t
                 (i>=0 && canvasManager->GetMode()==CanvasManagerMode::NothingSelected) ;
                 i--)
            {
                if (areas[i]->HitTest(clicLocation))
                {
                    // !!!!!!!!!!!!! TEST DES POIDS D'INTERACTION !!!!!!!!!!!
                    //std::cout << "poids d'interaction = " << areasOrderedForDrawing[i]->ComputeInteractionWeight(clicLocation.toDouble()) << std::endl;
                    
                    // On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
                    std::shared_ptr<EditableArea> areaToSelect = std::dynamic_pointer_cast<EditableArea>(areas[i]);
					if (areaToSelect)
					{
						// will signal CanvasManager
						// = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
						// = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
						// THE DIRECT SIGNAL TO CANVAS MANAGER SHOULD STOP
						// ONLY EVENTS SHOULD INFORM PARENT MANAGERS
						// = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
						// = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
						SetSelectedArea(areaToSelect);
						graphicE = std::shared_ptr<GraphicEvent>(new AreaEvent(areaToSelect, AreaEventType::Selected));
					}
                    else
                        throw std::runtime_error(std::string("Cannot cast the area that should be selected to a Miam::EditableArea"));
                }
            }
            // New point creation POLYGONE SEULEMENT, PAS LES AUTRES FORMES !!!!!
            // Le bouton doit d'ailleurs apparaître de manière dynamique....
            if (canvasManager->GetMode() == CanvasManagerMode::WaitingForPointCreation)
            {
                // On lance une EXCEPTION SI LE CAST n'A PAS FONCTIONNÉ
                std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
                if (selectedPolygon)
                {
                    if(! selectedPolygon->TryCreatePoint(clicLocation))
                    {
                        // Here : the point wasn't created
                        std::string returnMsg = "No point created : please click closer to an existing point !";
                        graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, AreaEventType::NothingHappened));
                        graphicE->SetMessage(returnMsg);
                    }
                    else
                    {
                        // Event : shape has changed due to point creation
                        graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, AreaEventType::ShapeChanged));
                    }
                }
                else
                    throw std::runtime_error(std::string("Cannot create a point to the selected area (must be a polygon !)"));
            }
            // Existing point deletion
            else if (canvasManager->GetMode() == CanvasManagerMode::WaitingForPointDeletion)
            {
                // On lancera une exception si le cast n'a pas fonctionné
                std::shared_ptr<EditablePolygon> selectedPolygon = std::dynamic_pointer_cast<EditablePolygon>(selectedArea);
                if (selectedPolygon)
                {
                    std::string returnMsg = (selectedPolygon->TryDeletePoint(clicLocation)).toStdString();
                    // no issue
                    if (returnMsg.empty())
                        graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, AreaEventType::ShapeChanged));
                    // error message contains something
                    else
                    {
                        graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, AreaEventType::NothingHappened));
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
            if (selectedArea)
            {
                // The area will create the event type itself, then we build the AreaEvent
                AreaEventType areaEventType = selectedArea->TryMovePoint(mouseLocation);
                graphicE = std::shared_ptr<AreaEvent>(new AreaEvent(selectedArea, areaEventType));
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
            if (selectedArea)
            {
                selectedArea->EndPointMove();
            }
        }
    }
    
    return graphicE;
}

