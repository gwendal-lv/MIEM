/*
  ==============================================================================

    Exciter.cpp
    Created: 11 Jan 2017 2:36:45pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Exciter.h"

#include <cmath>

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

void Exciter::init()
{
    // Centre (voir DrawableArea)
    displayCenter = false;
    
    SetNameVisible(false);
    
    SetActive(true);
    SetEnableTranslationOnly(true);
    
    // Clignotement
    volume = 0.0;
    startTimePt = clock::now();
    isAnimationSynchronized = true;
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
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

void Exciter::OnAreaExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis)
{
    // On fait la recherche par sécurité...
    if ( findAreaInteracting(areaExcitedByThis) == areasInteractingWith.end() )
        areasInteractingWith.push_back(areaExcitedByThis);
    else
        DBG("Double notification d'excitation d'une aire par cet excitateur");
}
void Exciter::OnAreaNotExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis)
{
    // On fait la recherche, et on vérifie le résultat par sécurité...
    auto it = findAreaInteracting(areaExcitedByThis);
    if ( it != areasInteractingWith.end() )
        areasInteractingWith.erase(it);
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



// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> Exciter::GetTree()
{
    auto inheritedTree = EditableEllipse::GetTree();
    
    inheritedTree->put("is_animation_sync", isAnimationSynchronized);
    
    return inheritedTree;
}


