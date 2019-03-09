/*
  ==============================================================================

    MatrixRowSlider.h
    Created: 9 Mar 2019 11:16:38am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace Miam
{
    class MatrixRowSlider : public Slider {
        
        
        // =========== ATTRIBUTES ==========
        private :
        const int height;
        const int width = 100; // px
        
        
        // =========== METHODS ==========
        // ction and dtion
        public :
        MatrixRowSlider(const String &componentName, int _height)
        :
        Slider(componentName),
        height(_height)
        {
            // Slider lui-même
            setSliderStyle(SliderStyle::LinearBar);
            
            // Texte (qui sera automatiquement à l'intérieur avec les linear bar)
            setColour(ColourIds::textBoxTextColourId, Colours::white);
            setColour(ColourIds::backgroundColourId, Colours::black);
            setColour(ColourIds::trackColourId, Colours::darkgrey);
        }
        virtual ~MatrixRowSlider() {}
        
        
        virtual void paint(Graphics& g) override
        {
            // fond en noir
            g.fillAll(Colours::black);
            
            Slider::paint(g);
        }
        
        virtual void enablementChanged() override
        {
            if (isEnabled())
            {
                setColour(ColourIds::backgroundColourId, Colours::black);
                setColour(ColourIds::trackColourId, Colours::darkgrey);
            }
            else
            {
                setColour(ColourIds::backgroundColourId, Colours::transparentBlack);
                setColour(ColourIds::trackColourId, Colour(30,30,30));
            }
        }
        
        
    };
}

