/*
  ==============================================================================

    InteractiveArea.cpp
    Created: 13 Apr 2016 3:56:27pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <numeric> // std::accumulate

#include "InteractiveArea.h"

#include "Exciter.h"

using namespace Miam;


InteractiveArea::~InteractiveArea()
{
}





// = = = = = = = = = = Interaction avec Excitateurs = = = = = = = = = =

std::shared_ptr<AreaEvent> InteractiveArea::UpdateInteraction(std::shared_ptr<Exciter>& exciter)
{
    // Évènement "nothing" par défaut... Peut-être modifié dans la suite
    auto areaE = std::make_shared<AreaEvent>();
    
    //  - - - Test d'interaction pour commencer - - -
    bool hitTestResult = HitTest(exciter->GetCenterInPixels());
    
    
    //  - - - Ensuite, on regarde si l'excitateur fait déjà partie, ou non,
    // de ceux qui interagissent avec this - - -
    int exciterLocalIndex = -1;
    for (size_t i = 0 ; i<excitersInteractingWithThis.size() && exciterLocalIndex == -1 ; i++)
    {
        if ( excitersInteractingWithThis[i].lock() == exciter)
            exciterLocalIndex = (int) i; // donne une condition de sortie
    }
    
    
    // - - - Selon, on fait les modifs qui s'imposent au niveau de cette instance : - - -
    bool somethingHappened = true;
    // on peut l'ajouter si besoin (de son côté, on l'informe également)
    if (exciterLocalIndex == -1 && hitTestResult)
    {
        // ajout local
        excitersInteractingWithThis.push_back(exciter);
        excitersWeights.push_back(ComputeInteractionWeight(exciter->GetCenterInPixels()));
        // notif à l'excitateur
        exciter->OnAreaExcitedByThis(shared_from_this(), excitersWeights.back());
    }
    // on le supprime s'il existait mais que le test de collision est revenu négatif
    else if (exciterLocalIndex >= 0 && !hitTestResult)
    {
        // Supression locale
        auto exciterIt = excitersInteractingWithThis.begin();
        std::advance(exciterIt, exciterLocalIndex);
        excitersInteractingWithThis.erase(exciterIt);
        auto weightIt = excitersWeights.begin();
        std::advance(weightIt, exciterLocalIndex);
        excitersWeights.erase(weightIt);
        // Notif à l'excitateur
        exciter->OnAreaNotExcitedByThis(shared_from_this());
    }
    // Sinon on met juste à jour le coefficient
    else if (exciterLocalIndex >= 0)
    {
        excitersWeights[exciterLocalIndex] = ComputeInteractionWeight(exciter->GetCenterInPixels());
    }
    // Dernier cas : il ne s'est rien passé...
    else
        somethingHappened = false;

    
    // - - - Pour finir, update et création de l'area event - - -
    if (somethingHappened)
    {
        updateOpacityFromExcitement();
        // On pourrait aussi mettre "color changed", mais c'est inclus dans le type "excitement" :
        areaE = std::make_shared<AreaEvent>(shared_from_this(), AreaEventType::ExcitementAmountChanged);
    }
    
    return areaE;
}


double InteractiveArea::GetTotalExcitementAmount() const
{
    // accumulation type is defined by the type of the last input parameter
    return std::accumulate(excitersWeights.begin(), excitersWeights.end(), 0.0);
}

void InteractiveArea::updateOpacityFromExcitement()
{
    float totalExcitement = (float) GetTotalExcitementAmount();
    if (totalExcitement < 0.0f)
        totalExcitement = 0.0f;
    else if (totalExcitement > 2.0f)
        totalExcitement = 2.0f;
    
    // Opacité pleine à une excitation de 2.0, minimale à un excitation de 0.0
    float newAlpha = getLowFillOpacity() + (1.0f-getLowFillOpacity()) * (totalExcitement / 2.0f);
    
    SetAlpha( newAlpha );
}

