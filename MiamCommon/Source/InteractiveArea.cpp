/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <numeric> // std::accumulate

#include "InteractiveArea.h"
#include "Exciter.h"

#include "SceneCanvasComponent.h"

#include "MiamMath.h"

using namespace Miam;


InteractiveArea::~InteractiveArea()
{
}

void InteractiveArea::onCloned()
{
    while (excitersInteractingWithThis.size() > 0)
        // comme on a des vector< >, on supprime les derniers pour éviter des ré-allocations
        deleteLinksToExciter(excitersInteractingWithThis.size()-1);
}




void InteractiveArea::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    elementInteractionRadius = 0.01f * (_parentCanvas->getWidth()+_parentCanvas->getHeight())/2.0f; // 1%
}




// = = = = = = = = = = Interaction avec Excitateurs = = = = = = = = = =

std::shared_ptr<AreaEvent> InteractiveArea::UpdateInteraction(std::shared_ptr<Exciter>& exciter, bool forceDisableInteraction)
{
    // Évènement "nothing" par défaut... Peut-être modifié dans la suite
    auto areaE = std::make_shared<AreaEvent>();
    
    //  - - - Test d'interaction pour commencer - - -
    bool hitTestResult = HitTest(exciter->GetCenterInPixels()) && (! forceDisableInteraction);
    
    
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
        excitementAmounts.push_back(Excitement());
        // notif à l'excitateur, qui renvoit son niveau d'excitation envers cette aire
        exciter->OnAreaExcitedByThis(shared_from_this(), excitersWeights.back());
    }
    // on le supprime s'il existait mais que le test de collision est revenu négatif
    else if (exciterLocalIndex != excitersInteractingWithThis.size() && !hitTestResult)
    {
        // Supression locale
        deleteLinksToExciter(exciterLocalIndex);
        // Notif à l'excitateur
        exciter->OnAreaNotExcitedByThis(shared_from_this());
    }
    // Sinon on met juste à jour le coefficient
    else if (exciterLocalIndex != excitersInteractingWithThis.size())
    {
        // Poids seulement, pas excitation
        excitersWeights[exciterLocalIndex] = ComputeInteractionWeight(exciter->GetCenterInPixels());
        // Notif à l'excitateur
        exciter->OnAreaExcitedByThis(shared_from_this(), excitersWeights[exciterLocalIndex]);
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

void InteractiveArea::OnExciterDestruction()
{
    size_t i = 0;
    while (i < excitersInteractingWithThis.size())
    {
        // On supprime les excitateurs expirés (détruits)
        if (excitersInteractingWithThis[i].expired())
            deleteLinksToExciter(i);
        // seulement Si l'excitateur est OK, on incréménte
        else
            i++;
    }
}

void InteractiveArea::deleteLinksToExciter(size_t exciterLocalIndex)
{
    auto exciterIt = excitersInteractingWithThis.begin();
    std::advance(exciterIt, exciterLocalIndex);
    excitersInteractingWithThis.erase(exciterIt);
    auto weightIt = excitersWeights.begin();
    std::advance(weightIt, exciterLocalIndex);
    excitersWeights.erase(weightIt);
    auto excitementIt = excitementAmounts.begin();
    std::advance(excitementIt, exciterLocalIndex);
    excitementAmounts.erase(excitementIt);
}

double InteractiveArea::GetTotalInteractionWeight() const
{
    // accumulation type is defined by the type of the last input parameter
    return std::accumulate(excitersWeights.begin(), excitersWeights.end(), 0.0);
}
double InteractiveArea::GetTotalLinearExcitement() const
{
    double totalExcitement = 0.0;
    for (const auto& excitement : excitementAmounts)
        totalExcitement += excitement.Linear;
    return totalExcitement;
}
double InteractiveArea::GetTotalAudioExcitement() const
{
    double totalExcitement = 0.0;
    for (const auto& excitement : excitementAmounts)
        totalExcitement += excitement.Audio;
    return totalExcitement;
}

void InteractiveArea::OnNewExcitementAmount(const std::shared_ptr<Exciter>& sender, Excitement excitementAmount)
{
    // Recherche inverse de l'excitateur
    size_t exciterLocalIndex = getExciterLocalIndex(sender);
    // Qu'on le trouve ou pas, on continue quand même
    if (exciterLocalIndex < excitersInteractingWithThis.size()) // si on le trouve
        excitementAmounts[exciterLocalIndex] = excitementAmount;

    
    // Calcul graphique ensuite (sur des float)
    float totalExcitement = (float) GetTotalLinearExcitement();
    totalExcitement = Math::Clamp(totalExcitement, 0.0f, 2.0f);
    
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
        auto lockedExciterInteractingWithThis = excitersInteractingWithThis[i].lock();
#ifdef __MIAM_DEBUG
        if ( ! (lockedExciterInteractingWithThis) )
            throw std::logic_error("weak_ptr sur Excitateur référencé n'est plus valide...");
#endif
        if ( lockedExciterInteractingWithThis == exciter)
            return i;
    }
    return i;
}


