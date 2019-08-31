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

// To avoid projects' versions and names conflicts
//#include "../../MatrixRouter/JuceLibraryCode/JuceHeader.h"

#include <vector>
#include <cmath>

#include "MatrixSlider.h" // small rectangle slider volume, for full compact matrices
#include "MatrixRowSlider.h" // horizontal larger slider

#include "BasicInterpolationCurve.hpp"
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
        std::vector<ScopedPointer<MatrixRowSlider>> horizontalSliders;
        /// \brief et leur courbe d'interpolation associée, qui sont des sauvegardes
        /// locales des vraies courbes d'interpolation (de l'interpolateur du Modèle)
        std::vector<BasicInterpolationCurve<double>> interpolationCurvesCopy;
        
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
        void SetSpatMatrix(std::shared_ptr<ControlMatrix<double>> spatMatrix);
        /// \brief Builds and constructs the corresponding Miam::SpatMatrix
        std::shared_ptr<ControlMatrix<double>> GetSpatMatrix();
        
        /// \brief Same as SetSliderValue, with a decibels input.
        void SetSliderValue_dB(int row, int col, double newValue_dB,
                               NotificationType juceNotification = NotificationType::dontSendNotification);
        
        /// \brief Sets the normalised (linear scale) value of a Slider.
        ///
        /// Sauvegarde la valeur en linéaire à l'intérieur de la classe,
        /// puis met à jour graphiquement les sliders de la matrice, et les sliders horizontaux pour les
        /// coeffs de la 1ière colonne.
        void SetSliderValue(int row, int col, double newValue,
                            NotificationType juceNotification = NotificationType::dontSendNotification);
        
        double GetSliderValue(int row, int col);
        
        /// \brief Sets and horizontal slider range and skew factor
        void SetHorizontalSliderInterpolationData(int row,
                                                  BasicInterpolationCurve<double> newInterpCurve);
        
        
        
        
        
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
        
        /// \brief Fonction appelée dès qu'un slider horizontal, ou une case de matrice a changé.
        /// Le grand-parent (labelled matrice) n'aura en retour que les évènement intéressants
        /// selon le AppPurpose (les sliders horizontaux, ou la matrice de sliders)
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
