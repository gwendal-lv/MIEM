/*
  ==============================================================================

    MiemExpePreset.cpp
    Created: 9 Feb 2019 8:44:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MiemExpePreset.h"

#include "JuceHeader.h"


/// ============= Liste de presets =============
/// ============= Liste de presets =============
/// ============= Liste de presets =============
/// ============= Liste de presets =============
///
///
/// Synth ID : de 0 à 9. Représente un type de
/// synthétiseur dans Reaper
///
/// UIDs : extraits depuis les synth IDs
/// Pour les faders : UID = synthID
/// Pour les interpolations : UID = synthID + 10
///
///
///
/// findFromInterpolation : si false, alors on va cherche le preset
/// depuis des faders séparés. Si True, alors on teste avec la
/// grosse interpolation MIEM.
///
///
///
/// ============= Liste de presets =============
/// ============= Liste de presets =============
/// ============= Liste de presets =============
/// ============= Liste de presets =============


std::map<int, size_t> MiemExpePreset::GeneratePresetIndexToRandomIndexMap(int actualPresetsCount,
                                                                 int trialPresetsCount)
{
    // There must be the same number of fader presets and of MIEM interpolation presets
    assert(((trialPresetsCount % 2) == 0) && ((actualPresetsCount % 2) == 0));
    
    std::map<int, size_t> presetRandomIdx;
    
    // values insertion using []
    // NO randomization for trial presets
    if (trialPresetsCount != (trialSynthsCount*2))
        throw std::logic_error("Impossible de gérer ça. Corriger et mettre de la cohérence dans le nombre de presets d'essai !");
    
    for (int i=0 ; i<trialPresetsCount ; i++)
        presetRandomIdx[-(int)(trialPresetsCount) + i] = actualPresetsCount + i;
    
    // Randomization from a single vector containing the 10 presets
    // On ne sait pas, pour chacun des 10, si c'est un fader ou un interpolation
    // (c'est aléatoire au démarrage)
    std::vector<int> randomVector1; //  for classical presets
    for (int i=0 ; i<actualPresetsCount ; i++)
        randomVector1.push_back(i);
    // mersenne-twister 1997 (32 bits)
    auto longIntSeed = Time::currentTimeMillis() % 1000000;
    int timeSeed = (int) longIntSeed;
    std::shuffle(randomVector1.begin(), randomVector1.end(), std::mt19937(timeSeed));
    for (int i=0 ; i<actualPresetsCount ; i++)
            presetRandomIdx[i] = randomVector1[i];
    
    return presetRandomIdx;
}


MiemExpePreset::MiemExpePreset(int _synthId, bool _findFromInterpolation) :
synthId(_synthId),
findFromInterpolation(_findFromInterpolation)
{
    if (synthId > 9  || synthId < -1)
    {
        throw std::runtime_error("Synths IDs prévus actuellement de -2 à +9");
    }
    /// - - - - MIEM SCENE INDEX A CHOISIR ICI - - - -
    /// - - - - MIEM SCENE INDEX A CHOISIR ICI - - - -
    /// - - - - MIEM SCENE INDEX A CHOISIR ICI - - - -
    int sceneBaseIndex = -1000;
    if (synthId == -2)
        sceneBaseIndex = 1;
    else if (synthId == -1)
        sceneBaseIndex = 1;
    else if (synthId == 0)
        sceneBaseIndex = 1;
    else if (synthId == 1)
        sceneBaseIndex = 1;
    else if (synthId == 2)
        sceneBaseIndex = 1;
    else if (synthId == 3)
        sceneBaseIndex = 1;
    else if (synthId == 4)
        sceneBaseIndex = 1;
    else if (synthId == 5)
        sceneBaseIndex = 1;
    else if (synthId == 6)
        sceneBaseIndex = 1;
    else if (synthId == 7)
        sceneBaseIndex = 1;
    else if (synthId == 8)
        sceneBaseIndex = 1;
    else if (synthId == 9)
        sceneBaseIndex = 1;
    /// - - - - MIEM SCENE INDEX A CHOISIR ICI - - - -
    /// - - - - MIEM SCENE INDEX A CHOISIR ICI - - - -
    /// - - - - MIEM SCENE INDEX A CHOISIR ICI - - - -
}


int MiemExpePreset::GetUID() const
{
    if (findFromInterpolation)
        return synthId + 10;
    else
        return synthId;
}


std::shared_ptr<bptree::ptree> MiemExpePreset::GetInfoTree()
{
    auto preseTree = std::make_shared<bptree::ptree>(); // lol
    
    // UID and display order
    preseTree->put("<xmlattr>.synth_id", synthId);
    preseTree->put("<xmlattr>.from_interpolation", findFromInterpolation);
    preseTree->put("<xmlattr>.appearance_index", appearanceIndex);
    preseTree->put("<xmlattr>.is_valid", isValid);
    preseTree->put("<xmlattr>.samples_count", samples.size());
    
    return preseTree;
}

int MiemExpePreset::GetReaperTrackNumber(bool getReferenceTrack)
{
    // les premières tracks dans Reaper sont faites comme suit :
    // 1 - Preset ESSAI référence
    // 2 - preset ESSAI contrôlable en OSC (par fader ou par interpolation)
    // 3 - Preset1 référence
    // 4 - Preset1 contrôlable en OSC (par fader ou par interpolation)
    // 5 - Preset1 référence
    // 6 - Preset1 contrôlable en OSC (par fader ou par interpolation)
    // etc...
    
    // Pour 10 presets + 2 presets d'essai...
    // Il faudra 24 tracks dans Reaper....
    
    if (getReferenceTrack)
        return 1 + (synthId + trialSynthsCount) * 2;
    else
        return 2 + (synthId + trialSynthsCount) * 2;
}
