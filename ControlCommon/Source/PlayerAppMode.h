/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
