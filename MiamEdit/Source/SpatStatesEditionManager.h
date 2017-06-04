/*
  ==============================================================================

    SpatStatesEditionManager.h
    Created: 26 Nov 2016 6:47:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATSTATESEDITIONMANAGER_H_INCLUDED
#define SPATSTATESEDITIONMANAGER_H_INCLUDED

#include "SpatInterpolator.hpp"
#include "SpatMatrix.hpp"

namespace Miam
{
    // forward declarations
    class View;
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
        std::shared_ptr<SpatInterpolator<double>> spatInterpolator; // from Model
        
        // Selected spat state
        std::shared_ptr<SpatState<double>> selectedSpatState = nullptr;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        SpatType GetSpatType() {return spatInterpolator->GetSpatType();}
    private :
        void selectSpatState(std::shared_ptr<SpatState<double>> _spatState);
    public :
        
        // 1 dimension speakers' volumes, faders edition
        size_t GetFadersCount();
        std::string GetFaderName(size_t _i) {
            throw std::logic_error("Plus utilisé dans la version avec matrices");
         //   return spatInterpolator->GetOutputName(_i);
        }
        
    
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        
        SpatStatesEditionManager(View* _view);
        void CompleteInitialisation(std::shared_ptr<SpatInterpolator<double>> _spatInterpolator);
        
        
        // - - - - - Events from Presenter - - - - -
        void OnLeaveSpatStatesEdition();
        
        // - - - - - Events from View - - - - -
        
        /// \brief Called with the next index to select, and directly with the current
        /// matrix (that must be saved to the state to be unselected)
        void OnSpatStateSelectedById(std::shared_ptr<SpatMatrix> currentMatrix, int _spatStateId);
        /// \brief Called when the displayed text of the combo box has been edited
        void OnRenameState(std::string newName, int stateIndex);
        
        void OnAddState();
        void OnDeleteSelectedState();
        void OnMoveSelectedStateUp();
        void OnMoveSelectedStateDown();
        
        
        // - - - - - Graphical helpers - - - - -
        
        void updateView();
        
        
        // - - - - - Internal helpers - - - - -
        void sendDataToModel(std::shared_ptr<SpatMatrix> currentMatrix);
        
        // - - - - - Settings Management - - - - -
        void AllowKeyboardEdition(bool allow);

    };
    
}


#endif  // SPATSTATESEDITIONMANAGER_H_INCLUDED
