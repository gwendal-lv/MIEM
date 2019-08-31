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

#include "AudioMatrix.hpp"
#include "AudioDefines.h"

namespace Miam {
    
    // Alias declaration (from c++11)
    template <typename T> // supposed to be a floating-point type
    using ControlMatrix = AudioMatrix<T, Miam_MaxNumInputs, Miam_MaxNumOutputs, Miam_MinVolume_PowOf10>;
    
}
