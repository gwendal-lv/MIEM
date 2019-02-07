/*
  ==============================================================================

    OSCRecorder.h
    Created: 7 Feb 2019 9:09:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <vector>

#include "ExperimentState.h"

class OSCRecorderComponent;

class OSCRecorder {
    
    // PARAMETERS OF THE EXPERIMENT
    public :
    
#ifdef __MIEM_SHORT_DELAYS
    const int delayAfterFinished_ms = 500;
    const int listeningTime_ms = 1000;
#else
    const int delayAfterFinished_ms = 3000;
    const int listeningTime_ms = 5000;
#endif
    
    
    
    
    
    
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    private :
    
    // init at construction only
    OSCRecorderComponent& recorderComponent;
    
    ExperimentState state = ExperimentState::NotInitialized;
    
    /// \brief Les indexes -1 et -2 correspondent au "tours d'essais" qui ne seront pas comptabilité
    int currentPresetIndex = -2;
    const size_t presetsCount = 2;
    std::vector<size_t> presetsList; // a preset is represented by a size_t integer
    
    
    // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
    
    
    
    
    // = = = = = = = = = = METHODS = = = = = = = = = =
    public :
    OSCRecorder(OSCRecorderComponent& _component);
    
    // Internal management
    protected :
    void changeState(ExperimentState newState);
    
    
    // OSC control of Reaper and MIEM Controller
    protected :
    void selectNewScene();
    
    
    public :
    
    // Events froms View
    void OnButtonClicked(ExperimentState requestedState);
    
    // will check for double-strokes (3s delay between 2 key downs ?)
    void OnSpaceBarPushed();
    
    
};
