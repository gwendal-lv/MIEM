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




namespace Miam
{
    
    /// \brief Abtract class inherited by any object that wants to receive
    /// the sliders values' modification from a Miam::LabelledMatrixComponent
    class ISlidersMatrixListener
    {
        public :
        virtual void OnSliderValueChanged(int row, int col, double value) = 0;
        virtual void OnMatrixZeroed() = 0;
        
        virtual void OnInterpolationCurveChanged(int row, BasicInterpolationCurve<double> newInterpCurve) = 0;
        
        virtual ~ISlidersMatrixListener() {}
    };
    
    
} // namespace Miam
