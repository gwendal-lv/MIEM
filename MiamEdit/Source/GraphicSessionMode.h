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

#ifndef GRAPHICSESSIONMODE_H_INCLUDED
#define GRAPHICSESSIONMODE_H_INCLUDED


namespace Miam {
    
    /// \brief Describes the current running mode of GraphicSessionManager, which is
    /// a sub-module of the Presenter.
    enum class GraphicSessionMode {
        
        
        
        Null, ///< Undefined state
        
        Loading, ///< When scene manager is loading something (useless at the moment ??)
        Loaded, ///< When loading has just finished
        
        NothingSelected, ///< No canvas or area selected ; ready to process any input
        
        CanvasSelected, ///< A canvas is selected (but no area) ; ready to process mouse input from canvas
        
        ExcitersEdition, ///< On est en mode d'édition des excitateurs, mais aucun excitateurs n'est sélectionné pour l'instant
        
        // - - - - - Info coming from the canvas - - - - -
        // - - - - -(still interesting for the edition  manager to know) - - - - -
    
        AreaSelected, ///< Area selected, still ready to process mouse input from CanvasComponent
        
        ExciterSelected, ///< Un unique excitateur est sélectionné. Mode qui suit le mode ExcitersEditionMode, qui peut être suivi de ce même mode si l'on désélectionne l'excitateur considéré.
        
        /* Useless state for now ? We should maybe hide all buttons... If a stupid
         * multitouch user clicks a button while editing an area with another hand
         
        EditingArea, ///< Mouse is editing the current area
         */
        
        WaitingForPointCreation, ///< Next mouse input will create a point
        WaitingForPointDeletion ///< Next mouse input will delete a point
        
    };
    
}


#endif  // GRAPHICSESSIONMODE_H_INCLUDED
