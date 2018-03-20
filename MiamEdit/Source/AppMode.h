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
        EditControlStates, /**< Normal control state editing mode (matrices or independant coeffs) */
        EditControlScenes,  /**< Normal graphical control scenes editing mode */
        EditSettings,  /**< Normal settings editing mode */
        
        MiemPlayerLaunched, /**< When the external Spat Player (or Generic Controller) is running */
        
        Quitting /**< Le dernier mode, juste avant la fermeture de l'application */
    };
    
}


#endif  // APPMODE_H_INCLUDED
