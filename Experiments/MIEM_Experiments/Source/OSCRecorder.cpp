/*
  ==============================================================================

    OSCRecorder.cpp
    Created: 7 Feb 2019 9:09:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "OSCRecorder.h"
#include "OSCRecorderComponent.h"

// The constructor will init the experiment with random preset values
OSCRecorder::OSCRecorder(OSCRecorderComponent& _component)
: recorderComponent(_component)
{
    recorderComponent.SetRecorderManager(this);
    
    // Random presets are planned from the beginning
    // We must have all 20 indexes in the vector
    presetsList.resize(presetsCount);
    
    // LES SCENES DEVRONT AUSSI ETRE ALEATOIRES
    // si je veux vraiment que l'expé ne soit pas biaisée...
    
    
    // Prêt dès la fin de la construction
    changeState(ExperimentState::ReadyToListen);
}



void OSCRecorder::changeState(ExperimentState newState)
{
    // Pre-change processing
    // OSC control messages are sent here (as soon as possible)
    switch(newState)
    {
        case ExperimentState::NotInitialized:
            break;
            
        case ExperimentState::ReadyToListen:
            break;
            
        case ExperimentState::Listening:
            break;
            
        case ExperimentState::ReadyToSearchPreset:
            break;
            
        case ExperimentState::SearchingPreset:
            selectNewScene();
            break;
            
        case ExperimentState::FinishedSearchingPreset:
            if (currentPresetIndex == (presetsCount - 1))
                newState = ExperimentState::Finished;
            break;
            
        case ExperimentState::Finished:
            break;
    }
    
    // Actual change of state
    state = newState;
    
    // Display updates
    std::cout << OSCRecorderComponent::GetExperimentStateName(state) << " #" << currentPresetIndex << std::endl;
    recorderComponent.DisplayNewState(state, currentPresetIndex, presetsCount);
    
    // Post-updates processing
    if (state == ExperimentState::FinishedSearchingPreset)
    {
        currentPresetIndex++;
        Timer::callAfterDelay(delayAfterFinished_ms,
                              [this] {
                                  changeState(ExperimentState::ReadyToListen);
                              });
    }
    else if (state == ExperimentState::Listening)
        Timer::callAfterDelay(listeningTime_ms,
                              [this] {
                                  changeState(ExperimentState::ReadyToSearchPreset);
                              });
}

void OSCRecorder::selectNewScene()
{
    std::cout << "DEMANDE OSC DE LA SCENE INDEX #" << currentPresetIndex << std::endl;
}



// ================== Events froms View =============================

void OSCRecorder::OnButtonClicked(ExperimentState requestedState)
{
    switch(requestedState)
    {
        case ExperimentState::NotInitialized:
            assert(false); // does not correspond to a button
            break;
            
        case ExperimentState::ReadyToListen:
            assert(false); // does not correspond to a button
            break;
            
        case ExperimentState::Listening:
            changeState(requestedState);
            break;
            
        case ExperimentState::ReadyToSearchPreset:
            assert(false); // does not correspond to a button
            break;
            
        case ExperimentState::SearchingPreset:
            changeState(requestedState);
            break;
            
        case ExperimentState::FinishedSearchingPreset:
            changeState(requestedState);
            break;
            
        case ExperimentState::Finished:
            assert(false);
            break;
    }
}


void OSCRecorder::OnSpaceBarPushed()
{
    
}
