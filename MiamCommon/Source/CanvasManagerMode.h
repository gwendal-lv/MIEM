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
        
        Loading, ///< When the manager is loading something (useless at the moment ??)
        
        
        
        // - - - Modes relevant when your program is editing/playing something - - -
        
        
        Unselected, ///< Not currently selected
        
        NothingSelected, ///< No area or point selected ; ready to process any input
        CanvasSelected, ///< Undefined mode : the canvas is selected, but something else might be selected inside
        
        AreaSelected, ///< Area selected, still ready to process mouse input from CanvasComponent
        EditingArea, ///< Mouse is editing the current area
        
        WaitingForPointCreation, ///< Next mouse input will create a point
        WaitingForPointDeletion ///< Next mouse input will delete a point
        
        
        
        
        //
        
    };
    
}



#endif  // CANVASMANAGERMODE_H_INCLUDED
