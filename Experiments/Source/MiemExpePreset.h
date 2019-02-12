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
#include <memory>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;



struct MiemSample {
    int time_ms;
    int parameterIndex;
    float value;
};
typedef struct MiemSample MiemSample;


class MiemExpePreset {
    
    
    // =============== ATTRIBUTES ================
    private :
    
    // - - - - - - - - - - Actual Preset Data - - - - - - - - - -
    const int UID; ///< Universal ID
    int appearanceIndex; ///< Random index <-> order within the experiment. Negative values for trial presets
    
    /// \brief Did the experiment encoutered issues during the recording of this preset ?
    bool isValid = true;
    
    std::vector<MiemSample> samples;

    
    
    // =============== Setters and Getters ================
    public :
    void SetIsValid(bool _isValid) {isValid = _isValid;}
    void SetAppearanceIndex(int _randomizedIndex) {appearanceIndex = _randomizedIndex;}
    
    int GetUID() const {return UID;}
    
    
    
    
    // =============== METHODS ================
    public :
    /// \brief Constructor will build the presets' parameters (scene number, parameter default values, ...),
    /// event from trial presets (negative UIDs)
    MiemExpePreset(int _UID);
    
    static std::map<int, size_t> GeneratePresetIndexToRandomIndexMap(int actualPresetsCount,
                                                                     int trialPresetsCount);
    
    std::shared_ptr<bptree::ptree> GetInfoTree();
};
