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
    

    // Typename for easiness of use ; the floating-point type follows what is defined
    // in ControlModel.h
    typedef AudioMatrix<double, Miam_MaxNumInputs, Miam_MaxNumOutputs, Miam_MinVolume_PowOf10> ControlMatrix;
    
}
