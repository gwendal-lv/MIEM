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

/// ==== Index de base des scènes =====
/// ==== Index de base des scènes =====
/// ==== Index de base des scènes =====
///
///  1 : valeurs allant de 0,00 à 1,00   (états MIEM +0 :  de 1 à 5)    (largeur 1.00)
///  5 : valeurs allant de 0,50 à 1,00   (états MIEM +5 :  de 6 à 10)   (largeur 0.50)
///  9 : valeurs allant de 0,25 à 1,00   (états MIEM +10 : de 11 à 15)  (largeur 0.75)
/// 13 : valeurs allant de 0,00 à 0,75   (états MIEM +15 : de 16 à 20)  (largeur 0.75)
/// 17 : valeurs allant de 0,00 à 0,50   (états MIEM +20 : de 21 à 25)  (largeur 0.50)
/// 21 : valeurs allant de 0,25 à 0,75   (états MIEM +25 : de 26 à 30)  (largeur 0.50)
///
/// ==== Index de base des scènes =====
/// ==== Index de base des scènes =====
/// ==== Index de base des scènes =====

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
    if (synthId == -1)
    {
        name = "[T] Wurli hammer+delay"; // original Analog lab 3 : midrange
        sceneBaseIndex = 1;
        tempo = 145.0f;
        parametersInfo = "1-Chorus/2-Delay/3-250Hz/4-Hammer";
        parametersTargetValues[0] = 0.065;
        parametersTargetValues[1] = 0.207; // un peu de delay
        parametersTargetValues[2] = 0.064;
        parametersTargetValues[3] = 0.664; // et beaucoup de hammer
    }
    else if (synthId == 0)
    {
        name = "[0] ThunderBass"; // thunder bass
        sceneBaseIndex = 1;
        tempo = 105.0f;
        parametersInfo = "1-Delay/2-Osc1Mix/3-Noise/4-Chorus";
        parametersTargetValues[0] = 0.1;
        parametersTargetValues[1] = 0.2;
        parametersTargetValues[2] = 0.3;
        parametersTargetValues[3] = 0.4;
    }
    else if (synthId == 1)
    {
        name = "Synth #1";
        sceneBaseIndex = 1;
        tempo = 120.0f;
        parametersInfo = "1-/2-/3-/4-";
        parametersTargetValues[0] = 0.5;
        parametersTargetValues[1] = 0.5;
        parametersTargetValues[2] = 0.5;
        parametersTargetValues[3] = 0.5;
    }
    else if (synthId == 2)
    {
        name = "Synth #2";
        sceneBaseIndex = 1;
        tempo = 120.0f;
        parametersInfo = "1-/2-/3-/4-";
        parametersTargetValues[0] = 0.5;
        parametersTargetValues[1] = 0.5;
        parametersTargetValues[2] = 0.5;
        parametersTargetValues[3] = 0.5;
    }
    else if (synthId == 3)
    {
        name = "Synth #3";
        sceneBaseIndex = 1;
        tempo = 120.0f;
        parametersTargetValues[0] = 0.5;
        parametersTargetValues[1] = 0.5;
        parametersTargetValues[2] = 0.5;
        parametersTargetValues[3] = 0.5;
    }
    else if (synthId == 4)
    {
        name = "Synth #4";
        sceneBaseIndex = 1;
        tempo = 120.0f;
        parametersTargetValues[0] = 0.5;
        parametersTargetValues[1] = 0.5;
        parametersTargetValues[2] = 0.5;
        parametersTargetValues[3] = 0.5;
    }
    else if (synthId == 5)
    {
        name = "Synth #5";
        sceneBaseIndex = 1;
        tempo = 120.0f;
        parametersTargetValues[0] = 0.5;
        parametersTargetValues[1] = 0.5;
        parametersTargetValues[2] = 0.5;
        parametersTargetValues[3] = 0.5;
    }
    else if (synthId == 6)
    {
        name = "Synth #6";
        sceneBaseIndex = 1;
        tempo = 120.0f;
        parametersTargetValues[0] = 0.5;
        parametersTargetValues[1] = 0.5;
        parametersTargetValues[2] = 0.5;
        parametersTargetValues[3] = 0.5;
    }
    else if (synthId == 7)
    {
        name = "Synth #7";
        sceneBaseIndex = 1;
        tempo = 120.0f;
        parametersTargetValues[0] = 0.5;
        parametersTargetValues[1] = 0.5;
        parametersTargetValues[2] = 0.5;
        parametersTargetValues[3] = 0.5;
    }
    else if (synthId == 8)
    {
        name = "Synth #8";
        sceneBaseIndex = 1;
        tempo = 120.0f;
        parametersTargetValues[0] = 0.5;
        parametersTargetValues[1] = 0.5;
        parametersTargetValues[2] = 0.5;
        parametersTargetValues[3] = 0.5;
    }
    else if (synthId == 9)
    {
        name = "Synth #9";
        sceneBaseIndex = 1;
        tempo = 120.0f;
        parametersTargetValues[0] = 0.5;
        parametersTargetValues[1] = 0.5;
        parametersTargetValues[2] = 0.5;
        parametersTargetValues[3] = 0.5;
    }
    /// - - - - MIEM SCENE INDEX and TEMPO and TARGET VALUES - - - -
    /// - - - - MIEM SCENE INDEX and TEMPO and TARGET VALUES - - - -
    /// - - - - MIEM SCENE INDEX and TEMPO and TARGET VALUES - - - -
    else
        assert(false); // index must be found in the previous cases
        
    // Valeurs déduites du scene index (texte ci-dessous, RECOPIE DEPUIS LA SOURCE
    // AU DEBUT DE CE FICHIER)
    ///  1 : valeurs allant de 0,00 à 1,00   (états MIEM +0 :  de 1 à 5)    (largeur 1.00)
    ///  5 : valeurs allant de 0,50 à 1,00   (états MIEM +5 :  de 6 à 10)   (largeur 0.50)
    ///  9 : valeurs allant de 0,25 à 1,00   (états MIEM +10 : de 11 à 15)  (largeur 0.75)
    /// 13 : valeurs allant de 0,00 à 0,75   (états MIEM +15 : de 16 à 20)  (largeur 0.75)
    /// 17 : valeurs allant de 0,00 à 0,50   (états MIEM +20 : de 21 à 25)  (largeur 0.50)
    /// 21 : valeurs allant de 0,25 à 0,75   (états MIEM +25 : de 26 à 30)  (largeur 0.50)
    switch (sceneBaseIndex)
    {
        case 1:
            parametersSpan = 1.00;
            break;
        case 9:
        case 13:
            parametersSpan = 0.75;
            break;
        case 5:
        case 17:
        case 21:
            parametersSpan = 0.50;
            break;
            
        default:
            assert(false); // should not happen...
            parametersSpan = 0.0; // to trigger bugs
    }
}





std::map<int, size_t> MiemExpePreset::GeneratePresetIndexToRandomIndexMap(int actualPresetsCount,
                                                                 int trialPresetsCount,
                                                                          bool randomize)
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
    preseTree->put("<xmlattr>.parameters_span", parametersSpan);
    
    return preseTree;
}
std::shared_ptr<bptree::ptree> MiemExpePreset::GetTargetValuesInnerTree()
{
    auto targetValuesInnerTree = std::make_shared<bptree::ptree>();
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
void MiemExpePreset::AddSamples(const std::vector<MiemSample> & newSamples)
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
    std::list<MiemSample> copiedList(samples.begin(), samples.end());
    
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
