/*
  ==============================================================================

    ControlMatrix.hpp
    Created: 26 May 2017 9:10:10am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "AudioMatrix.hpp"
#include "AudioDefines.h"

namespace Miam {
    
    // Alias declaration (from c++11)
    template <typename T> // supposed to be a floating-point type
    using ControlMatrix = AudioMatrix<T, Miam_MaxNumInputs, Miam_MaxNumOutputs, Miam_MinVolume_PowOf10>;
    
}
