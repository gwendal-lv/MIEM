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

#include <cmath>

#include "boost/lexical_cast.hpp"

#include "JuceHeader.h"

#include "BasicInterpolationCurve.hpp"
#include "LabelledMatrixComponent.h"


namespace Miam
{
    /// \brief Single min/max slider to be put in a matrix row
    class MinMaxRowSlider : public Slider {
        
        // =========== ATTRIBUTES ==========
        private :
        const int rowIndex;
        const bool isMax; ///< If true, is a Max slider ; else, is a min slider
        Colour backColour;
        
        double currentMin = 0.0; ///< Value of the other slider, if this one is the max
        double currentMax = 1.0; ///< Value of the other slider, if this one is the min
        
        
        
        // =========== Getters and Setters ==========
        public :
        int GetRowIndex() const {return rowIndex;}
        bool GetIsMax() const {return isMax;}
        void SetCurrentMinAndMax(double _min, double _max)
        { currentMin = _min; currentMax = _max; }
        
        
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
            
            // Comportement souris
            setSliderSnapsToMousePosition(false);
            setScrollWheelEnabled(false);
            
        }
        virtual ~MinMaxRowSlider() {}
        
        
        /// \brief Overriden to control attempted values
        virtual double snapValue(double attemptedValue, DragMode /*dragMode*/) override
        {
            const double slidersMinOffset = BasicInterpolationCurve<double>::GetMinOutputRange();
            // slider min
            if (! isMax)
                // test validité
                return ( attemptedValue < (currentMax - slidersMinOffset) ) ?
                attemptedValue : currentMax - slidersMinOffset ;
            // slider max
            else
                // test de validité
                return (attemptedValue > (currentMin + slidersMinOffset)) ?
                attemptedValue : currentMin + slidersMinOffset ;
        }
        
        
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
    
    
    /// \brief Slider made for displaying a default value (provides a "inactive but enabled" mode)
    class DefaultSlider : public Slider
    {
        // ========== Attributes ==========
        private :
        
        
        // ========== Setters and Getters ==========
        public :
        
        
        // ========== Methods ==========
        public :
        DefaultSlider(int _rowIndex)
        :
        Slider(std::string("Default slider ") + boost::lexical_cast<std::string>(_rowIndex))
        {
            // Slider lui-même : type et valeurs
            setSliderStyle(SliderStyle::LinearBar);
            // Texte (qui sera automatiquement à l'intérieur avec les linear bar)
            setColour(ColourIds::textBoxTextColourId, Colours::white);
            // Autres couleurs
            setColour(ColourIds::backgroundColourId, Colours::black);
            setColour(ColourIds::trackColourId, Colours::darkgrey);
            // Comportement souris
            setSliderSnapsToMousePosition(false);
            setScrollWheelEnabled(false);
        }
        
        virtual void paint(Graphics& g) override
        {
            // fond en noir ou transparent
            if (isEnabled())
                g.fillAll(Colours::black);
            else
                g.fillAll(Colours::transparentBlack);
            
            Slider::paint(g);
        }
        
        virtual void enablementChanged() override
        {
            if (isEnabled())
                setColour(ColourIds::backgroundColourId, Colours::black);
            else
                setColour(ColourIds::backgroundColourId, Colours::transparentBlack);
        }
    };
    
    
    
    
    /// \brief Pair of min/max sliders (extrem values depend on each other). Now also includes a "default" value, that
    /// can be checked for each parameter of each preset
    class MinDefaultMaxSliders : public Component, public Slider::Listener
    {
        // =========== ATTRIBUTES ==========
        private :
        LabelledMatrixComponent* labelledMatrixParent;
        int rowIndex;
        double lastMinValue = 0.0;
        double lastMaxValue = 1.0;
        
        MinMaxRowSlider minSlider;
        MinMaxRowSlider maxSlider;
        DefaultSlider defaultSlider;
        
        
        // =========== Getters and Setters ==========
        public :
        void SetMinValue(double newValue)
        {
            lastMinValue = newValue;
            assert(lastMinValue < lastMaxValue); // min must remain < to max
            minSlider.setValue(lastMinValue, NotificationType::sendNotificationSync);
            defaultSlider.setRange(lastMinValue, lastMaxValue, 0.0);
        }
        void SetMaxValue(double newValue)
        {
            lastMaxValue = newValue;
            assert(lastMinValue < lastMaxValue); // min must remain < to max
            maxSlider.setValue(lastMaxValue, NotificationType::sendNotificationSync);
            defaultSlider.setRange(lastMinValue, lastMaxValue, 0.0);
        }
        void SetDefaultValue(double newValue)
        {
            defaultSlider.setValue(newValue); // no other check
        }
        void SetDefaultEnabled(bool shouldBeEnabled)
        { defaultSlider.setEnabled(shouldBeEnabled); }
        double GetMinValue() {return minSlider.getValue();}
        double GetMaxValue() {return maxSlider.getValue();}
        double GetDefaultValue() {return defaultSlider.getValue();}
        
        
        // =========== METHODS ==========
        public :
        MinDefaultMaxSliders(LabelledMatrixComponent* _parent, const String &componentName, int _rowIndex)
        :
        Component (componentName),
        
        labelledMatrixParent(_parent),
        rowIndex(_rowIndex),
        
        minSlider(rowIndex, false),
        maxSlider(rowIndex, true),
        defaultSlider(rowIndex)
        {
            // Actual min/max sliders
            addAndMakeVisible(&minSlider);
            addAndMakeVisible(&maxSlider);
            addAndMakeVisible(&defaultSlider);
            // Pour partir de valeurs correctes...
            minSlider.setValue(0.0, NotificationType::dontSendNotification);
            maxSlider.setValue(1.0, NotificationType::dontSendNotification);
            defaultSlider.setValue(0.0, NotificationType::dontSendNotification);
            // Ajout des listeners ensuite
            minSlider.addListener(this);
            maxSlider.addListener(this);
            defaultSlider.addListener(this);
            
            // Default UIs between these
        }
        
        virtual void paint(Graphics& /*g*/) override
        {
        }

        virtual void resized() override
        {
            // inter-sliders margin = 4px
            const int elementBaseWidth = getWidth()/3 - 2;
            minSlider.setBounds( getLocalBounds().removeFromLeft(elementBaseWidth) );
            maxSlider.setBounds( getLocalBounds().removeFromRight(elementBaseWidth) );
            defaultSlider.setBounds( getLocalBounds().withTrimmedLeft(elementBaseWidth).withTrimmedRight(elementBaseWidth) );
        }
        
        /// \brief Callback from the min/max value sliders
        virtual void sliderValueChanged (Slider *slider) override
        {
            if (auto minMaxSlider = dynamic_cast<MinMaxRowSlider*>(slider))
            {
                // Le snap a déjà fait son travail si besoin...
                
                // Si on est dans ce callback, c'est qu'il n'a pas eu besoin de snapper
                // dont on met à jour les limites actuelles
                minSlider.SetCurrentMinAndMax(minSlider.getValue(), maxSlider.getValue());
                maxSlider.SetCurrentMinAndMax(minSlider.getValue(), maxSlider.getValue());
				// forced update of defaultSlider value
				defaultSlider.setRange(minSlider.getValue(), maxSlider.getValue(), 0.0);
				auto defaultValueBackup = defaultSlider.getValue();
				defaultSlider.setValue(minSlider.getValue(), NotificationType::dontSendNotification);
				defaultSlider.setValue(defaultValueBackup, NotificationType::dontSendNotification);
                
                // Notif du parent
                labelledMatrixParent->OnMinMaxValuesChanged(rowIndex,
                                                            minSlider.getValue(), maxSlider.getValue());
            }
            else if (auto castedDefaultSlider = dynamic_cast<DefaultSlider*>(slider))
            {
                assert(castedDefaultSlider == &defaultSlider); // only 1 default should call back
                labelledMatrixParent->OnDefaultValueChanged(rowIndex);
            }
            else
                assert(false); // only MinMaxRowSlider and DefaultSlider should callback at the moment
        }
    };
}

