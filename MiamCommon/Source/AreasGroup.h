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
        // ========================= ATTRIBUTES =========================
        private :
        
        std::vector<std::shared_ptr<InteractiveArea>> areas;
        
        // for a display in an image
        juce::Colour colour;
        // Random generator is always the same, init with the same seed (for debugging)
        static std::mt19937 randomGenerator;
        
        
        
        // ========================= Gets and Sets =========================
        public :
        Colour GetColour() {return colour;}
        
        
        
        // ========================= METHODS =========================
        public :
        AreasGroup(bool randomColour = true);
        
        
        
        
        static void ReinitRandomColours(int newSeed = 0) { randomGenerator.seed(newSeed); }
        
    };
}
