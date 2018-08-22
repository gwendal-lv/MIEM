/*
  ==============================================================================

    ISlidersMatrixListener.h
    Created: 8 May 2017 3:40:52pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once




namespace Miam
{
    
    /// \brief Abtract class inherited by any object that wants to receive
    /// the sliders values' modification from a Miam::LabelledMatrixComponent
    class ISlidersMatrixListener
    {
        public :
        virtual void OnSliderValueChanged(int row, int col, double value) = 0;
        virtual void OnMatrixZeroed() = 0;
        
        virtual ~ISlidersMatrixListener() {}
    };
    
    
} // namespace Miam
