/*
  ==============================================================================

    MiemExpePreset.h
    Created: 9 Feb 2019 8:44:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <random>


class MiemExpePreset {
    
    // =============== ATTRIBUTES ================
    private :
    
    const int UID; ///< Universal ID
    
    int randomizedIndex; ///< Random index <-> order within the experiment. Negative values for trial presets
    
    
    
    /// \brief Did the experiment encoutered issues during the recording of this preset ?
    bool isValid = true;
    

    
    // =============== Setters and Getters ================
    public :
    void SetIsValid(bool _isValid) {isValid = _isValid;}
    void SetRandomizedIndex(int _randomizedIndex) {randomizedIndex = _randomizedIndex;}
    
    int GetUID() const {return UID;}
    
    // =============== METHODS ================
    public :
    /// \brief Constructor will build the presets' parameters (scene number, parameter default values, ...),
    /// event from trial presets (negative UIDs)
    MiemExpePreset(int _UID);
    
    static std::map<int, size_t> GeneratePresetIndexToRandomIndexMap(int actualPresetsCount,
                                                                     int trialPresetsCount);
    
};
