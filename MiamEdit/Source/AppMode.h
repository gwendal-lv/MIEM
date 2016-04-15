/*
  ==============================================================================

    AppMode.h
    Created: 24 Mar 2016 1:29:02pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef APPMODE_H_INCLUDED
#define APPMODE_H_INCLUDED


namespace Miam {
    
    // The app may be running in one of all these different modes, except "Null"
    enum class AppMode {
        
        Null, // undefined state
        None, // no app mode selected (menu bar available only)
        
        Loading, // between modes change
        
        // Normal editing modes
        EditSpeakers,
        EditSpeakersGroups,
        EditSpatScenes,
        EditHardwareConfiguration,
        
        // When the external Miam Spat Player is running
        MiamSpatPlaying
    };
    
}


#endif  // APPMODE_H_INCLUDED
