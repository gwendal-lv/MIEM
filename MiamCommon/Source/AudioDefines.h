/*
  ==============================================================================

    AudioDefines.h
    Created: 10 May 2017 5:32:12pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#define Miam_MinVolume_dB               (-60.0)
#define Miam_MinVolume                  (0.001)
#define Miam_MinVolume_PowOf10          (-3)
#define Miam_MaxVolume_dB               (6.0)
#define Miam_MaxVolume                  (1.9952623149688)

// Overall maxima (particular applications, VSTs for example, might not
// tolerate such numbers of inputs/outputs)
#define Miam_MaxNumInputs               64
#define Miam_MaxNumOutputs              64
