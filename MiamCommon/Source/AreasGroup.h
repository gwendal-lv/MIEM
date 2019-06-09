/*
  ==============================================================================

    AreasGroup.h
    Created: 8 Jun 2019 5:26:57pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
        // Random generator is always the same, init with the same seed (for debugging)
        static std::mt19937 randomGenerator;
        
        
        
        // ========================= Gets and Sets =========================
        public :
        Colour GetColour() const {return colour;}
        int GetIndexInScene() const {return indexInScene; }
        
        
        // ========================= METHODS =========================
        public :
        AreasGroup(int _indexInScene, bool randomColour = true);
        
        
        
        
        static void ReinitRandomColours(int newSeed = 0) { randomGenerator.seed(newSeed); }
        
    };
}
