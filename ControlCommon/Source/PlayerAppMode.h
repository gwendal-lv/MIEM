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
        
        Playing, /**< normally spatialising, interactive canvases (playgrounds) displayed */
        Stopped, /**< Spatialisation is currently stopped, canvases are displayed but disabled */
        
    };
    
}


#endif  // PLAYERAPPMODE_H_INCLUDED
