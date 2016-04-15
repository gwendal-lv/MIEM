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
    
    class Presenter;
    
    
    
    
    
    class View {
        
        
        
        
        
        
        private :
        Presenter* presenter;
        
        // Owned by the MainWindow within the MiamEditApplication
        MainContentComponent* mainContentComponent;
        
        
        public :
        // Default constructor and destructor
        View(MainContentComponent* _mainContentComponent);
        ~View();
        // called after View construction
        void CompleteInitialization(Presenter* _presenter);
        
        
        // To be called from all main- and sub-components
        void ButtonClicked(const String& name);
        
        
        // Orders from the Presenter
        void ChangeAppMode(AppMode newAppMode);
        void DisplayInfo(const String& message);
        
        
        
        // ===== Setters and Getters =====
        
        // Basic
        MainContentComponent* getMainContentComponent()
        {return mainContentComponent;}
        
        // General UI controls and displays
        //void

        
    };
    
    
    
    
    
}

#endif  // VIEW_H_INCLUDED
