/*
  ==============================================================================

    SpatMatrix.hpp
    Created: 26 May 2017 9:10:10am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "SparseMatrix.hpp"
#include "AudioDefines.h"

namespace Miam {
    

    // Typename for easiness of use ; the floating-point type follows what is defined
    // in SpatModel.h
    typedef SparseMatrix<double, Miam_MaxNumInputs, Miam_MaxNumOutputs, Miam_MinVolume_PowOf10> SpatMatrix;
    
}
