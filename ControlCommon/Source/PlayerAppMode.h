/*
  ==============================================================================

    PlayerAppMode.h
    Created: 24 Mar 2016 1:29:02pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef PLAYERAPPMODE_H_INCLUDED
#define PLAYERAPPMODE_H_INCLUDED


namespace Miam {
    
    /// \brief Describes the current running mode of the Presenter (the MiamSpat GUI).
	/// 
	/// The app may be running in one of all these different modes, except "Null"
    enum class PlayerAppMode {
        
        Null, /**< Undefined state */
        None, /**< No app mode selected (menu bar available only) */
        
        Loading, /**< Between modes change */
        
        MainMenu, /**< Menu menu is displayed, waiting for a user action */
        LoadingFile, /**< A Native File Chooser is currently being displayed (for loading) */
        //SavingFile, /**< A Native File Chooser is currently being displayed (for saving) */
        
        Playing, /**< normally playing and sending OSC, interactive canvases (playgrounds) displayed */
        Stopped, /**< Interpolation and OSC send are currently stopped, canvases might be displayed but are disabled */
        Stopped_NoPlay, ///< Model does not send, and the user cannot decide to re-play
        
    };
    
}


#endif  // PLAYERAPPMODE_H_INCLUDED
