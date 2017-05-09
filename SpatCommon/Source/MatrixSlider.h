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
        
        
        
        public :
        
        MatrixSlider(int _nbOfDigits = 3)
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
            
            
            setRange(GetMinVolume(), GetMaxVolume());
            setValue(GetMinVolume());
            //slider->setDoubleClickReturnValue(true, getMinVolume());
        }
        
        void SetBackgroundColour(Colour _backgroundColour)
        {
            backgroundColour = _backgroundColour;
        }
        
        
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
                nbFiguresBeforeComa = (int)std::ceil(std::log10(valueAbs));
            
            std::stringstream numberStream;
            numberStream << std::fixed << std::setprecision(numberOfDigits-nbFiguresBeforeComa) << value;
            std::string result = numberStream.str();
            if (isPositive)
                result = "+" + result;
            
            return String(result);
        }
            
            
            
            
            // - - - - - Volume management functions - - - - -
            public :
            static double GetMaxVolume() {return 6.0;}
            static double GetMinVolume() {return -60.0;}
            
        
    };
    
}
