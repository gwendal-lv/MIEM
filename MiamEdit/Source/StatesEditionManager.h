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

#ifndef SPATSTATESEDITIONMANAGER_H_INCLUDED
#define SPATSTATESEDITIONMANAGER_H_INCLUDED

#include "StatesInterpolator.hpp"
#include "ControlMatrix.hpp"

#include "AppPurpose.h"

namespace Miam
{
    // forward declarations
    class View;
    class Model;
    class StatesEditionComponent;
    /*class SpatInterpolator<double>;*/ // from Model
    
    
    /// \brief Manages the behavior (Presenter functionnalities) of a
    /// Miam::SpatStatesEditionComponent
    class StatesEditionManager
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        // Links to other modules
        
        View* view = nullptr;
        StatesEditionComponent* editionComponent = nullptr;
        /// \brief for direct OSC messages send requests... this link should
        /// be deleted if more functionnalities are implemented
        Model* model = nullptr;
        
        std::shared_ptr<StatesInterpolator<double>> interpolator = nullptr; // from Model
        
        // Selected spat state
        std::shared_ptr<ControlState<double>> selectedState = nullptr;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        InterpolationType GetInterpolationType() {return interpolator->GetType();}
        AppPurpose GetSessionPurpose();
    private :
        void selectSpatState(std::shared_ptr<ControlState<double>> _controlState);
    public :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        
        StatesEditionManager(View* _view);
        void CompleteInitialisation(Model* _model, std::shared_ptr<StatesInterpolator<double>> _spatInterpolator);
        void Reinit();
        
        // - - - - - Events from Presenter - - - - -
        /// \brief Actualization of everything that might have been modified from
        /// other app modes.
        void OnEnterSpatStatesEdition();
        /// \brief Data saving (and returning by pointer) before entering
        /// another mode.
        std::shared_ptr<bptree::ptree> OnLeaveSpatStatesEdition();
        /// \brief Retransmis depuis le Miam::SettingsManager
        void OnInOutNamesDisplayedChanged(bool areInputNamesVisible, bool areOutputNamesVisible);
        
        // - - - - - Events from View - - - - -
        
        /// \brief Called with the next index to select, and directly with the current
        /// matrix (that must be saved to the state to be unselected)
        void OnSpatStateSelectedById(std::shared_ptr<ControlMatrix<double>> currentMatrix, int _spatStateId);
        /// \brief Called when the displayed text of the combo box has been edited
        void OnRenameState(std::string newName, int stateIndex);
        void OnAddState();
        void OnDeleteSelectedState();
        /// \brief Sends the full state if row==-1, or a unique row if asked.
        ///
        /// This function might be called internally, or after a callback on a button.
        void OnSendState(int rowToSend = -1);
        void OnSendZeros();
        void OnMoveSelectedStateUp();
        void OnMoveSelectedStateDown();
        
        void OnColourChanged(Colour& colour);
        
        void OnMatrixValueChanged(int row, int col, double matrixValue);
        void OnMatrixButtonClicked(int row, int col, std::string matrixText, double matrixValue);
        
        /// \brief Will trigger the update of all internal normalized values of states
        void OnInterpolationCurveChanged(int row, BasicInterpolationCurve<double>& newInterpCurve);
        
        // - - - - - Graphical helpers - - - - -
        
        /// \brief Updates graphic components but not the data of the state itself.
        void UpdateView();
        private :
        /// \brief For now : only computes the total volume of a matrix, and displays it
        void updateStateInfo();
        std::string getLinkedAreasInfo();
        
        
        // - - - - - Internal helpers - - - - -
        private :
        // fait toutes les mises à jour, transfert des données graphiques à l'écran vers le modèle
        void sendCurrentDataToModel();
        // Met à jour uniquement la mat passée en paramètre
        void sendMatrixDataToModel();
        
        // - - - - - Settings Management - - - - -
        public :
        void AllowKeyboardEdition(bool allow);
        
        
        // - - - - - Property tree (XML) management - - - - -
        /// \brief Va demander au Modèle les états de spatialisation courants,
        /// puis les renvoie.
        std::shared_ptr<bptree::ptree> GetTree();

        
    };
    
}


#endif  // SPATSTATESEDITIONMANAGER_H_INCLUDED
