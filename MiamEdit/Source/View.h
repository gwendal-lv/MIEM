/*
  ==============================================================================

    View.h
    Created: 11 Mar 2016 5:22:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "MainComponent.h"

#include "DrawableArea.h"


namespace Miam {
    
    class Presenter; // sufficient declaration for a pointer class member
    
    
    
    
    /// \brief One of the three main modules of the Miam::Edit program, that obeys orders from 
	/// the Presenter, while only managing simple display logic
	///
	/// Read <a href="http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter">MPV scheme</a>
	/// for a better understanding of MiamEdit functionnalities between the Model, the Presenter 
	/// and the View : "The view is a passive interface that displays data (the model) and routes
	/// user commands (events) to the presenter to act upon that data."
	///
	/// \remark Usual GUI controls (created from the Projucer) belong to this module.
    class View : public SpatView {
        
        // ========== ATTRIBUTES ==========
        private :
        Presenter* presenter;
        
        // Owned by the MainWindow within the MiamEditApplication
        MainContentComponent* mainContentComponent;
        
        


		// ========== METHODS ==========

        public :
        /// \brief Constructor
		///
		/// \param _mainContentComponent Pointer to the unique MainContentComponent, which is
		/// auto-instanciated by Juce within the MainWindow, and sent to the View module from
		/// the MiamEditApplication.
        View(DocumentWindow* mainWindow_, MainContentComponent* _mainContentComponent);
		/// \brief Destructor
        ~View();
        /// \brief Function called after both View and Presenter are contructed
        void CompleteInitialization(Presenter* _presenter);
        

		// ----- Events to the Presenter -----
        
		/// \brief Function called from all main- and sub-components, transmits
		/// proper events to the Presenter.
        void ButtonClicked(const String& name);
        

		// ----- Events from the Presenter -----

		/// \brief Obeys orders from the Presenter when the global AppMode has changed.
		///
		/// \param newAppMode The application mode that should be displayed as quickly as possible.
        void ChangeAppMode(AppMode newAppMode);
		/// \brief Obeys orders from the Presenter when a informative message has to de displayed to
		/// the user.
		///
		/// \param message Short sentence to be written in the upper uneditable
        /// text box, and maybe in a dialog pop-up like window.
        void DisplayInfo(const std::string& message, bool alsoDisplayInNewWindow = false);
        private :
        /// \brief Crée une nouvelle fenêtre sobre et affiche le message centré.
        ///
        /// Si le début du message est entre crochets, il va devenir le titre de la
        /// fenêtre.
        void displayInfoInNewWindow(const std::string& message);
        
        
        // ----- Setters and Getters -----
	public :
        
        // Basic
		/// \brief Pointer to the unique MainContentComponent.
        MainContentComponent* GetMainContentComponent()
		{return mainContentComponent;}
        
        // General UI controls and displays

        
    };
    
    
    
    
    
}

#endif  // VIEW_H_INCLUDED
