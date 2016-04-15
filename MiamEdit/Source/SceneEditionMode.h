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
    
    // The modes that describes the current state of the scene manager
    enum class SceneEditionMode {
        
        
        Null, // undefined state
        
        Loading, // when scene manager is loading something ?
        
        NoAreaSelected, // nothing selected, ready to process mouse input from canvas
        PolygonSelected, // polygon selected, still ready to process mouse input from canvas
        EditingArea, // mouse is editing the current area
        
        WaitingForPointCreation, // mouse input will create a point
        WaitingForPointDeletion // mouse input will delete a point
        
    };
    
}


#endif  // SCENEEDITIONMODE_H_INCLUDED
