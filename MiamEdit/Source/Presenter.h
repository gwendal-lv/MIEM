/*
  ==============================================================================

    Presenter.h
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef PRESENTER_H_INCLUDED
#define PRESENTER_H_INCLUDED

#include <iostream>

#include "AppMode.h"

#include "SceneEditionManager.h"



namespace Miam {
    // Simple declaration : we don't need the entire description
    class View;
    

    
    /// \brief One of the three main modules of the MiamEdit program, that defines the behavior
	/// between the user and displayed GUI elements. Miam-specific graphical objects belong to it.
	///
	/// Read <a href="http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter">MPV scheme</a>
	/// for a better understanding of MiamEdit functionnalities between the Model, the Presenter 
	/// and the View : "The presenter acts upon the model and the view. It retrieves data from 
	/// repositories (the model), and formats it for display in the view".
	///
	/// \remark Usual GUI controls (created from the Projucer) however belong to the View.
    class Presenter
    {
        private :
        
        // Pointer to the unique View module
        View* view;
        
        AppMode appMode;
        
        
        // Sub-modules
        SceneEditionManager sceneEditionManager;
        
        
        public :
        // Only constructor
        Presenter(View* _view);
        
        // Getters and Setters
        AppMode getAppMode() {return appMode;};
        SceneEditionManager* getSceneEditionManager() {return &sceneEditionManager;}
        
        // Events from the View
        AppMode appModeChangeRequest(AppMode newAppMode);
        
    };
    
    
    
}



#endif  // PRESENTER_H_INCLUDED
