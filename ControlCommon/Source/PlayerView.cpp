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



PlayerView::PlayerView(DocumentWindow* mainWindow_, PlayerBackgroundComponent* _backgroundComponent) :
ControlView(mainWindow_),
backgroundComponent(_backgroundComponent)
{
    std::string deviceDescription = SystemStats::getDeviceDescription().toStdString();
    
    std::cout << "[View] device = " << deviceDescription << std::endl;
}



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
void PlayerView::DisplayComplementaryInfo(const String& message)
{
    backgroundComponent->DisplayInfo2(message);
}
void PlayerView::SetOscConfigurationFromTree(bptree::ptree& oscTree)
{
    backgroundComponent->GetMainMenuComponent()->SetOscConfigurationFromTree(oscTree);
}

void PlayerView::ShowHelpContents(bool showHelpContents)
{
    backgroundComponent->GetMainMenuComponent()->SetIsHelpDisplayed(showHelpContents);
}


