/*
  ==============================================================================

    SpatStatesEditionManager.cpp
    Created: 26 Nov 2016 6:47:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatStatesEditionManager.h"

#include "View.h"
#include "SpatStatesEditionComponent.h"



using namespace Miam;

SpatStatesEditionManager::SpatStatesEditionManager(View* _view)
{
    view = _view;
    
    editionComponent = view->GetMainContentComponent()->GetSpatStatesEditionComponent();
    editionComponent->CompleteInitialization(this);
}

void SpatStatesEditionManager::CompleteInitialisation(std::shared_ptr<SpatInterpolator<double>> _spatInterpolator)
{
    spatInterpolator = _spatInterpolator;
    
    // Update of the list on the GUI side
    updateView();
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =


// = = = = = = = = = = EVENTS from PRESENTER = = = = = = = = = =

void SpatStatesEditionManager::OnLeaveSpatStatesEdition()
{
    sendDataToModel(editionComponent->GetDisplayedSpatMatrix());
    // Update now to the editionComponent
    selectSpatState(selectedSpatState);
}


// = = = = = = = = = = EVENTS from VIEW = = = = = = = = = =

void SpatStatesEditionManager::OnSpatStateSelectedById(std::shared_ptr<SpatMatrix> currentMatrix, int _spatStateId)
{
    // Matrix save within Model if necessary
    sendDataToModel(currentMatrix);
    
    // Check for the information (not updated info might come the GUI, we never know...)
    if (_spatStateId == -1)
        selectSpatState(nullptr);
    else if (0 <= _spatStateId && _spatStateId < spatInterpolator->GetSpatStatesCount())
        selectSpatState(spatInterpolator->GetSpatState(_spatStateId));
    
    // This shouldn't happen, so we throw an exception (for debug only ?)
    else
        throw std::out_of_range("Spat State Id does not exist within the Model");
}
void SpatStatesEditionManager::OnRenameState(std::string newName, int stateIndex)
{
    // Actual renaming
    spatInterpolator->GetSpatState(stateIndex)->SetName(newName);
    
    // Total list update + Selection of the state that has just been renamed
    updateView();
    selectSpatState(spatInterpolator->GetSpatState(stateIndex));
}
void SpatStatesEditionManager::selectSpatState(std::shared_ptr<SpatState<double>> _spatState)
{
    // Internal update at first
    selectedSpatState = _spatState;
    
    // Graphical updates : info label (links count)
    std::string infoText;
    std::shared_ptr<SpatMatrix> matrixToSend = std::make_shared<SpatMatrix>(); // initially full of zeros
    if (selectedSpatState)
    {
        infoText = "Linked to "
        + std::to_string(selectedSpatState->GetLinkedAreasCount())
        + " area" + (selectedSpatState->GetLinkedAreasCount()>1 ? "s" : "");
        
        // At last : routing matrix (only choice available for now...)
        if (std::shared_ptr<MatrixState<double>> matrixState = std::dynamic_pointer_cast<MatrixState<double>>(selectedSpatState) )
            matrixToSend = matrixState->GetMatrix();
        // else if the cast did not work
        else
            throw std::runtime_error("Spat state is not a Matrix state");
    }
    else // if no state selected
        infoText = "-";
    // Update command
    editionComponent->SelectAndUpdateState(selectedSpatState->GetIndex(), infoText, matrixToSend);
}


size_t SpatStatesEditionManager::GetFadersCount()
{
    return selectedSpatState->GetOutputsCount();
}



// = = = = = = = = = = GRAPHICAL HELPERS = = = = = = = = = =

void SpatStatesEditionManager::updateView()
{
    // GUI update (copy of whole vector)
    std::vector<std::shared_ptr<SpatState<double>>> newSpatStates = spatInterpolator->GetSpatStates();
    editionComponent->UpdateStatesList(newSpatStates);
}



// = = = = = = = = = = INTERNAL HELPERS = = = = = = = = = =

void SpatStatesEditionManager::sendDataToModel(std::shared_ptr<SpatMatrix> currentMatrix)
{
    if (selectedSpatState) // if exists
    {
        if (std::shared_ptr<MatrixState<double>> matrixState = std::dynamic_pointer_cast<MatrixState<double>>(selectedSpatState) )
            matrixState->SetMatrix(currentMatrix);
        else
            throw std::logic_error("State is not a Matrix (behavior not implemented");
    }
}


// = = = = = = = = = = SETTINGS MANAGEMENT = = = = = = = = = =
void SpatStatesEditionManager::AllowKeyboardEdition(bool allow)
{
    editionComponent->AllowKeyboardEdition(allow);
}

