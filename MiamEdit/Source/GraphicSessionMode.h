/*
  ==============================================================================

    SceneEditionMode.h
    Created: 7 Apr 2016 2:07:08pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef GRAPHICSESSIONMODE_H_INCLUDED
#define GRAPHICSESSIONMODE_H_INCLUDED


namespace Miam {
    
    /// \brief Describes the current running mode of GraphicSessionManager, which is
    /// a sub-module of the Presenter.
    enum class GraphicSessionMode {
        
        
        
        Null, ///< Undefined state
        
        Loading, ///< When scene manager is loading something (useless at the moment ??)
        Loaded, ///< When loading has just finished
        
        NothingSelected, ///< No canvas or area selected ; ready to process any input
        
        CanvasSelected, ///< A canvas is selected (but no area) ; ready to process mouse input from canvas
        
        ExcitersEdition, ///< On est en mode d'édition des excitateurs, mais aucun excitateurs n'est sélectionné pour l'instant
        
        // - - - - - Info coming from the canvas - - - - -
        // - - - - -(still interesting for the edition  manager to know) - - - - -
    
        AreaSelected, ///< Area selected, still ready to process mouse input from CanvasComponent
        
        ExciterSelected, ///< Un unique excitateur est sélectionné. Mode qui suit le mode ExcitersEditionMode, qui peut être suivi de ce même mode si l'on désélectionne l'excitateur considéré.
        
        /* Useless state for now ? We should maybe hide all buttons... If a stupid
         * multitouch user clicks a button while editing an area with another hand
         
        EditingArea, ///< Mouse is editing the current area
         */
        
        WaitingForPointCreation, ///< Next mouse input will create a point
        WaitingForPointDeletion ///< Next mouse input will delete a point
        
    };
    
}


#endif  // GRAPHICSESSIONMODE_H_INCLUDED
