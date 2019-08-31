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

