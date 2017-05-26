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

#include "SpatModel.h" // sparse miam spatialization matrix

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
        const size_t maxRowsCount;
        const size_t maxColsCount;
        
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
            // Initial values (to be changed after construction)
            n = 0;
            m = 0;
            
            ReconstructGuiObjects();
        }
        
        ~MatrixComponent()
        {
        }
        
        // Auxiliary functions
        private :
        inline size_t idx(size_t i, size_t j) { return i*maxColsCount+j; }
        public:
        
        // - - - - - Setters and Getters - - - - -
        void SetSliderValue(int row, int col, double newValue)
        {
            MatrixSlider* slider = sliders[idx(row,col)].get();
            // To prevent direct backwards retransmission
            slider->setValue(newValue, NotificationType::dontSendNotification);
            
            // Graphical update
            slider->SetPropertiesFromVolume();
        }
        void SetActiveSliders(int inputsCount, int outputsCount)
        {
            // Rows updating
            if (n != inputsCount)
            {
                // For all existing cols, we do something
                for (int j=0 ; j<m ; j++)
                {
                    if (inputsCount > n) // Increase of the rows count
                    {
                        for (int i=n ; i<inputsCount ; i++)
                            sliders[idx(i,j)]->SetIsActive(true);
                    }
                    else // Decrease of the rows count
                    {
                        for (int i=n-1 ; i>=inputsCount ; i--)
                            sliders[idx(i,j)]->SetIsActive(false);
                    }
                }
                n = inputsCount;
            }
                
            // Cols updating, based on the current up-to-date rows number
            if (m != outputsCount)
            {
                // For all existing rows, we do something
                for (int i=0 ; i<n ; i++)
                {
                    if (outputsCount > m) // Increase of the cols count
                    {
                        for (int j=m ; j<outputsCount ; j++)
                            sliders[idx(i,j)]->SetIsActive(true);
                    }
                    else // Decrease of the cols count
                    {
                        for (int j=m-1 ; j>=outputsCount ; j--)
                            sliders[idx(i,j)]->SetIsActive(false);
                    }
                }
                m = outputsCount;
            }
            
            // Repaint at the very end
            repaint();
        }
        void SetSlidersTextBoxesAreEditable(bool shouldBeEditable)
        {
            for (int i=0 ; i<maxRowsCount ; i++)
                for (int j=0 ; j<maxColsCount ; j++)
                    sliders[idx(i,j)]->setTextBoxIsEditable(shouldBeEditable);
        }
        
        /// \brief Builds and constructs the corresponding Miam::SpatMatrix
        std::shared_ptr<SpatMatrix> GetSpatMatrix()
        {
            // VLA (Variable Length Array) is not OK with the sparse matrix ctor
            double* rawDenseMatrix;
            rawDenseMatrix = new double[maxRowsCount*maxColsCount];
            for (int i=0 ; i<maxRowsCount ; i++)
            {
                for (int j=0 ; j<maxColsCount ; j++)
                {
                    // If non-zero only
                    if (sliders[idx(i,j)]->getValue() > sliders[idx(i,j)]->GetMinVolume_dB() + MiamRouter_LowVolumeThreshold_dB)
                    {
                        double linearValue = Decibels::decibelsToGain(sliders[idx(i,j)]->getValue());
                        rawDenseMatrix[idx(i,j)] = linearValue;
                    }
                    else
                        rawDenseMatrix[idx(i,j)] = 0.0;
                }
            }
            // construction optimisée
            std::shared_ptr<SpatMatrix> returnPtr = std::make_shared<SpatMatrix>(rawDenseMatrix);
            delete rawDenseMatrix;
            return returnPtr;
        }
        
        /// \brief Updates its internal sliders from the given Miam::SpatMatrix
        void SetSpatMatrix(std::shared_ptr<SpatMatrix> spatMatrix)
        {
            // Reset to an undisplayed value in dB
            for (size_t i=0 ; i<maxRowsCount ; i++)
                for (size_t j=0 ; j<maxColsCount ; j++)
                    SetSliderValue(i,j,Miam_MinVolume_dB-MiamRouter_LowVolumeThreshold_dB);
            
            // Update of non-zero sliders only (input is a sparse matrix)
            for( spatMatrix->ResetIterator() ;
                spatMatrix->GetIterator() < spatMatrix->GetEndIterator();
                spatMatrix->IncIterator() )
            {
                SpatMatrix::Index2d current2dCoord = spatMatrix->GetIterator2dCoord();
                double sliderValue = Decibels::gainToDecibels(spatMatrix->GetIteratorValue());
                SetSliderValue(current2dCoord.i, current2dCoord.j, sliderValue);
            }
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
            setSize(maxRowsCount*itemW, maxColsCount*itemH);
        }
        
        void RepositionGuiObjects()
        {
            // Actual positionning
            // i repesents a *graphical* row (= matrix row in this case...)
            for (int i=0 ; i<maxRowsCount ; i++)
            {
                // j repesents a *graphical* column (= matrix col now...)
                for (int j=0 ; j<maxColsCount ; j++)
                {
                    int idx = i*maxRowsCount + j; // graphically one col further than the matrix
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
            sliders.resize(maxRowsCount*maxColsCount);
            
            // Actual creation of sliders and labels
            for (int i=0 ; i<maxRowsCount ; i++)
            {
                for (int j=0 ; j<maxColsCount ; j++)
                {
                    // Slider (i,j)
                    sliders[idx(i,j)] = new MatrixSlider();
                    sliders[idx(i,j)]->setName("Slider ID=" + std::to_string(idx(i,j)) + " : row=" + std::to_string(i) + " col=" + std::to_string(j));
                    sliders[idx(i,j)]->setComponentID(std::to_string(idx(i,j)));
                    initAndAddSlider(sliders[idx(i,j)]);
                    
                    sliders[idx(i,j)]->SetPropertiesFromVolume();
                }
            }
            
            // Active sliders
            SetActiveSliders(n, m);
            
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
                slider->SetPropertiesFromVolume();
            }
        }
        
        
        
        // - - - - - Volume management functions - - - - -
        public :
        
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixComponent)
    };
    
    
} // namespace Miam;
