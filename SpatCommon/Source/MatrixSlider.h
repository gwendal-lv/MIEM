/*
  ==============================================================================

    MatrixSlider.h
    Created: 4 May 2017 4:38:12pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <iostream>

#include <iomanip> // setprecision
#include <sstream> // stringstream

#include <string>

#include <cmath>

#include "AudioDefines.h"

// We'll authorize the slider to get a bit lower than the Miam_MinVolume_dB
// (but the values will not actually be processed by the model)
#define MiamRouter_LowVolumeThreshold_dB        (0.1)


namespace Miam
{
    
    /// \brief Box within a matrix slider, displaying a limited number of digits only
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
            // On récupère d'abord des infos sur ce nombre
            bool isPositive = (value >= 0.0);
            double valueAbs = isPositive ? value : (-value);
            int nbFiguresBeforeComa;
            if (valueAbs < 1.0)
                nbFiguresBeforeComa = 1; // on compte le zéro pour l'affichage
            else
                // the + 0.00001 is here to compensate for log10(10^x) < x
                // due to numeric approximations (for double on macOS, intel proc...)
                nbFiguresBeforeComa = (int)std::ceil(std::log10(valueAbs + 0.00001));
                
            std::stringstream numberStream;
            numberStream << std::fixed << std::setprecision(numberOfDigits-nbFiguresBeforeComa) << value;
            std::string result = numberStream.str();
            if (isPositive)
                result = "+" + result;
            
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
                    float hue = (getValue()-GetMinVolume_dB()) / (GetMaxVolume_dB()-GetMinVolume_dB());
                    hue = (1.0-hue)*0.5 + 0.1;
                    uint8 alpha;
                    if (GetIsActive())
                        alpha = 0xff;
                    else
                        alpha = 0x77;
                    
                    
                    SetBackgroundColour(Colour(hue, 1.0, 0.7, alpha));
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
