/*
  ==============================================================================

    CanvasManagerMode.h
    Created: 18 Nov 2016 4:45:41pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef CANVASMANAGERMODE_H_INCLUDED
#define CANVASMANAGERMODE_H_INCLUDED



namespace Miam {
    
    /// \brief Describes the current running mode of a MultiSceneCanvasInteractor, which is a
    /// sub-sub-module a Presenter module.
    enum class CanvasManagerMode {
        
        
        // - - - Common running modes - - -
        
        Null, ///< Undefined state
        
        Loading, ///< When the manager is loading something
        Loaded, ///< When the manager has just finished loading something
        
        
        
        
        // - - - Modes relevant when your program is editing/playing something - - -
        
        
        Unselected, ///< Not currently selected
        
        SceneOnlySelected, ///< No area or point selected ; the selected scene (there is always at least 1 scene on a canvas) is ready to process any input
        
        CanvasSelected, ///< Not fully mode : the canvas is selected, but something else (we don't) might be selected inside
        
        AreaSelected, ///< Area selected, still ready to process mouse input from CanvasComponent
        EditingArea, ///< Mouse is editing the current area
        
        WaitingForPointCreation, ///< Next mouse input will create a point
        WaitingForPointDeletion ///< Next mouse input will delete a point
        
        
        
        
        //
        
    };
    
}



#endif  // CANVASMANAGERMODE_H_INCLUDED
