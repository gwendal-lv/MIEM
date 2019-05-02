/*
  ==============================================================================

    MinMaxSlidersPair.h
    Created: 10 Mar 2019 10:56:51am
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
        virtual double snapValue(double attemptedValue, DragMode dragMode) override
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
    
    
    
    
    /// \brief Pair of min/max sliders (extrem values depend on each other)
    class MinMaxSlidersPair : public Component, public Slider::Listener
    {
        // =========== ATTRIBUTES ==========
        private :
        LabelledMatrixComponent* labelledMatrixParent;
        int rowIndex;
        
        MinMaxRowSlider minSlider;
        MinMaxRowSlider maxSlider;
        
        // =========== Getters and Setters ==========
        public :
        void SetMinValue(double newValue)
        {
            minSlider.setValue(newValue, NotificationType::sendNotificationSync);
        }
        void SetMaxValue(double newValue)
        {
            maxSlider.setValue(newValue, NotificationType::sendNotificationSync);
        }
        double GetMinValue() {return minSlider.getValue();}
        double GetMaxValue() {return maxSlider.getValue();}
        
        
        // =========== METHODS ==========
        public :
        MinMaxSlidersPair(LabelledMatrixComponent* _parent, const String &componentName, int _rowIndex)
        :
        Component (componentName),
        
        labelledMatrixParent(_parent),
        rowIndex(_rowIndex),
        
        minSlider(rowIndex, false),
        maxSlider(rowIndex, true)
        {
            addAndMakeVisible(&minSlider);
            addAndMakeVisible(&maxSlider);
            // Pour partir de valeurs correctes...
            minSlider.setValue(0.0, NotificationType::dontSendNotification);
            maxSlider.setValue(1.0, NotificationType::dontSendNotification);
            // Ajout des listeners ensuite
            minSlider.addListener(this);
            maxSlider.addListener(this);
        }
        
        virtual void paint(Graphics& g) override
        {
        }

        virtual void resized() override
        {
            // inter-sliders margin = 4px
            const int sliderDistanceFromEdge = getWidth()/2 + 4/2;
            minSlider.setBounds( getLocalBounds().withTrimmedRight(sliderDistanceFromEdge) );
            maxSlider.setBounds( getLocalBounds().withTrimmedLeft(sliderDistanceFromEdge) );
        }
        
        /// \brief Callback from the min/max value sliders
        virtual void sliderValueChanged (Slider *slider) override
        {
            auto minMaxSlider = dynamic_cast<MinMaxRowSlider*>(slider);
            if (minMaxSlider)
            {
                // Le snap a déjà fait son travail si besoin...
                
                // Si on est dans ce callback, c'est qu'il n'a pas eu besoin de snapper
                // dont on met à jour les limites actuelles
                minSlider.SetCurrentMinAndMax(minSlider.getValue(), maxSlider.getValue());
                maxSlider.SetCurrentMinAndMax(minSlider.getValue(), maxSlider.getValue());
                
                // Notif du parent
                labelledMatrixParent->OnMinMaxValuesChanged(rowIndex,
                                                            minSlider.getValue(), maxSlider.getValue());
            }
            else
                assert(false); // only MinMaxRowSlider should callback at the moment
        }
    };
}

