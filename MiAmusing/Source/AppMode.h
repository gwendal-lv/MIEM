/*
  ==============================================================================

    AppMode.h
    Created: 24 Mar 2016 1:29:02pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef APPMODE_H_INCLUDED
#define APPMODE_H_INCLUDED


namespace Amusing {
    
    /// \brief Describes the current running mode of the Presenter (the MiamSpat GUI).
	/// 
	/// The app may be running in one of all these different modes, except "Null"
    enum class AppMode {
        
        Null, /**< Undefined state */
        None, /**< No app mode selected (menu bar available only) */
        
        Loading, /**< Between modes change */
        
        Playing /**< normally spatialising */
        
    };
    
}


#endif  // APPMODE_H_INCLUDED
