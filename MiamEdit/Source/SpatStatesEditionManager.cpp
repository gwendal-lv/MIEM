/*
  ==============================================================================

    SpatStatesEditionManager.cpp
    Created: 26 Nov 2016 6:47:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatStatesEditionManager.h"

#include "View.h"
#include "SpatStatesEditionComponent.h"



using namespace Miam;

SpatStatesEditionManager::SpatStatesEditionManager(View* _view)
{
    view = _view;
    
    editionComponent = view->GetMainContentComponent()->GetSpatStatesEditionComponent();
    editionComponent->CompleteInitialization(this);
}

void SpatStatesEditionManager::CompleteInitialisation(SpatInterpolator<double>* _spatInterpolator)
{
    spatInterpolator = _spatInterpolator;
    
    // TEMPORARY
    // PARCE QUE SÉLECTION DÉBILE
    selectedSpatState = spatInterpolator->GetSpatState(0);
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

void SpatStatesEditionManager::SelectSpatState(std::shared_ptr<SpatState<double>> _spatState)
{
    //editionComponent->HIDE FADERS
}


size_t SpatStatesEditionManager::GetFadersCount()
{
    return selectedSpatState->GetOutputsCount();
}
