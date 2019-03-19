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
#include <list>
#include <map>
#include <random>
#include <memory>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;



template<typename ValueType, typename TimeDurationType>
class MiemSample {
    public :
    TimeDurationType time_ms; ///< Time of reception of the sample, in milliseconds since the beginning of experiment.
    int parameterIndex; ///< Index of the parameter concerned by this sample.
    ValueType value; ///< Actual value of the sample.
};
typedef MiemSample<float, int> MiemExpeSample; ///< To store samples recorder during an experiment
typedef MiemSample<int, int64_t> MiemMidiSample; ///< To store samples that are being MIDI-forwarded.


class MiemExpePreset {
    
    
    // =============== ATTRIBUTES ================
    private :
    
    // - - - - - - - - - - Actual Preset Data - - - - - - - - - -
    
    /// \brief ID du synth dans Reaper qui sera joué pour ce preset. Négatif pour
    /// les synthés de test
    const int synthId;
    const bool findFromInterpolation; ///< Type de moyen de recherche du preset
    
    static int trialSynthsCount;
    
    /// - - - - EXPERIMENT VALUE ASSIGNED BY CONSTRUCTOR - - - -
    std::string name;
    std::string parametersInfo;
    const int parametersCount; ///< Parameters indexes count (actual indexes start at 1)
    std::vector<double> parametersTargetValues; ///< Double storage (actual data is float)
    int sceneBaseIndex; ///< index de la scène MIEM Controller pour ce synthé
    float tempo; ///< raw REAPER tempo value
    
    int appearanceIndex; ///< Random index <-> order within the experiment. Negative values for trial presets
    
    /// \brief Did the experiment encoutered issues during the recording of this preset ?
    bool isValid = true;
    
    // - - - - Valeurs déduites des valeurs du constructeur - - - -
    /// \brief Span of all parameters for this particular synth
    // double parametersSpan; // calculable maintenant depuis les valeurs min/max
    double parametersMin;
    double parametersMax;
    
    std::vector<MiemExpeSample> samples;
    std::vector<MiemExpeSample> sortedSamples;

    
    
    // =============== Setters and Getters ================
    public :
    void SetIsValid(bool _isValid) {isValid = _isValid;}
    void SetAppearanceIndex(int _randomizedIndex) {appearanceIndex = _randomizedIndex;}
    
    int GetUID() const;
    int GetMiemSceneIndex() const;
    int GetSynthId() const {return synthId;}
    float GetTempo() const {return tempo;}
    std::string GetName() const {return name;}
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
    std::shared_ptr<bptree::ptree> GetTargetValuesInnerTree();
    
    /// \brief Returns the name of the columns for the CSV file
    /// (including \n)
    static std::string GetCSVFileHeader();
    std::shared_ptr<std::string> GetSortedSamples_CSV();

    // - - - - - Samples management - - - - -
    void AddSamples(const std::vector<MiemExpeSample> & newSamples);
    /// \brief Trie les samples par paramètre (en supposant que
    /// les temps sont parfaitement croissants)
    void SortSamples();
    void DisplayInStdCout(bool displaySortedSamples = false);
    
    // - - - - - Generic presets indexes manipulations - - - - -
    
    /// \brief
    ///
    /// \param randomize can be false for debug purposes
    static std::map<int, size_t> GeneratePresetIndexToRandomIndexMap(int actualPresetsCount,
                                                                     int trialPresetsCount,
                                                                     bool randomize = true);
    
};
