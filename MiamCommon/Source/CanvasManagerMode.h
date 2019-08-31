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

#ifndef CANVASMANAGERMODE_H_INCLUDED
#define CANVASMANAGERMODE_H_INCLUDED



namespace Miam {
    
    /// \brief Describes the current running mode of a MultiSceneCanvasInteractor, which is a
    /// sub-sub-module a Presenter module.
    enum class CanvasManagerMode {
        
        
        // - - - Common running modes - - -
        
        Null = 0, ///< Undefined state
        
        Loading, ///< When the manager is loading something
        Loaded, ///< When the manager has just finished loading something
        
        
        
        
        // - - - Modes relevant when your program is PLAYING something - - -
        PlayingWithExciters = 10,
        
        
        
        
        // - - - Modes relevant when your program is EDITING something - - -
        
        Unselected = 20, ///< Not currently selected
        CanvasSelected, ///< Not a fully-defined mode : the canvas is selected, but something else (we don't know) might be selected inside
        
        SceneOnlySelected, ///< No area or point selected ; the selected scene (there is always at least 1 scene on a canvas) is ready to process any input
        
        ExcitersEdition, ///< Toutes les aires graphiques ne peuvent pas être modifiées, seuls les excitateurs peuvent interagir avec l'utilisateur tactile/souris. Dans ce mode, aucun excitateur n'est encore sélectionné.
        ExciterSelected, ///< Un unique excitateur est sélectionné. Mode qui suit le mode ExcitersEditionMode, qui peut être suivi de ce même mode si l'on désélectionne l'excitateur considéré.
        
        AreaSelected, ///< Area selected, still ready to process mouse input from CanvasComponent
        
        EditingArea, ///< Mouse is editing the current area (qui peut être un excitateur)
        
        WaitingForPointCreation, ///< Next mouse input will create a point
        WaitingForPointDeletion, ///< Next mouse input will delete a point
        
        
        
        
    };
    
}



#endif  // CANVASMANAGERMODE_H_INCLUDED
