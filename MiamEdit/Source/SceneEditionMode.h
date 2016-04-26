/*
  ==============================================================================

    SceneEditionMode.h
    Created: 7 Apr 2016 2:07:08pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENEEDITIONMODE_H_INCLUDED
#define SCENEEDITIONMODE_H_INCLUDED


namespace Miam {
    
    /// \brief Describes the current running mode of SceneEditionManager, which is a sub-module of
    /// the Presenter.
    enum class SceneEditionMode {
        
        
        Null, ///< Undefined state
        
        Loading, ///< When scene manager is loading something (useless at the moment ??)
        
        NothingSelected, ///< No canvas or area selected ; ready to process any input
        
        CanvasSelected, ///< A canvas is selected (but no area) ; ready to process mouse input from canvas
        PolygonSelected, ///< Polygon selected, still ready to process mouse input from canvas
        EditingArea, ///< Mouse is editing the current area
        
        WaitingForPointCreation, ///< Next mouse input will create a point
        WaitingForPointDeletion ///< Next mouse input will delete a point
        
    };
    
}


#endif  // SCENEEDITIONMODE_H_INCLUDED
