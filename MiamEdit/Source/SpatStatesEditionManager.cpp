/*
  ==============================================================================

    SpatStatesEditionManager.cpp
    Created: 26 Nov 2016 6:47:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <sstream> // std::ostringstream

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

void SpatStatesEditionManager::CompleteInitialisation(std::shared_ptr<StatesInterpolator<double>> _statesInterpolator)
{
    spatInterpolator = _statesInterpolator;
    
    // Update of the list on the GUI side
    selectSpatState(nullptr);
    UpdateView();
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
AppPurpose SpatStatesEditionManager::GetSessionPurpose()
{
    return view->GetSessionPurpose();
}

void SpatStatesEditionManager::selectSpatState(std::shared_ptr<ControlState<double>> _spatState)
{
    // Internal update at first
    selectedSpatState = _spatState;
    
    // Graphical updates : info label (links count)
    std::string infoText;
    std::shared_ptr<ControlMatrix> matrixToSend = std::make_shared<ControlMatrix>(); // initially full of zeros
    int stateIndexToSend = -1;
    if (selectedSpatState)
    {
        infoText = "Linked to "
        + boost::lexical_cast<std::string>(selectedSpatState->GetLinkedAreasCount())
        + " area" + (selectedSpatState->GetLinkedAreasCount()>1 ? "s" : "");
        stateIndexToSend = selectedSpatState->GetIndex();
        
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
    editionComponent->SelectAndUpdateState(stateIndexToSend, infoText, matrixToSend);
}


size_t SpatStatesEditionManager::GetFadersCount()
{
    return selectedSpatState->GetOutputsCount();
}


// = = = = = = = = = = EVENTS from PRESENTER = = = = = = = = = =
void SpatStatesEditionManager::OnEnterSpatStatesEdition()
{
    // Forces graphical updates
    selectSpatState(selectedSpatState);
}
std::shared_ptr<bptree::ptree> SpatStatesEditionManager::OnLeaveSpatStatesEdition()
{
    // Transfert des données depuis l'affichage graphique vers le modèle
    sendDataToModel(editionComponent->GetDisplayedSpatMatrix());
    editionComponent->GetLabelledMatrix()->GetChannelsName();
    
    // Update now to the editionComponent
    selectSpatState(selectedSpatState);
    
    // Save to XML (Presenter does it)
    return GetTree();
}
void SpatStatesEditionManager::OnInOutNamesDisplayedChanged(bool areInputNamesVisible, bool areOutputNamesVisible)
{
    editionComponent->SetInOutNamesDisplayed(areInputNamesVisible, areOutputNamesVisible);
}

// = = = = = = = = = = EVENTS from VIEW = = = = = = = = = =

void SpatStatesEditionManager::OnSpatStateSelectedById(std::shared_ptr<ControlMatrix> currentMatrix, int _spatStateId)
{
    // Matrix save within Model if necessary
    sendDataToModel(currentMatrix);
    
    // Check for the information (not updated info might come the GUI, we never know...)
    if (_spatStateId == -1)
        selectSpatState(nullptr);
    else if (0 <= _spatStateId && _spatStateId < spatInterpolator->GetStatesCount())
        selectSpatState(spatInterpolator->GetState(_spatStateId));
    
    // This shouldn't happen, so we throw an exception (for debug only ?)
    else
        throw std::out_of_range("State Id does not exist within the Model");
}
void SpatStatesEditionManager::OnRenameState(std::string newName, int stateIndex)
{
    // Sauvegarde avant tout
    sendDataToModel(editionComponent->GetDisplayedSpatMatrix());
    
    // Actual renaming
    spatInterpolator->GetState(stateIndex)->SetName(newName);
    
    // Total list update + Selection of the state that has just been renamed
    UpdateView();
    selectSpatState(spatInterpolator->GetState(stateIndex));
}


void SpatStatesEditionManager::OnAddState()
{
    // Actualisation depuis l'affichage graphique
    sendDataToModel(editionComponent->GetDisplayedSpatMatrix());
    
    // Puis addition
    auto newState = spatInterpolator->AddDefaultState();
    UpdateView();
    selectSpatState(newState);
}
void SpatStatesEditionManager::OnDeleteSelectedState()
{
    if (selectedSpatState)
    {
        // Model command transmission at first
        size_t spatStateIndexBackup = (size_t)selectedSpatState->GetIndex();
        spatInterpolator->DeleteState(selectedSpatState);
        // Display updates
        UpdateView();
        if (spatStateIndexBackup == 0)
        {
            // if it was the last one, nothing selected
            if (spatInterpolator->GetStatesCount() == 0)
                selectSpatState(nullptr);
            else // else, the next one is selected
                selectSpatState(spatInterpolator->GetState(spatStateIndexBackup));
        }
        else // selection of the previous
            selectSpatState(spatInterpolator->GetState(spatStateIndexBackup-1));
    }
    else
        throw std::logic_error("Cannot delete state: no state is currently selected.");
}
void SpatStatesEditionManager::OnMoveSelectedStateUp()
{
    if (selectedSpatState
        && spatInterpolator->GetStatesCount() >= 2
        && selectedSpatState->GetIndex() > 0)
    {
        // Actualisation depuis l'affichage graphique
        sendDataToModel(editionComponent->GetDisplayedSpatMatrix());
        
        spatInterpolator->SwapStatesByIndex(selectedSpatState->GetIndex(), selectedSpatState->GetIndex()-1);
        // Updates
        UpdateView();
        selectSpatState(selectedSpatState); // re-selection
    }
    else
        throw std::logic_error("Cannot move spat state towards the first position.");
}
void SpatStatesEditionManager::OnMoveSelectedStateDown()
{
    if (selectedSpatState
        && spatInterpolator->GetStatesCount() >= 2
        && selectedSpatState->GetIndex() < spatInterpolator->GetStatesCount()-1)
    {
        // Actualisation depuis l'affichage graphique
        sendDataToModel(editionComponent->GetDisplayedSpatMatrix());
        
        spatInterpolator->SwapStatesByIndex(selectedSpatState->GetIndex(), selectedSpatState->GetIndex()+1);
        // Updates
        UpdateView();
        selectSpatState(selectedSpatState); // re-selection
    }
    else
        throw std::logic_error("Cannot move spat state towards the last position.");
}



// = = = = = = = = = = GRAPHICAL HELPERS = = = = = = = = = =

void SpatStatesEditionManager::UpdateView()
{
    // GUI update (copy of whole vector)
    std::vector<std::shared_ptr<ControlState<double>>> newSpatStates = spatInterpolator->GetStates();
    editionComponent->UpdateStatesList(newSpatStates);
}



// = = = = = = = = = = INTERNAL HELPERS = = = = = = = = = =

void SpatStatesEditionManager::sendDataToModel(std::shared_ptr<ControlMatrix> currentMatrix)
{
    if (selectedSpatState) // if exists
    {
        if (std::shared_ptr<MatrixState<double>> matrixState = std::dynamic_pointer_cast<MatrixState<double>>(selectedSpatState) )
            matrixState->SetMatrix(currentMatrix);
        else
            throw std::logic_error("State is not a Matrix (behavior not implemented)");
    }
}


// = = = = = = = = = = SETTINGS MANAGEMENT = = = = = = = = = =
void SpatStatesEditionManager::AllowKeyboardEdition(bool allow)
{
    editionComponent->AllowKeyboardEdition(allow);
}


// = = = = = = = = = = PROPERTY TREE (XML) MANAGEMENT = = = = = = = = = =
std::shared_ptr<bptree::ptree> SpatStatesEditionManager::GetTree()
{
    // à chaque fois qu'on demande l'arbre :
    // D'abord on met à jour vers le modèle...
    sendDataToModel(editionComponent->GetDisplayedSpatMatrix());
    // Puis le modèle mis à jour renvoie les données bien formattées
    return spatInterpolator->GetStatesTree();
}



