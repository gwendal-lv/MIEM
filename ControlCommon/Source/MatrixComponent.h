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

#include "ControlModel.h" // sparse miam spatialization matrix

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
        // matrice
        std::vector<ScopedPointer<MatrixSlider>> sliders;
        // Sliders lorsque l'on n'a qu'une seule colonne
        std::vector<ScopedPointer<Slider>> horizontalSliders;
        
        // Graphics and internal data
        const size_t maxRowsCount;
        const size_t maxColsCount;
        // VLA (Variable Length Array) is not OK with the sparse matrix ctor
        double* rawDenseMatrix;
        
        int n; // Number of actually displayed rows
        int m; // Number of actually displayed columns
        
        const int itemW = 40;
        const int itemH = 20;
        
        
        // ======================= METHODS ===========================
    public:
        
        // - - - - - Construction and Destruction - - - - -
        
        MatrixComponent(LabelledMatrixComponent* _grandParent,
                        unsigned int _maxRowsCount, unsigned int _maxColsCount);
        ~MatrixComponent();
        
        void ReconstructGuiObjects();
        void RepositionGuiObjects();
        
        // Auxiliary functions
        private :
        inline size_t idx(size_t i, size_t j) { return i*maxColsCount+j; }
        Index2d idx2d(size_t indexToConvert);
        public:
        
        // - - - - - Setters and Getters - - - - -
        size_t GetActiveInputsCount() {return n;}
        size_t GetActiveOutputsCount() {return m;}
        void SetActiveSliders(int inputsCount, int outputsCount);
        private :
        AppPurpose getPurpose();
        public :
        void SetSlidersTextBoxesAreEditable(bool shouldBeEditable)
        {
            for (int i=0 ; i<maxRowsCount ; i++)
                for (int j=0 ; j<maxColsCount ; j++)
                    sliders[idx(i,j)]->setTextBoxIsEditable(shouldBeEditable);
        }
        
        /// \brief Updates its internal sliders from the given Miam::SpatMatrix
        void SetSpatMatrix(std::shared_ptr<ControlMatrix> spatMatrix);
        void SetSliderValue_dB(int row, int col, double newValue_dB,
                               NotificationType juceNotification = NotificationType::dontSendNotification);
        void SetSliderValue(int row, int col, double newValue,
                            NotificationType juceNotification = NotificationType::dontSendNotification);
        /// \brief Builds and constructs the corresponding Miam::SpatMatrix
        std::shared_ptr<ControlMatrix> GetSpatMatrix();
        
        
        // - - - - - Juce graphics - - - - -
        public :
        void paint (Graphics& g) override;
        void resized() override;
        
        
        // - Internal graphical helpers -
        private :
        void addSlider(Slider* slider);
        void initAndAddHorizontalSlider(Slider* slider);
        
        
        // - - - - - Callbacks - - - - -
        public :
        
        /// \brief
        ///
        /// Might be called from mouse input as well as from direct value assignation
        /// from c++ code.
        void sliderValueChanged(Slider* _slider) override;
        
        
        
        // - - - - - Volume management functions - - - - -
        public :
        
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixComponent)
    };
    
    
} // namespace Miam;
