/*
  ==============================================================================

    IMatrixButtonListener.h
    Created: 27 Aug 2018 10:41:36pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once



namespace Miam
{
    
    /// \brief Abtract class inherited by any object that wants to receive
    /// a callback when a generic button from a Miam::LabelledMatrixComponent
    /// is clicked
    class IMatrixButtonListener
    {
        public :
        /// \param matrixText Can be empty
        /// \param matrixValue Can be empty
        virtual void OnMatrixButtonClicked(int row, int col, std::string matrixText, double matrixValue) = 0;
        
        virtual ~IMatrixButtonListener() {}
    };
    
    
} // namespace Miam
