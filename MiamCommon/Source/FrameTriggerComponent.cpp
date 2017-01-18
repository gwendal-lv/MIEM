/*
  ==============================================================================

    FrameTriggerComponent.cpp
    Created: 17 Jan 2017 3:15:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "FrameTriggerComponent.h"

FrameTriggerComponent::FrameTriggerComponent(IGraphicSessionManager* graphicSessionManager_)
:
graphicSessionManager(graphicSessionManager_)
{
    
}



void FrameTriggerComponent::update()
{
    graphicSessionManager->OnFrameTrigger();
}
