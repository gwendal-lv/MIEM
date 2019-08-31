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

#include <iostream>


#include <string>

#include <cmath>

#include "AudioDefines.h"
#include "TextUtils.h"

// We'll authorize the slider to get a bit lower than the Miam_MinVolume_dB
// (but the values will not actually be processed by the model)
#define MiamRouter_LowVolumeThreshold_dB        (0.1)


namespace Miam
{
    
    /// \brief Box within a matrix slider, displaying a limited number of digits only. Values
    /// contained inside are decibels only.
    class MatrixSlider : public Slider
    {
        private :
        
        /// \brief Including figures before and after the point.
        ///
        /// Example : -46.7 has 3 digits, +3.42 has also 3 digits
        int numberOfDigits;
        
        // Because the slider alone doesn't allow a full customization...
        // (on macOS at least, not tested on other platforms)
        Colour backgroundColour;
        
        /// \brief Wether the slider is currently employed within the model
        /// for matrix routing, or juste stored just in case
        bool isActive;
        
        
        public :
        
        MatrixSlider(int _nbOfDigits = 3, bool _authorizeDoubleClickEditing=false)
            :
            numberOfDigits(_nbOfDigits)
        {
            setSliderStyle(Slider::SliderStyle::LinearBarVertical);
            setVelocityBasedMode(true);
            
            // Surlignage en édition
            setColour(Slider::textBoxHighlightColourId, Colours::deepskyblue);
            
            /*
            // Track = la piste qu'on manipule
            // Légèrement transparente pour qu'on voie un petit truc se passer
            setColour(Slider::trackColourId, Colour(0x33ffffff));
            
            // Aucun effet....
            setColour(Slider::backgroundColourId, Colours::black);
            
            // Aucun effet....
            setColour(Slider::thumbColourId, Colours::black);
            
             
            
            // Aucun effet....
            //setColour(Slider::backgroundColourId, Colours::chartreuse);
            */
            
            
            setRange(GetMinVolume_dB(), GetMaxVolume_dB());
            setValue(GetMinVolume_dB());
            
            setTextBoxIsEditable(_authorizeDoubleClickEditing);
            setDoubleClickReturnValue(!_authorizeDoubleClickEditing, 0.0); // 0dB
            
            // deactivated by default
            SetIsActive(false);
        }
        
        void SetBackgroundColour(Colour _backgroundColour)
        {
            backgroundColour = _backgroundColour;
        }
        
        void SetIsActive(bool _isActive)
        {
            isActive = _isActive;
            if (isActive)
                setColour(Slider::textBoxTextColourId, Colours::white);
            else
                setColour(Slider::textBoxTextColourId, Colours::lightgrey);
            
            // Graphical update
            SetPropertiesFromVolume();
        }
        bool GetIsActive() {return isActive;}
        
        
        void paint(Graphics& g) override
        {
            g.fillAll(backgroundColour);
        }

        
        String getTextFromValue(double value) override
        {
            auto result = TextUtils::GetLimitedDigitsString(value, numberOfDigits);
            return String(result);
        }
            
        
        
        
        // - - - - - Volume-related functions - - - - -
        void SetPropertiesFromVolume()
        {
            // Graphical update depending on new value
            
            // If value is a bit lower than the minimum : special pure white box
            if (getValue() < GetMinVolume_dB() + MiamRouter_LowVolumeThreshold_dB)
            {
                // And depending on the active or not state
                if (isActive)
                    SetBackgroundColour(Colours::white);
                else
                    SetBackgroundColour(Colours::lightgrey);
            }
            
            // Displayable value
            else
            {
                // Volume relatif, entre 0 et 1
                double relativeVolume = ( getValue()-GetMinVolume_dB() )
                                        / ( GetMaxVolume_dB()-GetMinVolume_dB() );
                // On applique une distortion simple....
                relativeVolume = std::pow(relativeVolume, 2.5);
                double hue = (1.0-relativeVolume)*0.5 + 0.1;
                uint8 alpha;
                if (GetIsActive())
                    alpha = 0xff;
                else
                    alpha = 0x77;
                
                
                SetBackgroundColour(Colour((float)hue, 1.0f, 0.7f, alpha));
            }
            
            // In any case : possible value on next double click event
            if (! isTextBoxEditable()) // textbox is read-only
            {
                // We'll go to 0dB if lower than the median volume
                if (getValue() < GetMiddleVolume_dB())
                {
                    setDoubleClickReturnValue(true, 0.0);
                }
                // Else, we'll go to the minimum (undisplayed) volume
                else
                {
                    setDoubleClickReturnValue(true, GetMinVolume_dB());
                }
            }
        }
        
        public :
        static double GetMaxVolume_dB()
        {
            return Miam_MaxVolume_dB;
        }
        static double GetMinVolume_dB()
        {
            // we autorize the slider to get a bit lower than
            return Miam_MinVolume_dB - MiamRouter_LowVolumeThreshold_dB;
        }
        static double GetMiddleVolume_dB()
        {
            return (0.0 + Miam_MinVolume_dB)/2.0;
        }
        
        
    };
    
}
