/*
  ==============================================================================

    Exciter.cpp
    Created: 11 Jan 2017 2:36:45pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Exciter.h"

#include "MultiAreaEvent.h"

#include <cmath>
#include <algorithm>
#include <numeric> // std::accumulate

using namespace Miam;

// = = = = = = = = = = Construction/Destruction + polymorphic cloning = = = = = = = = = =

Exciter::Exciter(bptree::ptree & areaTree, std::chrono::time_point<clock> commonStartTimePoint_)
:
EditableEllipse(areaTree),

commonStartTimePt(commonStartTimePoint_)
{
    init();
    
    // Paramètres facultatifs
    try {
        isAnimationSynchronized = areaTree.get<bool>("is_animation_sync");
    }
    catch (bptree::ptree_error&) { }
    
}

Exciter::Exciter(uint64_t uniqueId, std::chrono::time_point<clock> commonStartTimePoint_)
:
// Taille = 5% du canevas (de la + petite taille). Ratio inconnu, 1 par défaut...
EditableEllipse(uniqueId, bpt(0.5, 0.5), 0.05, Colours::lightgrey, 1.0f),

commonStartTimePt(commonStartTimePoint_)
{
    init();
}

Exciter::~Exciter()
{
    // Dé-exitation des aires qui existent encore
    for (auto& weakArea : areasInteractingWith)
        if (auto area = weakArea.lock())
            area->OnExciterDestruction();
}


void Exciter::init()
{
    // Centre (voir DrawableArea)
    displayCenter = false;
    
    SetNameVisible(false);
    
    SetActive(false);
    SetOpacityMode(OpacityMode::Mid);
    SetEnableTranslationOnly(true);
    
    // Clignotement
    startTimePt = clock::now();
    isAnimationSynchronized = true;
    
    
    // Par défaut : volume de 1
    SetVolume(1.0);
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

void Exciter::SetVolume(double volume_)
{
    volume = volume_;
    
    // Pour l'instant rien, mais à l'avenir ça devra afficher graphiquement le résultat
}

void Exciter::SetIsAnimationSynchronized(bool isSynchronized_)
{
    // On suppose que le point de départ commun a bien été initialisé auparavant...
    
    // Dans tous les cas on sauvegarde l'état
    isAnimationSynchronized = isSynchronized_;
}


// = = = = = = = = = = Display = = = = = = = = = =
void Exciter::Paint(Graphics& g)
{
    std::chrono::duration<double> duration; // par défaut, en secondes
    if (isAnimationSynchronized)
        duration = clock::now() - commonStartTimePt;
    else
        duration = clock::now() - startTimePt;
    
    // Création d'une couleur opaque (alpha=1.0f)
    // à partir de la luminosité seulement (sat et teinte = 0.0f)
    double brightness = deltaBrightnessOffset + deltaBrightnessAmplitude
                        * std::cos(omega * duration.count());
    contourColour = Colour(0.0f, 0.0f, (float)brightness, 1.0f);
    
    // Parent painting
    EditableEllipse::Paint(g);
}


// = = = = = = = = = = Interactions = = = = = = = = = =

void Exciter::OnAreaExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis, double interactionWeight)
{
    size_t areaIndex = findAreaInteractingIndex(areaExcitedByThis);
    // Si l'aire n'existait pas, on l'ajoute
	if (areaIndex == areasInteractingWith.size())
	{
		areasInteractingWith.push_back(areaExcitedByThis);
        areaInteractionWeights.push_back(0.0); // calcul juste après
        areaExcitementAmounts.push_back(Excitement()); // calcul juste après
	}
    // Mise à jour dans tous les cas
    areaInteractionWeights[areaIndex] = interactionWeight;
    
    // Pas de mise à jour des excitations : cela dépend des modifs apportées à TOUTES les aires,
    // on attend l'ordre de notification (par la scène mère, ou autre...)
}
void Exciter::OnAreaNotExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis)
{
    // On fait la recherche, et on vérifie le résultat par sécurité...
    size_t areaIndex = findAreaInteractingIndex(areaExcitedByThis);
    if ( areaIndex < areasInteractingWith.size() )
    {
        // D'abord on impose une mise à jour côté aire graphique (excitation nulle forcée)
        areasInteractingWith[areaIndex].lock()->OnNewExcitementAmount(getCastedSharedFromThis(), Excitement());
        
        // Puis : Suppression effective
        auto areaIt = areasInteractingWith.begin();
        std::advance(areaIt, areaIndex);
        areasInteractingWith.erase(areaIt);
        auto interactionIt = areaInteractionWeights.begin();
        std::advance(interactionIt, areaIndex);
        areaInteractionWeights.erase(interactionIt);
        auto excitementIt = areaExcitementAmounts.begin();
        std::advance(excitementIt, areaIndex);
        areaExcitementAmounts.erase(excitementIt);
    }
    else
        DBG("Double notification de fin d'excitation d'une aire par cet excitateur");
}

std::vector< std::weak_ptr<IInteractiveArea> >::iterator Exciter::findAreaInteracting(std::shared_ptr<IInteractiveArea> areaToFind)
{
    // naive research
    for (auto it = areasInteractingWith.begin() ; it != areasInteractingWith.end() ; ++it )
        if ( (*it).lock() == areaToFind )
            return it;
    // if nothing found :
    return areasInteractingWith.end();
}
size_t Exciter::findAreaInteractingIndex(std::shared_ptr<IInteractiveArea> areaToFind)
{
    // naive research
    size_t i = 0;
    for (i = 0 ; i < areasInteractingWith.size() ; i++ )
        if ( areasInteractingWith[i].lock() == areaToFind )
            return i;
    // if nothing found :
    return i;
}
void Exciter::updateExcitationAmounts()
{
    // On appliquera le volume le + tard possible (on fait tout par rapport à 1, puis on diminue ensuite...)
    
    // Total : accumulation type is defined by the type of the last input parameter
    double totalInteractionWeight
    = std::accumulate(areaInteractionWeights.begin(), areaInteractionWeights.end(), 0.0);
    
    // - - - - - Calcul des excitations, tel que la somme des excitations vaut 1 - - - - -
    
    // en vérifiant les cas bizarres limites...
    // Pour être sûr même si des erreurs
    // numériques sont là...
    
    // Si on a un poids total non-valide : répartition uniforme sur toutes les aires
    if (totalInteractionWeight <= 0.0)
    {
        double uniformWeight = 1.0 / (double)(areaExcitementAmounts.size());
        for (auto &excitement : areaExcitementAmounts)
            excitement.Linear = uniformWeight;
    }
    // Si le poids total est OK : on normalise simplement les poids pour calculer les excitations
    else
    {
        for (size_t i = 0 ; i<areaExcitementAmounts.size() ; i++)
            areaExcitementAmounts[i].Linear = areaInteractionWeights[i] / totalInteractionWeight;
    }
    
    // - - - - - Distorsion logarithmique, pour donner la précision aux faibles volumes - - - - -
    // Voir article JIM 2015 pour + d'explications (même si dans l'article la disto était appliquée
    // directement sur les poids d'interaction)
    // on prépare déjà la normalisation qui suit
    double totalAudioExcitement = 0.0;
    for (auto &excitement : areaExcitementAmounts)
    {
        excitement.Audio = AudioUtils<double>::ApplyLowVolumePrecisionDistorsion(excitement.Linear);
        totalAudioExcitement += excitement.Audio;
    }
    
    // - - - - - Dernière normalisation, et application du volume - - - - -
    for (auto &excitement : areaExcitementAmounts)
        excitement.Audio = excitement.Audio / totalAudioExcitement;
}

void Exciter::NotifyNewExcitationToAreas()
{
    // Préparatifs internes
    updateExcitationAmounts();
    //auto multiAreaE = std::make_shared<MultiAreaEvent>(); // le premier event va rester Nothing....
    
    // Notifications à toutes les aires
    for (size_t i = 0; i<areasInteractingWith.size() ; i++)
        areasInteractingWith[i].lock()->OnNewExcitementAmount(getCastedSharedFromThis(), areaExcitementAmounts[i]);
}


// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> Exciter::GetTree()
{
    auto inheritedTree = EditableEllipse::GetTree();
    
    inheritedTree->put("is_animation_sync", isAnimationSynchronized);
    
    return inheritedTree;
}


