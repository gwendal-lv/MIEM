/*
  ==============================================================================

    ModelDefines.h
    Created: 16 May 2017 11:18:53am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once



#include "AudioDefines.h"


#define MiamRouter_MaxBufferSize            16384

#define MiamRouter_DefaultAttackTime_ms     5
#define MiamRouter_DefaultVolume            (0.0f)


namespace Miam
{
    enum class DataOrigin
    {
        InitialValue,
        
        PluginProcessorModel, ///< For debug purposes mostly
        
        NetworkModel,
        
        Presenter,
        
        Daw,
        DawInit,

		View, ///< for debug purposes only at the moment
    };
    
    
    // - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - -
    // Async Param Changes types specification for the miam matrix router only

    // - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - -
}
