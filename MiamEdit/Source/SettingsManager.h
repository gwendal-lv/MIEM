/*
  ==============================================================================

    SettingsManager.h
    Created: 25 May 2017 11:09:10am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once



namespace Miam {
    
    // Forward declarations
    class Presenter;
    class Model;
    class View;
    
    
    
    /// \brief Sub-Module of the Miam::Presenter
    ///
    /// Not intended to be re-employed in the Miam spat player
    class SettingsManager
    {
        private :
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        // Pointer to the unique View module
        View* view = 0;
        // Pointer to the unique Model module
        Model* model = 0;
        // Pointer to the unique Presenter module
        Presenter* presenter = 0;
        
        
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        
        public :
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // - - - - - Contruction and Destruction - - - - -
        // Only constructor
        SettingsManager(View* _view, Presenter* _presenter);
        
        /// \brief To be called from the Miam::Model when it is being
        /// constructed
        void CompleteInitialisation(Model* _model);
        
        
        // - - - - - Callbacks from View - - - - -
        void OnInOutChannelsCountChanged(int inputsCount, int outputsCount);
        void OnAllowKeyboardEdition(bool allow);
        
        
    };
    
    
    
}

