/*
  ==============================================================================

    PlayerView.cpp
    Created: 1 Mar 2018 4:08:15pm
    Author:  Gwendal

  ==============================================================================
*/

#include "PlayerView.h"

#include "PlayerBackgroundComponent.h"


using namespace Miam;







void PlayerView::ChangeAppMode(PlayerAppMode newAppMode)
{
    // Pas de traitement global ! On laisse toujours le fond visible.
    // Retransmission simple au composant le + concerné....
    if (backgroundComponent)
        backgroundComponent->ChangeAppMode(newAppMode);
}
void PlayerView::DisplayInfo(const String& message, bool isImportant)
{
    backgroundComponent->DisplayInfo(message, isImportant);
}




