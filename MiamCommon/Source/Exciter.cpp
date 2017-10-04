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

Exciter::Exciter(uint64_t uniqueId,
                 std::chrono::time_point<clock> commonStartTimePoint_)
:
EditableEllipse(uniqueId),
volume(0.0),
startTimePt( clock::now() ),
commonStartTimePt(commonStartTimePoint_),
isAnimationSynchronized(true)
{
    // Centre (voir DrawableArea)
    displayCenter = false;
    
    // Plain white colour inside
    fillOpacity = 1.0f;
    
    SetNameVisible(false);
    
    SetActive(true);
    SetEnableTranslationOnly(true);
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
    std::chrono::duration<double> duration;
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
