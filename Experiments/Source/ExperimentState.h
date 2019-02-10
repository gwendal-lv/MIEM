/*
  ==============================================================================

    ExperimentState.h
    Created: 7 Feb 2019 9:26:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <string>

#include "JuceHeader.h"

enum class ExperimentState {
    
    
    // States of the actual subject-bsaed experiment :
    
    FirstExperimentState = 0,
    
    ReadyToListen,
    
    Listening,
    
    ReadyToSearchPreset,
    
    SearchingPreset,
    FinishedSearchingPreset,
    
    ExperimentStatesCount, ///< automatically contains the actual number of experiment states
    
    
    Finished, ///< Experiment is finished (including the last questions)
    
    
    // Special states
    NotInitialized,
    
    
    // Before displaying the first questions or screen
    WaitingForTcpServerConnection,
    ConnectionLost, ///< Error intermediate state
    
    
    // First and final states (questions)
    InitialQuestionsDisplayed,
    FinalQuestionsDisplayed,
    
};


class ExperimentStateUtils {
    
    public :
    /// \brief indicate wether the state is an "actual interactive experiment" state,
    /// or not. If not, it can be a pre- or post- experiment state, or any other special state.
    static bool IsInteractiveExperimentState(ExperimentState state)
    {
        const int intState = static_cast<int>(state);
        const int intFirstState = static_cast<int>(ExperimentState::FirstExperimentState);
        const int intLastState = intFirstState + static_cast<int>(ExperimentState::ExperimentStatesCount) - 1;
        return ((intFirstState <= intState) && (intState <= intLastState));
    }
    
    static std::string GetName(ExperimentState state)
    {
        switch (state)
        {
            case ExperimentState::NotInitialized:
                return "Not initialized";
                
            case ExperimentState::InitialQuestionsDisplayed:
                return "Initial questions displayed";
                
            case ExperimentState::FinalQuestionsDisplayed:
                return "Final questions displayed";
                
            case ExperimentState::ReadyToListen:
                return "Ready to listen";
                
            case ExperimentState::Listening:
                return "Listening";
                
            case ExperimentState::ReadyToSearchPreset:
                return "Ready to search preset";
                
            case ExperimentState::SearchingPreset :
                return "Searching preset";
                
            case ExperimentState::FinishedSearchingPreset :
                return "Finished searching preset";
                
            case ExperimentState::Finished :
                return "Finished";
                
            case ExperimentState::WaitingForTcpServerConnection:
                return "Waiting for TCP server connection";
            
            case ExperimentState::ConnectionLost:
                return "TCP connection lost";
                
            case ExperimentState::FirstExperimentState:
            case ExperimentState::ExperimentStatesCount:
                return "[[[internal utility state]]]";
        }
    }
    
};

