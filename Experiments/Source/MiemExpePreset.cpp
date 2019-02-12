/*
  ==============================================================================

    MiemExpePreset.cpp
    Created: 9 Feb 2019 8:44:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MiemExpePreset.h"

#include "JuceHeader.h"


/// ============= Liste de presets, selon leur UID réel =============
/// ============= Liste de presets, selon leur UID réel =============
/// ============= Liste de presets, selon leur UID réel =============
///
///
///
/// Tout ça sera à bien respecter....
///
/// Presets d'essais : index -2 avec faders, index -1 le même en
/// mode interpolation MIEM
///
/// Ensuite, presets classiques avec les mesures :
///
///        De 0 à 9 on aura les presets avec faders
///
///        De 10 à 19 on aura les mêmes presets (valeurs finales idem)
/// mais retrouvés avec l'interpolation graphique MIEM
///
///
///
/// ============= Liste de presets, selon leur UID réel =============
/// ============= Liste de presets, selon leur UID réel =============
/// ============= Liste de presets, selon leur UID réel =============


std::map<int, size_t> MiemExpePreset::GeneratePresetIndexToRandomIndexMap(int actualPresetsCount,
                                                                 int trialPresetsCount)
{
    // There must be the same number of fader presets and of MIEM interpolation presets
    assert(((trialPresetsCount % 2) == 0) && ((actualPresetsCount % 2) == 0));
    
    std::map<int, size_t> presetRandomIdx;
    
    // values insertion using []
    // NO randomization for trial presets
    for (int i=0 ; i<trialPresetsCount ; i++)
        presetRandomIdx[-(int)(trialPresetsCount) + i] = actualPresetsCount + i;
    
    // Randomization from 2 vectors for beginning
    std::vector<int> randomVector1; //  for classical presets
    for (int i=0 ; i<actualPresetsCount/2 ; i++)
        randomVector1.push_back(i);
    std::vector<int> randomVector2; //  for MIEM-interpolation presets
    for (int i=0 ; i<actualPresetsCount/2 ; i++)
        randomVector2.push_back(i + actualPresetsCount/2);
    // mersenne-twister 1997 (32 bits)
    std::shuffle(randomVector1.begin(), randomVector1.end(), std::mt19937(Time::getMillisecondCounter()));
    std::shuffle(randomVector2.begin(), randomVector2.end(), std::mt19937(Time::getMillisecondCounter()));
    for (int i=0 ; i<actualPresetsCount ; i++)
    {
        // EVEN DISPLAY INDEXES : classical FADER presets
        if ((i%2) == 0)
            presetRandomIdx[i] = randomVector1[i/2];
        // ODD DISPLAY INDEXES : MIEM interpolation presets (each follows a fader preset)
        else
            presetRandomIdx[i] = randomVector2[(i-1)/2];
    }
    
    return presetRandomIdx;
}


MiemExpePreset::MiemExpePreset(int _UID) :
UID(_UID)
{
    if (UID >= 20 || UID < -2)
    {
        throw std::runtime_error("Presets prévus actuellement de -2 à +19");
    }
}


std::shared_ptr<bptree::ptree> MiemExpePreset::GetInfoTree()
{
    auto preseTree = std::make_shared<bptree::ptree>(); // lol
    
    // UID and display order
    preseTree->put("<xmlattr>.UID", UID);
    preseTree->put("<xmlattr>.appearance_index", appearanceIndex);
    preseTree->put("<xmlattr>.is_valid", isValid);
    preseTree->put("<xmlattr>.samples_count", samples.size());
    
    return preseTree;
}
