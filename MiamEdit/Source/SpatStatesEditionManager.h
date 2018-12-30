/*
  ==============================================================================

    SpatStatesEditionManager.h
    Created: 26 Nov 2016 6:47:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
    class SpatStatesEditionComponent;
    /*class SpatInterpolator<double>;*/ // from Model
    
    
    /// \brief Manages the behavior (Presenter functionnalities) of a
    /// Miam::SpatStatesEditionComponent
    class SpatStatesEditionManager
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        // Links to other modules
        View* view;
        SpatStatesEditionComponent* editionComponent;
        Model* model; // for direct OSC messages send requests... this link should is to be deleted if more functionnalities are implemented
        std::shared_ptr<StatesInterpolator<double>> spatInterpolator; // from Model
        
        // Selected spat state
        std::shared_ptr<ControlState<double>> selectedSpatState = nullptr;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        InterpolationType GetInterpolationType() {return spatInterpolator->GetType();}
        AppPurpose GetSessionPurpose();
    private :
        void selectSpatState(std::shared_ptr<ControlState<double>> _controlState);
    public :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        
        SpatStatesEditionManager(View* _view);
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
        void OnSpatStateSelectedById(std::shared_ptr<ControlMatrix> currentMatrix, int _spatStateId);
        /// \brief Called when the displayed text of the combo box has been edited
        void OnRenameState(std::string newName, int stateIndex);
        void OnAddState();
        void OnDeleteSelectedState();
        void OnSendState();
        void OnSendZeros();
        void OnMoveSelectedStateUp();
        void OnMoveSelectedStateDown();
        
        void OnColourChanged(Colour& colour);
        
        void OnMatrixButtonClicked(int row, int col, std::string matrixText, double matrixValue);
        
        
        // - - - - - Graphical helpers - - - - -
        
        void UpdateView();
        
        
        // - - - - - Internal helpers - - - - -
        private :
        // fait toutes les mises à jour, transfert des données graphiques à l'écran vers le modèle
        void sendCurrentDataToModel();
        // Met à jour uniquement la mat passée en paramètre
        void sendMatrixDataToModel(std::shared_ptr<ControlMatrix> currentMatrix);
        
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
