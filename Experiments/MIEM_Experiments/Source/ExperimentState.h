/*
  ==============================================================================

    ExperimentState.h
    Created: 7 Feb 2019 9:26:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <string>

enum class ExperimentState {
    
    NotInitialized,
    
    
    
    // States of the subject :
    
    ReadyToListen,
    
    Listening,
    
    ReadyToSearchPreset,
    
    SearchingPreset,
    FinishedSearchingPreset,
    
    
    Finished,
    
    
};


