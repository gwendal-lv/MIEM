/*
  ==============================================================================

    MinMaxRowSlider.h
    Created: 10 Mar 2019 10:56:51am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <cmath>

#include "boost/lexical_cast.hpp"

#include "JuceHeader.h"

namespace Miam
{
    class MinMaxRowSlider : public Slider {
        
        
        // =========== ATTRIBUTES ==========
        private :
        const int rowIndex;
        const bool isMax; ///< If true, is a Max slider ; else, is a min slider
        Colour backColour;
        
        
        
        // =========== Getters and Setters ==========
        public :
        int GetRowIndex() const {return rowIndex;}
        bool GetIsMax() const {return isMax;}
        
        
        // =========== METHODS ==========
        // ction and dtion
        public :
        MinMaxRowSlider(int _rowIndex, bool _isMax, double minMaxValues = 1000000.0)
        :
        // expression ternaire dégueu... pour que tout passe dans la liste d'init du cteur
        Slider( (_isMax ? "Max" : "Min") + std::string(" Value slider ") + boost::lexical_cast<std::string>(_rowIndex)),
        rowIndex(_rowIndex),
        isMax(_isMax)
        {
            // Slider lui-même : type et valeurs
            setSliderStyle(SliderStyle::LinearBar);
            setRange(-std::abs(minMaxValues), std::abs(minMaxValues), 0.0);
            
            // Texte (qui sera automatiquement à l'intérieur avec les linear bar)
            setColour(ColourIds::textBoxTextColourId, Colours::white);
            if (isMax)
                backColour = Colours::darkgrey;
            else
                backColour = Colours::black;
            
            setColour(ColourIds::backgroundColourId, backColour);
            setColour(ColourIds::trackColourId, backColour);
        }
        virtual ~MinMaxRowSlider() {}
        
        
        virtual void paint(Graphics& g) override
        {
            // fond en noir
            if (isEnabled())
                g.fillAll(backColour);
            else
                g.fillAll(Colours::transparentBlack);
            
            Slider::paint(g);
        }
        
        virtual void enablementChanged() override
        {
            if (isEnabled())
            {
                setColour(ColourIds::backgroundColourId, backColour);
                setColour(ColourIds::trackColourId, backColour);
            }
            else
            {
                setColour(ColourIds::backgroundColourId, Colours::transparentBlack);
                setColour(ColourIds::trackColourId, Colours::transparentBlack);
            }
        }
        
        
    };
}

