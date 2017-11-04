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
    size_t exciterLocalIndex = getExciterLocalIndex(exciter);
    
    
    // - - - Selon, on fait les modifs qui s'imposent au niveau de cette instance : - - -
    bool somethingHappened = true;
    // on peut l'ajouter si besoin (de son côté, on l'informe également)
    if (exciterLocalIndex == excitersInteractingWithThis.size() && hitTestResult)
    {
        // ajout local
        excitersInteractingWithThis.push_back(exciter);
        excitersWeights.push_back(ComputeInteractionWeight(exciter->GetCenterInPixels()));
        excitementAmounts.push_back(0.0);
        // notif à l'excitateur, qui renvoit son niveau d'excitation envers cette aire
        exciter->OnAreaExcitedByThis(shared_from_this(), excitersWeights.back());
    }
    // on le supprime s'il existait mais que le test de collision est revenu négatif
    else if (exciterLocalIndex != excitersInteractingWithThis.size() && !hitTestResult)
    {
        // Supression locale
        auto exciterIt = excitersInteractingWithThis.begin();
        std::advance(exciterIt, exciterLocalIndex);
        excitersInteractingWithThis.erase(exciterIt);
        auto weightIt = excitersWeights.begin();
        std::advance(weightIt, exciterLocalIndex);
        excitersWeights.erase(weightIt);
        auto excitementIt = excitementAmounts.begin();
        std::advance(excitementIt, exciterLocalIndex);
        excitementAmounts.erase(excitementIt);
        // Notif à l'excitateur
        exciter->OnAreaNotExcitedByThis(shared_from_this());
    }
    // Sinon on met juste à jour le coefficient
    else if (exciterLocalIndex != excitersInteractingWithThis.size())
    {
        // Poids seulement, pas excitation
        excitersWeights[exciterLocalIndex] = ComputeInteractionWeight(exciter->GetCenterInPixels());
        // Notif à l'excitateur
        exciter->OnAreaExcitedByThis(shared_from_this(), excitersWeights.back());
    }
    // Dernier cas : il ne s'est rien passé...
    else
        somethingHappened = false;

    
    // - - - Pour finir, update et création de l'area event - - -
    if (somethingHappened)
    {
        // On pourrait aussi mettre "color changed", mais c'est inclus dans le type "excitement" :
        // Et en plus ça serait plutôt "interaction weight changed", mais bon...
        std::shared_ptr<IDrawableArea> localSharedFromThis = shared_from_this();
        areaE = std::make_shared<AreaEvent>(localSharedFromThis, AreaEventType::ExcitementAmountChanged);
    }
    
    return areaE;
}


double InteractiveArea::GetTotalInteractionWeight() const
{
    // accumulation type is defined by the type of the last input parameter
    return std::accumulate(excitersWeights.begin(), excitersWeights.end(), 0.0);
}
double InteractiveArea::GetTotalExcitementAmount() const
{
    // accumulation type is defined by the type of the last input parameter
    return std::accumulate(excitementAmounts.begin(), excitementAmounts.end(), 0.0);
}

void InteractiveArea::OnNewExcitementAmount(const std::shared_ptr<Exciter>& sender, double excitementAmount)
{
    // Recherche inverse de l'excitateur
    size_t exciterLocalIndex = getExciterLocalIndex(sender);
    // S'il n'est pas là, on continue quand même !
    if (exciterLocalIndex < excitersInteractingWithThis.size())
        excitementAmounts[exciterLocalIndex] = excitementAmount;
    
    // Calcul graphique ensuite (sur des float)
    float totalExcitement = (float) GetTotalExcitementAmount();
    if (totalExcitement < 0.0f)
        totalExcitement = 0.0f;
    else if (totalExcitement > 2.0f)
        totalExcitement = 2.0f;
    
    // Opacité pleine à une excitation de 2.0, minimale à un excitation de 0.0
    float newAlpha = getLowFillOpacity() + (1.0f-getLowFillOpacity()) * (totalExcitement / 2.0f);
    
    SetAlpha( newAlpha );
    
    // (Renvoi d'un évènement qui va bien...) -> plus rien à renvoyer (forcément déjà fait)
    //return std::make_shared<AreaEvent>(shared_from_this(), AreaEventType::ExcitementAmountChanged);
}


size_t InteractiveArea::getExciterLocalIndex(const std::shared_ptr<Exciter>& exciter)
{
    size_t i;
    for (i = 0 ; i<excitersInteractingWithThis.size() ; i++)
    {
        if ( excitersInteractingWithThis[i].lock() == exciter)
            return i;
    }
    return i;
}
