/*
  ==============================================================================

    SpatStatesEditionManager.cpp
    Created: 26 Nov 2016 6:47:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <sstream> // std::ostringstream

#include "SpatStatesEditionManager.h"

#include "Model.h"
#include "View.h"
#include "SpatStatesEditionComponent.h"

#include "TextUtils.h"



using namespace Miam;

SpatStatesEditionManager::SpatStatesEditionManager(View* _view)
{
    view = _view;
    
    editionComponent = view->GetMainContentComponent()->GetSpatStatesEditionComponent();
    editionComponent->CompleteInitialization(this);
}

void SpatStatesEditionManager::CompleteInitialisation(Model* _model, std::shared_ptr<StatesInterpolator<double>> _statesInterpolator)
{
    model = _model;
    spatInterpolator = _statesInterpolator;
    
    Reinit();
}

void SpatStatesEditionManager::Reinit()
{
    // Update of the list on the GUI side
    selectSpatState(nullptr);
    UpdateView();
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
AppPurpose SpatStatesEditionManager::GetSessionPurpose()
{
    return view->GetSessionPurpose();
}

void SpatStatesEditionManager::selectSpatState(std::shared_ptr<ControlState<double>> _state)
{
    // Internal update at first
    selectedSpatState = _state;
    
    // Graphical updates : info label (links count)
    std::string infoText = getLinkedAreasInfo();
    std::shared_ptr<ControlMatrix> matrixToSend = std::make_shared<ControlMatrix>(); // initially full of zeros
    int stateIndexToSend = -1;
    if (selectedSpatState)
    {
        stateIndexToSend = selectedSpatState->GetIndex();
        
        // At last : routing matrix (only choice available for now...)
        if (std::shared_ptr<MatrixState<double>> matrixState = std::dynamic_pointer_cast<MatrixState<double>>(selectedSpatState) )
            matrixToSend = matrixState->GetMatrix();
        // else if the cast did not work
        else
            throw std::runtime_error("State is not a Matrix state");
    }
    // Update commands
    Colour colourToDisplay = selectedSpatState ? selectedSpatState->GetColour() : Colours::black;
    if (selectedSpatState)
        editionComponent->SelectAndUpdateState(stateIndexToSend, infoText, matrixToSend, colourToDisplay);
    /*
    auto channelsNameCopy = editionComponent->GetLabelledMatrix()->GetChannelsName();
    // ?????? à quoi ça sert ???
    // ?????? à quoi ça sert ???
    // ?????? à quoi ça sert ???
    // ?????? à quoi ça sert ???
    // ?????? à quoi ça sert ???
    editionComponent->SetInOutNames(channelsNameCopy);  // ?????? à quoi ça sert ???
    // ?????? à quoi ça sert ???
    // ?????? à quoi ça sert ???
    // ?????? à quoi ça sert ???
    // ?????? à quoi ça sert ???
    // ?????? à quoi ça sert ???
    */
}


// = = = = = = = = = = EVENTS from PRESENTER = = = = = = = = = =
void SpatStatesEditionManager::OnEnterSpatStatesEdition()
{
    // Forces graphical updates concerning the state
    selectSpatState(selectedSpatState);
    // Concernant tous les états (donc aucun en particulier...) màj des noms des canaux
    InOutChannelsName interpolatorChannelsName = spatInterpolator->GetInOutChannelsName();
    editionComponent->SetInOutNames(interpolatorChannelsName);
}
std::shared_ptr<bptree::ptree> SpatStatesEditionManager::OnLeaveSpatStatesEdition()
{
    // !!!!!!!!!
    // !!!!!!!!!
    // Transfert des données depuis l'affichage graphique vers le modèle
    sendCurrentDataToModel(); // en double ! Sera refait juste après, au GetTree...
    // Mais on laisse pour l'instant puisque ça fonctionne....
    
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
    // All data save within Model if necessary
    sendCurrentDataToModel();
    
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
    sendCurrentDataToModel();
    
    // Actual renaming
    spatInterpolator->GetState(stateIndex)->SetName(newName);
    
    // Total list update + Selection of the state that has just been renamed
    UpdateView();
    selectSpatState(spatInterpolator->GetState(stateIndex));
}


void SpatStatesEditionManager::OnAddState()
{
    // Actualisation depuis l'affichage graphique
    sendCurrentDataToModel();
    
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
void SpatStatesEditionManager::OnSendState()
{
    // At first : Actualisation depuis l'affichage graphique
    sendCurrentDataToModel();
    
    // Si le modèle a bien envoyé : on affiche ENVOYÉ
    try {
        if (selectedSpatState)
        {
            model->ConnectAndSendState(selectedSpatState);
            view->DisplayInfo(TRANS("OSC Message sent to ").toStdString() + model->GetStateSender(0)->GetAddressAsString());
        }
        else
            view->DisplayInfo(TRANS("No state selected (nothing to send).").toStdString());
    }
    // Sinon on affiche l'erreur de connection transmise par le modèle
    catch(Miam::OscException& e) {
        view->DisplayInfo( e.what() );
    }
}
void SpatStatesEditionManager::OnSendZeros()
{
    // Création d'un matrix state nul
    auto nullMatrixState = std::make_shared<MatrixState<double>>();
    nullMatrixState->SetInputOuputChannelsCount(spatInterpolator->GetInputsCount(),
                                                spatInterpolator->GetOutputsCount());
    // Si le modèle a bien envoyé : on affiche ENVOYÉ
    try {
        model->ConnectAndSendState(nullMatrixState);
        view->DisplayInfo(TRANS("OSC Message sent to ").toStdString() + model->GetStateSender(0)->GetAddressAsString());
    }
    // Sinon on affiche l'erreur de connection transmise par le modèle
    catch(Miam::OscException& e) {
        view->DisplayInfo( e.what() );
    }
}
void SpatStatesEditionManager::OnMoveSelectedStateUp()
{
    if (selectedSpatState
        && spatInterpolator->GetStatesCount() >= 2
        && selectedSpatState->GetIndex() > 0)
    {
        // Actualisation depuis l'affichage graphique
        sendCurrentDataToModel();
        
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
        sendCurrentDataToModel();
        
        spatInterpolator->SwapStatesByIndex(selectedSpatState->GetIndex(), selectedSpatState->GetIndex()+1);
        // Updates
        UpdateView();
        selectSpatState(selectedSpatState); // re-selection
    }
    else
        throw std::logic_error("Cannot move spat state towards the last position.");
}
void SpatStatesEditionManager::OnColourChanged(Colour& colour)
{
    if (selectedSpatState)
        selectedSpatState->SetColour(colour);
}

void SpatStatesEditionManager::OnMatrixValueChanged(int /*row*/, int /*col*/, double /*matrixValue*/)
{
    updateStateInfo();
}
void SpatStatesEditionManager::OnMatrixButtonClicked(int /*row*/, int /*col*/,
											std::string matrixText, double matrixValue)
{
    if (GetSessionPurpose() == AppPurpose::GenericController)
    {
        // On checke d'abord si le message OSC est OK... Sinon on envoie rien sauf un msg d'erreur
        try {
            TextUtils::ParseStringToJuceOscMessage(matrixText);
        }
        catch(ParseException&) {
            view->DisplayInfo( TRANS("Cannot send the OSC message: incorrect OSC address pattern.").toStdString() );
            return;
        }
        
        // Si le modèle a bien envoyé : on affiche ENVOYÉ
        try {
            model->ConnectAndSendOSCMessage(matrixText, matrixValue);
            view->DisplayInfo(TRANS("OSC Message sent to ").toStdString() + model->GetStateSender(0)->GetAddressAsString());
        }
        // Sinon on affiche l'erreur de connection transmise par le modèle
        catch(Miam::OscException& e) {
            view->DisplayInfo( e.what() );
        }
    }
}



// = = = = = = = = = = GRAPHICAL HELPERS = = = = = = = = = =

void SpatStatesEditionManager::UpdateView()
{
    // GUI update (copy of whole vector)
    std::vector<std::shared_ptr<ControlState<double>>> newSpatStates = spatInterpolator->GetStates();
    editionComponent->UpdateStatesList(newSpatStates);
    // also : input/outputs names
    auto inOutNames = spatInterpolator->GetInOutChannelsName();
    editionComponent->SetInOutNames(inOutNames);
    
    updateStateInfo();
    
    //editionComponent->resized(); // pas mal de modifs si le AppPurpose de session a changé
}
void SpatStatesEditionManager::updateStateInfo()
{
    std::string stateInfo = getLinkedAreasInfo();
    // Pour la SPAT seulement : calcul et affichage du volume de la matrice
    if (GetSessionPurpose() == AppPurpose::Spatialisation)
    {
        stateInfo = stateInfo + " " + boost::lexical_cast<std::string>(editionComponent->GetDisplayedSpatMatrix()->GetNonZeroCoeffsCount());
    }
    
    editionComponent->UpdateLinksLabel(stateInfo);
}
std::string SpatStatesEditionManager::getLinkedAreasInfo()
{
    std::string infoText;
    if (selectedSpatState)
    {
        infoText = "Linked to "
        + boost::lexical_cast<std::string>(selectedSpatState->GetLinkedAreasCount())
        + " area" + (selectedSpatState->GetLinkedAreasCount()>1 ? "s" : "") + ".";
    }
    else // if no state selected
        infoText = "-";
    return infoText;
}


// = = = = = = = = = = INTERNAL HELPERS = = = = = = = = = =

void SpatStatesEditionManager::sendCurrentDataToModel()
{
    sendMatrixDataToModel(editionComponent->GetDisplayedSpatMatrix());
    auto channelsNameCopy = editionComponent->GetLabelledMatrix()->GetChannelsName();
    spatInterpolator->SetInOutChannelsName(channelsNameCopy);
}
void SpatStatesEditionManager::sendMatrixDataToModel(std::shared_ptr<ControlMatrix> currentMatrix)
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
    sendCurrentDataToModel();
    
    // Puis le modèle mis à jour renvoie les données bien formattées
    return spatInterpolator->GetStatesTree();
}



