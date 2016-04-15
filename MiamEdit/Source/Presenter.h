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
    

    
    /*
     *
     */
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
