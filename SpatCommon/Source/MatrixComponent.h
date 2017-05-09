/*
 ==============================================================================
 
 MatrixComponent.h
 Created: 4 May 2017 11:42:00am
 Author:  Gwendal Le Vaillant
 
 ==============================================================================
 */

#pragma once

// To avoid projects' versions and names conflicts
//#include "../../MatrixRouter/JuceLibraryCode/JuceHeader.h"

#include <vector>
#include <cmath>

#include "MatrixSlider.h"

#include "LabelledMatrixComponent.h"

namespace Miam
{
    
    //==============================================================================
    /*
     */
    class MatrixComponent    : public Component, public Slider::Listener
    {
        
        // ======================= ATTRIBUTES ===========================
        private :
        
        // Children and parent components
        LabelledMatrixComponent* grandParent;
        std::vector<ScopedPointer<MatrixSlider>> sliders;
        
        // Graphics
        const unsigned int maxRowsCount;
        const unsigned int maxColsCount;
        int n; // Number of actually displayed rows
        int m; // Number of actually displayed columns
        
        const int itemW = 40;
        const int itemH = 20;
        
        
        // ======================= METHODS ===========================
    public:
        
        // - - - - - Construction and Destruction - - - - -
        
        MatrixComponent(LabelledMatrixComponent* _grandParent, unsigned int _maxRowsCount, unsigned int _maxColsCount)
        :
        grandParent(_grandParent),
        maxRowsCount(_maxRowsCount), maxColsCount(_maxColsCount)
        {
            n = maxRowsCount;
            m = maxColsCount;
            
            ReconstructGuiObjects();
        }
        
        ~MatrixComponent()
        {
        }
        
        
        // - - - - - Setters and Getters - - - - -
        void SetSliderValue(int row, int col, double newValue)
        {
            MatrixSlider* slider = sliders[row*maxColsCount + col].get();
            // To prevent direct backwards retransmission
            slider->removeListener(this);
            slider->setValue(newValue);
            slider->addListener(this);
            
            // Graphical update
            setSliderColourFromVolume(slider);
        }
        
        
        // - - - - - Juce graphics - - - - -
        
        void paint (Graphics& g) override
        {
            /* This demo code just fills the component's background and
             draws some placeholder text to get you started.
             
             You should replace everything in this method with your own
             drawing code..
             */
            
            g.fillAll (Colours::lightgrey);   // clear the background
            
            g.setColour (Colours::grey);
            g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
        }
        
        
        void resized() override
        {
            setSize(m*itemW, n*itemH);
        }
        
        void RepositionGuiObjects()
        {
            // Actual positionning
            // i repesents a *graphical* row (= matrix row in this case...)
            for (int i=0 ; i<n ; i++)
            {
                // j repesents a *graphical* column (= matrix col now...)
                for (int j=0 ; j<m ; j++)
                {
                    int idx = i*n + j; // graphically one col further than the matrix
                    sliders[idx]->setBounds((int)round(j*itemW), (int)round(i*itemH), (int)round(itemW), (int)round(itemH));
                }
            }
        }
        
        void ReconstructGuiObjects()
        {
            // Auto-sizing at first
            resized();
            
            // Pre-allocations for vector of scoped pointers
            sliders.clear();
            sliders.resize(n*m);
            
            // Actual creation of sliders and labels
            for (int i=0 ; i<n ; i++)
            {
                for (int j=0 ; j<m ; j++)
                {
                    // Slider (i,j)
                    int idx = i*n+j;
                    sliders[idx] = new MatrixSlider();
                    sliders[idx]->setName("Slider ID=" + std::to_string(idx) + " : row=" + std::to_string(i) + " col=" + std::to_string(j));
                    sliders[idx]->setComponentID(std::to_string(idx));
                    initAndAddSlider(sliders[idx]);
                    
                    setSliderColourFromVolume(sliders[idx]);
                }
            }
            
            // Graphical placement
            RepositionGuiObjects();
        }
        
        // - Internal graphical helpers -
        private :

        void initAndAddSlider(Slider* slider)
        {
            addAndMakeVisible(slider);
            slider->addListener(this);
        }
        void setSliderColourFromVolume(MatrixSlider* slider)
        {
            // Graphical update depending on new value
            if (slider->getValue() < MatrixSlider::GetMinVolume()+0.5)
            {
                slider->SetBackgroundColour(Colours::white);
            }
            else
            {
                float hue = (slider->getValue()-MatrixSlider::GetMinVolume()) / (MatrixSlider::GetMaxVolume()-MatrixSlider::GetMinVolume());
                hue = (1.0-hue)*0.5 + 0.1;
                slider->SetBackgroundColour(Colour(hue, 1.0, 0.5, (uint8)0xff));
            }
        }
        
        
        
        // - - - - - Callbacks - - - - -
        public :
        
        /// \brief
        ///
        /// Might be called from mouse input as well as from direct value assignation
        /// from c++ code.
        void sliderValueChanged(Slider* _slider) override
        {
            MatrixSlider* slider = dynamic_cast<MatrixSlider*>(_slider);
            if (slider)
            {
                // Slider identification
                int sliderId = std::stoi(slider->getComponentID().toStdString());
                unsigned int sliderRow = (unsigned int) std::floor((double)(sliderId)/(double)(maxRowsCount));
                unsigned int sliderCol = sliderId % maxColsCount;
                
                // Data transmission
                grandParent->OnSliderValueChanged(sliderRow, sliderCol, slider->getValue());
                
                // Graphical update depending on new value
                setSliderColourFromVolume(slider);
            }
        }
        
        
        
        // - - - - - Volume management functions - - - - -
        public :
        
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixComponent)
    };
    
    
} // namespace Miam;
