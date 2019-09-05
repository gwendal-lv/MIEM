/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <sstream> // std::ostringstream

#include "StatesEditionManager.h"

#include "Model.h"
#include "MiamOscSender.hpp" // pour méthode statique d'adresse OSC par défaut
#include "View.h"
#include "StatesEditionComponent.h"

#include "TextUtils.h"



using namespace Miam;

StatesEditionManager::StatesEditionManager(View* _view)
{
    view = _view;
    
    editionComponent = view->GetMainContentComponent()->GetSpatStatesEditionComponent();
    editionComponent->CompleteInitialization(this);
}

void StatesEditionManager::CompleteInitialisation(Model* _model, std::shared_ptr<StatesInterpolator<double>> _statesInterpolator)
{
    model = _model;
    interpolator = _statesInterpolator;
    
    Reinit();
}

void StatesEditionManager::Reinit()
{
    // Update of the list on the GUI side
    selectSpatState(nullptr);
	editionComponent->Reinit();
    UpdateView();
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
AppPurpose StatesEditionManager::GetSessionPurpose()
{
    return view->GetSessionPurpose();
}

void StatesEditionManager::selectSpatState(std::shared_ptr<ControlState<double>> _state)
{
    // Internal update at first
    selectedState = _state;
    
    // Graphical updates : info label (links count)
    std::string infoText = getLinkedAreasInfo();
    auto matrixToSend = std::make_shared<ControlMatrix<double>>(); // initially full of zeros
    int stateIndexToSend = -1;
    if (selectedState)
    {
        stateIndexToSend = selectedState->GetIndex();
        
        // At last : routing matrix (only choice available for now...)
        if (std::shared_ptr<MatrixState<double>> matrixState = std::dynamic_pointer_cast<MatrixState<double>>(selectedState) )
            matrixToSend = matrixState->GetMatrixCopy();
        // else if the cast did not work
        else
            throw std::runtime_error("State is not a Matrix state");
    }
    // Update commands
    Colour colourToDisplay = selectedState ? selectedState->GetColour() : Colours::black;
    if (selectedState)
    {
        // Update des courbes d'interpolation AVEC  l'état est maintenant obligatoire
        editionComponent->SelectAndUpdateState(stateIndexToSend, infoText,
                                               matrixToSend, colourToDisplay,
                                               interpolator->GetInterpolationCurves());
    }

    // State info (may not de anything depending on the app's purpose)
    updateStateInfo();
}


// = = = = = = = = = = EVENTS from PRESENTER = = = = = = = = = =
void StatesEditionManager::OnEnterSpatStatesEdition()
{
    // Forces graphical updates concerning the state
    selectSpatState(selectedState);
    // Concernant tous les états (donc aucun en particulier...) màj des noms des canaux
    InOutChannelsName interpolatorChannelsName = interpolator->GetInOutChannelsName();
    editionComponent->SetInOutNames(interpolatorChannelsName);
}
std::shared_ptr<bptree::ptree> StatesEditionManager::OnLeaveSpatStatesEdition()
{
    // !!!!!!!!!
    // !!!!!!!!!
    // Transfert des données depuis l'affichage graphique vers le modèle
    sendCurrentDataToModel(); // en double ! Sera refait juste après, au GetTree...
    // Mais on laisse pour l'instant puisque ça fonctionne....
    
    // Update now to the editionComponent
    selectSpatState(selectedState);
    
    // Save to XML (Presenter does it)
    return GetTree();
}
void StatesEditionManager::OnInOutNamesDisplayedChanged(bool areInputNamesVisible, bool areOutputNamesVisible)
{
    editionComponent->SetInOutNamesDisplayed(areInputNamesVisible, areOutputNamesVisible);
}

// = = = = = = = = = = EVENTS from VIEW = = = = = = = = = =

void StatesEditionManager::OnSpatStateSelectedById(std::shared_ptr<ControlMatrix<double>> currentMatrix, int _spatStateId)
{
    // All data save within Model if necessary
    sendCurrentDataToModel();
    
    // Check for the information (not updated info might come the GUI, we never know...)
    if (_spatStateId == -1)
        selectSpatState(nullptr);
    else if (0 <= _spatStateId && _spatStateId < interpolator->GetStatesCount())
        selectSpatState(interpolator->GetState(_spatStateId));
    
    // This shouldn't happen, so we throw an exception (for debug only ?)
    else
        throw std::out_of_range("State Id does not exist within the Model");
}
void StatesEditionManager::OnRenameState(std::string newName, int stateIndex)
{
    // Sauvegarde avant tout
    sendCurrentDataToModel();
    
    // Actual renaming
    interpolator->GetState(stateIndex)->SetName(newName);
    
    // Total list update + Selection of the state that has just been renamed
    UpdateView();
    selectSpatState(interpolator->GetState(stateIndex));
}


void StatesEditionManager::OnAddState()
{
    // Actualisation depuis l'affichage graphique
    sendCurrentDataToModel();
    
    // Puis addition
    auto newState = interpolator->AddDefaultState();
    
    if (GetSessionPurpose() == AppPurpose::GenericController)
        newState->SetName(TRANS("Default state").toStdString());
    
    UpdateView();
    selectSpatState(newState);
}
void StatesEditionManager::OnDeleteSelectedState()
{
    if (selectedState)
    {
        // Model command transmission at first
        size_t spatStateIndexBackup = (size_t)selectedState->GetIndex();
        interpolator->DeleteState(selectedState);
        // Display updates
        UpdateView();
        if (spatStateIndexBackup == 0)
        {
            // if it was the last one, nothing selected
            if (interpolator->GetStatesCount() == 0)
                selectSpatState(nullptr);
            else // else, the next one is selected
                selectSpatState(interpolator->GetState(spatStateIndexBackup));
        }
        else // selection of the previous
            selectSpatState(interpolator->GetState(spatStateIndexBackup-1));
    }
    else
        throw std::logic_error("Cannot delete state: no state is currently selected.");
}
void StatesEditionManager::OnSendState(int rowToSend)
{
    // At first : Actualisation depuis l'affichage graphique
    sendCurrentDataToModel();
    
    // Si le modèle a bien envoyé : on affiche ENVOYÉ
    try {
        if (selectedState)
        {
            model->ConnectAndSendState(selectedState, rowToSend);
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
void StatesEditionManager::OnSendZeros()
{
    // At first : Actualisation depuis l'affichage graphique
    // Car : même si on n'utilise pas les valeurs des paramètres, il faut quand même
    // actualiser les adresses OSC
    sendCurrentDataToModel();
    
    // Création d'un matrix state nul
    auto nullMatrixState = std::make_shared<MatrixState<double>>();
    nullMatrixState->SetInputOutputChannelsCount(interpolator->GetInputsCount(),
                                                interpolator->GetOutputsCount());
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
void StatesEditionManager::OnMoveSelectedStateUp()
{
    if (selectedState
        && interpolator->GetStatesCount() >= 2
        && selectedState->GetIndex() > 0)
    {
        // Actualisation depuis l'affichage graphique
        sendCurrentDataToModel();
        
        interpolator->SwapStatesByIndex(selectedState->GetIndex(), selectedState->GetIndex()-1);
        // Updates
        UpdateView();
        selectSpatState(selectedState); // re-selection
    }
    else
        // Cannot move spat state towards the first position
        assert(false);
}
void StatesEditionManager::OnMoveSelectedStateDown()
{
    if (selectedState
        && interpolator->GetStatesCount() >= 2
        && selectedState->GetIndex() < interpolator->GetStatesCount()-1)
    {
        // Actualisation depuis l'affichage graphique
        sendCurrentDataToModel();
        
        interpolator->SwapStatesByIndex(selectedState->GetIndex(), selectedState->GetIndex()+1);
        // Updates
        UpdateView();
        selectSpatState(selectedState); // re-selection
    }
    else
        //Cannot move spat state towards the last position
        assert(false);
}
void StatesEditionManager::OnColourChanged(Colour& colour)
{
    if (selectedState)
        selectedState->SetColour(colour);
}

void StatesEditionManager::OnMatrixValueChanged(int /*row*/, int /*col*/, double /*matrixValue*/)
{
    updateStateInfo();
}
void StatesEditionManager::OnMatrixButtonClicked(int row, int /*col*/,
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

void StatesEditionManager::OnInterpolationCurveChanged(int row, BasicInterpolationCurve<double>& newInterpCurve)
{
    interpolator->UpdateStatesForInterpolationCurve(row, newInterpCurve);
}



// = = = = = = = = = = GRAPHICAL HELPERS = = = = = = = = = =

void StatesEditionManager::UpdateView()
{
    // GUI update (copy of whole vector)
    std::vector<std::shared_ptr<ControlState<double>>> newSpatStates = interpolator->GetStates();
    editionComponent->UpdateStatesList(newSpatStates);
    // also : input/outputs names
    auto inOutNames = interpolator->GetInOutChannelsName();
    editionComponent->SetInOutNames(inOutNames);
    // and interpolation curves
    auto interpCurves = interpolator->GetInterpolationCurves();
    editionComponent->GetLabelledMatrix()->SetInterpolationCurves(interpCurves);
    
    updateStateInfo();
    
    //editionComponent->resized(); // pas mal de modifs si le AppPurpose de session a changé
}
void StatesEditionManager::updateStateInfo()
{
	if (selectedState)
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
std::string StatesEditionManager::getLinkedAreasInfo()
{
    std::string infoText;
    if (selectedState)
    {
        infoText = "Linked to "
        + boost::lexical_cast<std::string>(selectedState->GetLinkedAreasCount())
        + " area" + (selectedState->GetLinkedAreasCount()>1 ? "s" : "") + ".";
    }
    else // if no state selected
        infoText = TRANS("No state selected.").toStdString();
    return infoText;
}


// = = = = = = = = = = INTERNAL HELPERS = = = = = = = = = =

void StatesEditionManager::sendCurrentDataToModel()
{
    // matrice
    sendMatrixDataToModel(editionComponent->GetDisplayedSpatMatrix());
    // noms des canaux et courbes d'interpolation
    auto channelsNameCopy = editionComponent->GetLabelledMatrix()->GetChannelsName();
    interpolator->SetInOutChannelsName(channelsNameCopy);
    auto interpCurvesPtr = editionComponent->GetLabelledMatrix()->GetInterpolationCurves();
    interpolator->SetInterpolationCurves(interpCurvesPtr);
}
void StatesEditionManager::sendMatrixDataToModel(std::shared_ptr<ControlMatrix<double>> currentMatrix)
{
    if (selectedState) // if exists
    {
        if (std::shared_ptr<MatrixState<double>> matrixState = std::dynamic_pointer_cast<MatrixState<double>>(selectedState) )
            matrixState->SetMatrix(currentMatrix);
        else
            throw std::logic_error("State is not a Matrix (behavior not implemented)");
    }
}


// = = = = = = = = = = SETTINGS MANAGEMENT = = = = = = = = = =
void StatesEditionManager::AllowKeyboardEdition(bool allow)
{
    editionComponent->AllowKeyboardEdition(allow);
}


// = = = = = = = = = = PROPERTY TREE (XML) MANAGEMENT = = = = = = = = = =
std::shared_ptr<bptree::ptree> StatesEditionManager::GetTree()
{
    // à chaque fois qu'on demande l'arbre :
    // D'abord on met à jour vers le modèle...
    sendCurrentDataToModel();
    
    // Puis le modèle mis à jour renvoie les données bien formattées
    return interpolator->GetStatesTree();
}



