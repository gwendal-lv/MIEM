/*
  ==============================================================================

    SceneCanvasMode.h
    Created: 11 Oct 2016 3:18:15pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENECANVASMODE_H_INCLUDED
#define SCENECANVASMODE_H_INCLUDED


namespace Miam {
    
    /// \brief Describes the current running mode of a MultiSceneCanvasManager, which is a
    /// sub-sub-module of the Miam::Presenter.
    enum class SceneCanvasMode {
        
        
        Null, ///< Undefined state
        
        Loading, ///< When the manager is loading something (useless at the moment ??)
        
        Unselected, ///< Not currently selected
        
        NothingSelected, ///< No area or point selected ; ready to process any input
        CanvasSelected, ///< Undefined mode : the canvas is selected, but something else might be selected inside
        
        AreaSelected, ///< Area selected, still ready to process mouse input from CanvasComponent
        EditingArea, ///< Mouse is editing the current area
        
        WaitingForPointCreation, ///< Next mouse input will create a point
        WaitingForPointDeletion ///< Next mouse input will delete a point
        
    };
    
}


#endif  // SCENECANVASMODE_H_INCLUDED
