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


namespace Miam
{
    enum class DataOrigin
    {
        InitialValue,
        //PluginProcessorModel,
        
        NetworkModel,
        
        Presenter,
        
        Daw,
        DawInit,
    };
    
    
    // - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - -
    // Async Param Changes types specification for the miam matrix router only

    // - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - -
}
