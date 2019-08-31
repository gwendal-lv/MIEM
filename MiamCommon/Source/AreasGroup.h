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

#pragma once


#include <random>

#include "JuceHeader.h"



namespace Miam
{
    class InteractiveArea;
    
    
    /// \brief Represents a group of overlapping areas, with utility
    /// function to manage groups of areas
    class AreasGroup : public std::enable_shared_from_this<AreasGroup>
    {
        public :
        // Special groups
        enum class SpecialIds : int {
            
            None = -10, ///< Represents an undefined group
            
            OutOfBounds = -4,
            
            BlockUntilComputationFinished = -3, ///< Represent a special group that blocks but is ready to release
            Blocking = -2, ///< Represent a special group that blocks all elements belonging to it
            Background = -1, ///< The default background group (contains no actual area)
            
            FirstActualGroup = 0, ///< This ID, and all positive IDs represent actual (not special) groups.
        };
        
        
        // ========================= ATTRIBUTES =========================
        private :
        
        /// \brief Index of the group in the scene it belongs to
        int indexInScene;
        
        std::vector<std::shared_ptr<InteractiveArea>> areas;
        
        // for a display in an image
        juce::Colour colour;
        
        
        
        // ========================= Gets and Sets =========================
        public :
        Colour GetColour() const {return colour;}
        int GetIndexInScene() const {return indexInScene; }
        
        /// \brief Returns a colour from a cycle of 14 very different colours
        static Colour GetDefaultColour(int colorIndex);
        
            
        // ========================= METHODS =========================
        public :
        AreasGroup(int _indexInScene, juce::Colour _colour);
        
            
        // - - - -  utility functions - - - -
        private :
        static void buildErosionMasks(std::vector<int>& iMasks, std::vector<int>& jMasks, int erodeSize);
            public :
        /// \brief Applies an erosion algorithm to the given image, stores the result
        /// back into the given vector input.
        ///
        /// Should be written with the ErodeSize as a template parameter, for
        /// optimal results *************** TO DO ****************************
        static void ErodeAreasGroups(std::vector<AreasGroup*>& image, AreasGroup* backArea,
                                      size_t imgW, size_t imgH);
            
    };
}
    
    
