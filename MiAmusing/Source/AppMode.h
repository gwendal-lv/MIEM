/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
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
