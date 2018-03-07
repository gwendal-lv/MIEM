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
    
    /// \brief Describes the current running mode of the Presenter (the MiamEdit GUI).
	/// 
	/// The app may be running in one of all these different modes, except "Null"
    enum class AppMode {
        
        Null, /**< Undefined state */
        None, /**< No app mode selected (menu bar available only) */
        
        Startup, /**< During the launch phase of the app */
        
        Loading, /**< Between modes change */
        
        EditSpeakers, /**< Normal single speakers editing mode */
        EditSpatStates, /**< Normal spatialization state editing mode (routing matrix only at the moment) */
        EditSpatScenes,  /**< Normal spat scene editing mode */
        EditHardwareConfiguration,  /**< Normal hardware configuration editing mode */
        
        MiamSpatPlaying /**< When the external Miam::Spat Player is running */
    };
    
}


#endif  // APPMODE_H_INCLUDED
