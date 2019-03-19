/*
  ==============================================================================

    MiemExpePreset.cpp
    Created: 9 Feb 2019 8:44:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <algorithm>

#include "boost/lexical_cast.hpp"

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

MiemExpePreset::MiemExpePreset(int _synthId, bool _findFromInterpolation) :
synthId(_synthId),
findFromInterpolation(_findFromInterpolation),
parametersCount(4) // const at the moment
{
    parametersTargetValues.resize(parametersCount, -1.0);
    
    if (synthId > 9  || synthId < -2)
    {
        throw std::runtime_error("Synths IDs prévus actuellement de -2 à +9");
    }
    /// - - - - MIEM SCENE INDEX and TEMPO and TARGET VALUES - - - -
    /// - - - - MIEM SCENE INDEX and TEMPO and TARGET VALUES - - - -
    /// - - - - MIEM SCENE INDEX and TEMPO and TARGET VALUES - - - -
    sceneBaseIndex = -1000;
    if (synthId == -2)
    {
        name = "[T] Wurli"; // midrange
        sceneBaseIndex = 1;
        tempo = 145.0f;
        parametersInfo = "1-Chorus/2-Delay/3-250Hz/4-Hammer";
        parametersTargetValues[0] = 0.035;
        parametersTargetValues[1] = 0.215; // un peu de delay
        parametersTargetValues[2] = 0.091;
        parametersTargetValues[3] = 0.659; // et beaucoup de hammer
    }
    else if (synthId == -1)
    {
        name = "[T] Arpesque"; // Arpesque
        sceneBaseIndex = 1;
        tempo = 140.0f;
        parametersInfo = "1-Reso/2-HPFcut/3-attack/4-release";
        parametersTargetValues[0] = 0.764;
        parametersTargetValues[1] = 0.0;
        parametersTargetValues[2] = 0.236;
        parametersTargetValues[3] = 0.0;
    }
    else if (synthId == 0)
    {
        name = "[0] ThunderBass"; // thunder bass avec VCO1mix à 0,204
        sceneBaseIndex = 5;
        tempo = 105.0f;
        parametersInfo = "1-Delay/2-Env/3-Noise/4-Chorus";
        parametersTargetValues[0] = 0.0;
        parametersTargetValues[1] = 0.0;
        parametersTargetValues[2] = 0.214;
        parametersTargetValues[3] = 0.536;
    }
    else if (synthId == 1)
    {
        name = "[1] Shamisen"; // 6-string shamisen (avec algo=0)
        sceneBaseIndex = 17;
        tempo = 145.0f;
        parametersInfo = "1-op2lvl/2-Chorus/3-delay/4-op3lvl";
        parametersTargetValues[0] = 0.706;
        parametersTargetValues[1] = 0.250; // macro 4
        parametersTargetValues[2] = 0.544; // macro 3
        parametersTargetValues[3] = 0.250;
    }
    else if (synthId == 2)
    {
        name = "[2] DabDat";
        sceneBaseIndex = 1;
        tempo = 96.0f;
        parametersInfo = "1-lvlOsc1/2-lvlOsc2/3-part2pan/4-hatCutoff";
        parametersTargetValues[0] = 0.0;
        parametersTargetValues[1] = 0.0;
        parametersTargetValues[2] = 0.239;
        parametersTargetValues[3] = 0.741;
    }
    else if (synthId == 3)
    {
        name = "[3] TriOsc"; // 3 osc
        sceneBaseIndex = 21;
        tempo = 92.0f;
        parametersInfo = "1-cutoff/2-emphasis/3-noise/4-VCAdec";
        parametersTargetValues[0] = 0.542;
        parametersTargetValues[1] = 0.250;
        parametersTargetValues[2] = 0.250;
        parametersTargetValues[3] = 0.458;
    }
    else if (synthId == 4)
    {
        name = "[4] Hypno"; // Hypno Play
        sceneBaseIndex = 21;
        tempo = 90.0f;
        parametersInfo = "1-ChanAlvl/2-Decay/3-reverb/4-chanBlvl";
        parametersTargetValues[0] = 0.273;
        parametersTargetValues[1] = 0.601;
        parametersTargetValues[2] = 0.317;
        parametersTargetValues[3] = 0.308;
    }
    else if (synthId == 5)
    {
        name = "[5] MoodySky"; // Moody Sky
        sceneBaseIndex = 17;
        tempo = 90.0f;
        parametersInfo = "1-Delay/2-Spread/3-Time/4-Timbre";
        parametersTargetValues[0] = 0.250;
        parametersTargetValues[1] = 1.0;
        parametersTargetValues[2] = 0.250;
        parametersTargetValues[3] = 0.250;
    }
    else if (synthId == 6)
    {
        name = "[6] Piano"; // Japanese Jazz Studio
        sceneBaseIndex = 1;
        tempo = 113.0f;
        parametersInfo = "1-HamPos/2-SoundRes/3-HamHard/4-Mic1Pan";
        parametersTargetValues[0] = 1.0;
        parametersTargetValues[1] = 0.0;
        parametersTargetValues[2] = 0.0;
        parametersTargetValues[3] = 0.0;
    }
    else if (synthId == 7)
    {
        name = "[7] DX7block"; // Rom2A 30-BLOCK (DX7) + reaDelay
        sceneBaseIndex = 21;
        tempo = 128.0f;
        parametersInfo = "1-delay/2-macro3/3-macro4/4-pan";
        parametersTargetValues[0] = 0.250; //reaDelay
        parametersTargetValues[1] = 0.413;
        parametersTargetValues[2] = 0.587; 
        parametersTargetValues[3] = 0.250; //reaDelay
    }
    else if (synthId == 8)
    {
        name = "[8] SubGamelan"; // suburban gamelan
        sceneBaseIndex = 17;
        tempo = 120.0f;
        parametersInfo = "1-L/2-R/3-macro1/4-macro2";
        parametersTargetValues[0] = 0.895; // op1 out lvl
        parametersTargetValues[1] = 0.250; // op2 out lvl
        parametersTargetValues[2] = 0.355;
        parametersTargetValues[3] = 0.250;
    }
    else if (synthId == 9)
    {
        name = "[9] AntiqueStr"; // Antique Strings
        sceneBaseIndex = 1;
        tempo = 100.0f;
        parametersInfo = "1-Chorus/2-noise/3-cutoff/4-attack";
        parametersTargetValues[0] = 0.650;
        parametersTargetValues[1] = 0.0;
        parametersTargetValues[2] = 0.350;
        parametersTargetValues[3] = 0.0;
    }
    /// - - - - MIEM SCENE INDEX and TEMPO and TARGET VALUES - - - -
    /// - - - - MIEM SCENE INDEX and TEMPO and TARGET VALUES - - - -
    /// - - - - MIEM SCENE INDEX and TEMPO and TARGET VALUES - - - -
    else
        assert(false); // index must be found in the previous cases
        
    // Valeurs déduites du scene index
    /// ==== Index de base des scènes =====
    /// ==== Index de base des scènes =====
    /// ==== Index de base des scènes =====
    ///
    ///  1 : valeurs allant de 0,00 à 1,00   (états MIEM +0  : de 1  à 5 )  (largeur 1.00)
    ///  5 : valeurs allant de 0,00 à 0,75   (états MIEM +5  : de 6  à 10)  (largeur 0.75)
    ///  9 : valeurs allant de 0,00 à 0,50   (états MIEM +10 : de 11 à 15)  (largeur 0.50)
    /// 13 : valeurs allant de 0,50 à 1,00   (états MIEM +15 : de 16 à 24)  (largeur 0.50)
    /// 17 : valeurs allant de 0,25 à 1,00   (états MIEM +24 : de 25 à 33)  (largeur 0.75)
    /// 21 : valeurs allant de 0,25 à 0,75   (états MIEM +33 : de 34 à 42)  (largeur 0.50)
    ///
    /// ==== Index de base des scènes =====
    /// ==== Index de base des scènes =====
    /// ==== Index de base des scènes =====

    switch (sceneBaseIndex)
    {
        case 1:
            parametersMin = 0.0;
            parametersMax = 1.0;
            break;
            
        case 5:
            parametersMin = 0.0;
            parametersMax = 0.75;
            break;
            
        case 9:
            parametersMin = 0.0;
            parametersMax = 0.50;
            break;
            
        case 13:
            parametersMin = 0.50;
            parametersMax = 1.0;
            break;
            
        case 17:
            parametersMin = 0.25;
            parametersMax = 1.0;
            break;
            
        case 21:
            parametersMin = 0.25;
            parametersMax = 0.75;
            break;
            
        default:
            assert(false); // should not happen...
            parametersMin = -1.0; // to trigger bugs
            parametersMax = -1.0; // to trigger bugs
    }
}




int MiemExpePreset::trialSynthsCount = 0;
std::map<int, size_t> MiemExpePreset::GeneratePresetIndexToRandomIndexMap(int actualPresetsCount,
                                                                 int trialPresetsCount,
                                                                          bool randomize)
{
    // static assignement
    trialSynthsCount = trialPresetsCount;
    
    // There must be the same number of fader presets and of MIEM interpolation presets
    assert(((trialPresetsCount % 2) == 0) && ((actualPresetsCount % 2) == 0));
    
    std::map<int, size_t> presetRandomIdx;
    
    // values insertion using []
    // NO randomization for trial presets at the end of the vector
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
    
    // sometimes not randomized, for debug purposes
    if (randomize)
        std::shuffle(randomVector1.begin(), randomVector1.end(), std::mt19937(timeSeed));
    
    for (int i=0 ; i<actualPresetsCount ; i++)
            presetRandomIdx[i] = randomVector1[i];
    
    return presetRandomIdx;
}




int MiemExpePreset::GetUID() const
{
    if (findFromInterpolation)
        return synthId + 10;
    else
        return synthId;
}

int MiemExpePreset::GetMiemSceneIndex() const
{
    if (findFromInterpolation)
        return sceneBaseIndex + 1;
    else
        return sceneBaseIndex;
}


std::shared_ptr<bptree::ptree> MiemExpePreset::GetInfoTree()
{
    auto preseTree = std::make_shared<bptree::ptree>(); // lol
    
    // UID and display order
    preseTree->put("<xmlattr>.synth_id", synthId);
    preseTree->put("<xmlattr>.name", name);
    preseTree->put("<xmlattr>.from_interpolation", findFromInterpolation);
    preseTree->put("<xmlattr>.appearance_index", appearanceIndex);
    preseTree->put("<xmlattr>.is_valid", isValid);
    preseTree->put("<xmlattr>.samples_count", samples.size());
    
    return preseTree;
}
std::shared_ptr<bptree::ptree> MiemExpePreset::GetTargetValuesInnerTree()
{
    auto targetValuesInnerTree = std::make_shared<bptree::ptree>();
    targetValuesInnerTree->put("bounds.<xmlattr>.min", parametersMin);
    targetValuesInnerTree->put("bounds.<xmlattr>.max", parametersMax);
    targetValuesInnerTree->put("bounds.<xmlattr>.scene_base_index", sceneBaseIndex);
    for (size_t i=0 ; i<parametersTargetValues.size() ; i++)
    {
        bptree::ptree paramInnerTree;
        paramInnerTree.put("<xmlattr>.id", i+1); // IDs from 1
        paramInnerTree.put("<xmlattr>.target_value", parametersTargetValues[i]);
        targetValuesInnerTree->add_child("parameter", paramInnerTree);
    }
    return targetValuesInnerTree;
}
std::string MiemExpePreset::GetCSVFileHeader()
{
    return "synth_id;from_interpolation;parameter;time;value\n";
}
std::shared_ptr<std::string> MiemExpePreset::GetSortedSamples_CSV()
{
    auto returnStr = std::make_shared<std::string>();
    
    std::string synthIdAndInterpolationStr = boost::lexical_cast<std::string>(synthId);
    synthIdAndInterpolationStr += ";";
    synthIdAndInterpolationStr += boost::lexical_cast<std::string>((int)(findFromInterpolation));
    synthIdAndInterpolationStr += ";";
    for (size_t i=0 ; i<sortedSamples.size() ; i++)
    {
        *returnStr += synthIdAndInterpolationStr;
        *returnStr += boost::lexical_cast<std::string>(sortedSamples[i].parameterIndex);
        *returnStr += ";";
        *returnStr += boost::lexical_cast<std::string>(sortedSamples[i].time_ms);
        *returnStr += ";";
        *returnStr += boost::lexical_cast<std::string>(sortedSamples[i].value);
        *returnStr += "\n";
    }
    
    return returnStr;
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





// ======================== Samples management ============================
void MiemExpePreset::AddSamples(const std::vector<MiemSample<float>> & newSamples)
{
    size_t firstAdditionIndex = samples.size();
    
    // avec mise à zéro des nouvelles samples
    samples.resize(samples.size() + newSamples.size());
    
    for (size_t i=0 ; i<newSamples.size() ; i++)
    {
        samples[firstAdditionIndex + i] = newSamples[i];
    }
}


void MiemExpePreset::SortSamples()
{
    // ultra-pas-optimisé !
    // tri à la main à l'ancienne... avec des insertions/suppressions sauvages
    // (du coup : copie quand même dans une liste pour insertions/suppressions rapides
    std::list<MiemSample<float>> copiedList(samples.begin(), samples.end());
    
    // recopie dans un vecteur au fur et à mesure
    sortedSamples.clear();
    sortedSamples.reserve(copiedList.size());
    for (int i=1 ; i<= parametersCount ; i++)
    {
        // On parcourt la liste jusqu'à la fin
        auto it = copiedList.begin();
        while (it != copiedList.end())
        {
            if (it->parameterIndex == i)
            {
                sortedSamples.push_back((*it));
                // si suppression, il faut récupérer un itérateur valide !
                it = copiedList.erase(it);
                // qui sera re-testé directement après pour la prochaine itération
            }
            else
            {
                // si pas de suppression... on incrémente
                it++;
            }
        }
    }
    
    if (copiedList.size() > 0)
        assert(false); // on était censé vider la liste heeeeiiiiiin
    
    //std::copy(std::begin(copiedList), std::end(copiedList), std::back_inserter(sortedSamples));
    
    //DisplayInStdCout(true);
}


void MiemExpePreset::DisplayInStdCout(bool displaySortedSamples)
{
    if (! displaySortedSamples)
    {
        std::cout << "------------ Preset " << synthId << "/" << findFromInterpolation << " --------------" << std::endl;
        for (size_t i=0 ; i<samples.size() ; i++)
        {
            std::cout << samples[i].time_ms << " ; " << samples[i].parameterIndex << " ; " << samples[i].value << std::endl;
        }
    }
    else
    {
        std::cout << "------------ Preset " << synthId << "/" << findFromInterpolation << " SORTED --------------" << std::endl;
        for (size_t i=0 ; i<sortedSamples.size() ; i++)
        {
            std::cout << sortedSamples[i].time_ms << " ; " << sortedSamples[i].parameterIndex << " ; " << sortedSamples[i].value << std::endl;
        }
    }
}
