/*
  ==============================================================================

    SettingsManager.cpp
    Created: 25 May 2017 11:09:10am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <iostream>

#include "SettingsManager.h"

#include "View.h"
#include "Model.h"


using namespace Miam;



SettingsManager::SettingsManager(View* _view, Presenter* _presenter)
:
view(_view),
presenter(_presenter)
{
}
void SettingsManager::CompleteInitialisation(Model* _model)
{
    model = _model;
    
    // Links on the view side
    view->GetMainContentComponent()->GetConfigurationComponent()->CompleteInitialization(this);
}





void SettingsManager::OnInOutChannelsCountChanged(int inputsCount, int outputsCount)
{
    // model updates
    
    // Might not validate the change... We don't know !
    model->GetSpatInterpolator()->SetInputOuputChannelsCount(inputsCount, outputsCount);
    
    // Direct dispatching of the event (not telling other presenter modules)
    view->GetMainContentComponent()->GetSpatStatesEditionComponent()->SetInsOutsCount(model->GetSpatInterpolator()->GetInputsCount(),
         model->GetSpatInterpolator()->GetOutputsCount());
}
