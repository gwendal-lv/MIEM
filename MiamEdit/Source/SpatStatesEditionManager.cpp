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
#include "MiamOscSender.hpp" // pour méthode statique d'adresse OSC par défaut
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
	editionComponent->Reinit();
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
    auto matrixToSend = std::make_shared<ControlMatrix<double>>(); // initially full of zeros
    int stateIndexToSend = -1;
    if (selectedSpatState)
    {
        stateIndexToSend = selectedSpatState->GetIndex();
        
        // At last : routing matrix (only choice available for now...)
        if (std::shared_ptr<MatrixState<double>> matrixState = std::dynamic_pointer_cast<MatrixState<double>>(selectedSpatState) )
            matrixToSend = matrixState->GetMatrixCopy();
        // else if the cast did not work
        else
            throw std::runtime_error("State is not a Matrix state");
    }
    // Update commands
    Colour colourToDisplay = selectedSpatState ? selectedSpatState->GetColour() : Colours::black;
    if (selectedSpatState)
    {
        // Update des courbes d'interpolation AVEC  l'état est maintenant obligatoire
        editionComponent->SelectAndUpdateState(stateIndexToSend, infoText,
                                               matrixToSend, colourToDisplay,
                                               spatInterpolator->GetInterpolationCurves());
    }

    // State info (may not de anything depending on the app's purpose)
    updateStateInfo();
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

void SpatStatesEditionManager::OnSpatStateSelectedById(std::shared_ptr<ControlMatrix<double>> currentMatrix, int _spatStateId)
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
void SpatStatesEditionManager::OnSendState(int rowToSend)
{
    // At first : Actualisation depuis l'affichage graphique
    sendCurrentDataToModel();
    
    // Si le modèle a bien envoyé : on affiche ENVOYÉ
    try {
        if (selectedSpatState)
        {
            model->ConnectAndSendState(selectedSpatState, rowToSend);
            view->DisplayInfo(TRANS("OSC Messages sent to ").toStdString() + model->GetStateSender(0)->GetAddressAsString());
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
    // At first : Actualisation depuis l'affichage graphique
    // Car : même si on n'utilise pas les valeurs des paramètres, il faut quand même
    // actualiser les adresses OSC
    sendCurrentDataToModel();
    
    // Création d'un matrix state nul
    auto nullMatrixState = std::make_shared<MatrixState<double>>();
    nullMatrixState->SetInputOuputChannelsCount(spatInterpolator->GetInputsCount(),
                                                spatInterpolator->GetOutputsCount());
    // Si le modèle a bien envoyé : on affiche ENVOYÉ
    try {
        model->ConnectAndSendState(nullMatrixState);
        view->DisplayInfo(TRANS("OSC Messages sent to ").toStdString() + model->GetStateSender(0)->GetAddressAsString());
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
        // Cannot move spat state towards the first position
        assert(false);
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
        //Cannot move spat state towards the last position
        assert(false);
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
void SpatStatesEditionManager::OnMatrixButtonClicked(int row, int /*col*/,
											std::string matrixText, double /*matrixValue*/)
{
    if (GetSessionPurpose() == AppPurpose::GenericController)
    {
        // On checke d'abord si le message OSC est OK...
        try {
            TextUtils::ParseStringToJuceOscMessage(matrixText);
        }
        catch(ParseException&) {
            // Et on corrige l'erreur pour envoyer qqchose dans tous les cas
            matrixText = MiamOscSender<double>::GetDefaultOscAddress(row);
            
            //view->DisplayInfo( TRANS("Cannot send the OSC message: incorrect OSC address pattern.").toStdString() );
            //return;
        }
        
        // On utilise alors la méthode générale
        // d'envoi d'un coefficient, limitée à une seule ligne
        this->OnSendState(row);
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
    // and interpolation curves
    auto interpCurves = spatInterpolator->GetInterpolationCurves();
    editionComponent->GetLabelledMatrix()->SetInterpolationCurves(interpCurves);
    
    updateStateInfo();
    
    //editionComponent->resized(); // pas mal de modifs si le AppPurpose de session a changé
}
void SpatStatesEditionManager::updateStateInfo()
{
	if (selectedSpatState)
	{
		// Pour la SPAT seulement : calcul et affichage du volume de la matrice
		if (GetSessionPurpose() == AppPurpose::Spatialisation)
		{
			// spat engine correlation :
			CorrelationLevel spatEngineCorrelationLevel;
			if (GetInterpolationType() == InterpolationType::Matrix_ConstantPower)
				spatEngineCorrelationLevel = CorrelationLevel::Low;
			else if (GetInterpolationType() == InterpolationType::Matrix_ConstantAmplitude)
				spatEngineCorrelationLevel = CorrelationLevel::High;
			else
				return;// interpolator type is not properly set yet

			// 1 is correlated, 2 is decorrelated
			auto linearVolume1 = editionComponent->GetDisplayedSpatMatrix()->ComputeTotalVolume(CorrelationLevel::High, spatEngineCorrelationLevel);
			auto linearVolume2 = editionComponent->GetDisplayedSpatMatrix()->ComputeTotalVolume(CorrelationLevel::Low, spatEngineCorrelationLevel);
			auto volume1_dBFS = TextUtils::GetAmplitude_dB_string_from_Linear(linearVolume1, 3);
			auto volume2_dBFS = TextUtils::GetAmplitude_dB_string_from_Linear(linearVolume2, 3);

			// graphic display
			editionComponent->SetVisibleMatrixData(true);
			editionComponent->UpdateMatrixData(volume1_dBFS, volume2_dBFS);
		}
		else if (GetSessionPurpose() == AppPurpose::GenericController)
		{
			editionComponent->SetVisibleMatrixData(false);
		}
	}
	else
		editionComponent->SetVisibleMatrixData(false);
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
        infoText = TRANS("No state selected.").toStdString();
    return infoText;
}


// = = = = = = = = = = INTERNAL HELPERS = = = = = = = = = =

void SpatStatesEditionManager::sendCurrentDataToModel()
{
    // matrice
    sendMatrixDataToModel(editionComponent->GetDisplayedSpatMatrix());
    // noms des canaux et courbes d'interpolation
    auto channelsNameCopy = editionComponent->GetLabelledMatrix()->GetChannelsName();
    spatInterpolator->SetInOutChannelsName(channelsNameCopy);
    auto interpCurvesPtr = editionComponent->GetLabelledMatrix()->GetInterpolationCurves();
    spatInterpolator->SetInterpolationCurves(interpCurvesPtr);
}
void SpatStatesEditionManager::sendMatrixDataToModel(std::shared_ptr<ControlMatrix<double>> currentMatrix)
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



