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
    
    /// \brief ID du synth dans Reaper qui sera joué pour ce preset. Négatif pour
    /// les synthés de test
    static const int trialSynthsCount = 1;
    const int synthId;
    const bool findFromInterpolation; ///< Type de moyen de recherche du preset
    int miemSceneIndex; ///< index de la scène MIEM Controller pour ce synthé
    
    int appearanceIndex; ///< Random index <-> order within the experiment. Negative values for trial presets
    
    /// \brief Did the experiment encoutered issues during the recording of this preset ?
    bool isValid = true;
    
    std::vector<MiemSample> samples;

    
    
    // =============== Setters and Getters ================
    public :
    void SetIsValid(bool _isValid) {isValid = _isValid;}
    void SetAppearanceIndex(int _randomizedIndex) {appearanceIndex = _randomizedIndex;}
    
    int GetUID() const;
    int GetMiemSceneIndex() const {return miemSceneIndex;}
    int GetSynthId() const {return synthId;}
    bool GetIsFoundFromInterpolation() const {return findFromInterpolation;}
    
    /// \brief Donne le numéro de track correspond dans Reaper, pour le preset dont on
    /// parle (et selon qu'on demande la track de référence, ou celle de test).
    ///
    /// Si on get la référence track, on n'aura alors pas la contrôlable (qui est la suivante
    /// dans reaper)
    int GetReaperTrackNumber(bool getReferenceTrack);
    
    
    
    // =============== METHODS ================
    public :
    /// \brief Constructor will build the presets' parameters (scene number, parameter default values, ...),
    /// event from trial presets (negative UIDs)
    MiemExpePreset(int _synthId, bool _findFromInterpolation);
    
    
    std::shared_ptr<bptree::ptree> GetInfoTree();
    
    
    // - - - - - Generic presets indexes manipulations - - - - -
    static std::map<int, size_t> GeneratePresetIndexToRandomIndexMap(int actualPresetsCount,
                                                                     int trialPresetsCount);
    
};
