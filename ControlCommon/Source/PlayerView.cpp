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
    // Ne fonctionne pas dans simulateur macOS en juce v5.4.3 -> ajouté dans la branche
    // develop en mars 2019
    std::string deviceDescription = SystemStats::getDeviceDescription().toStdString();
    std::cout << "[View] device = " << deviceDescription << std::endl;
    
    // Récupération de l'info : besoin d'une safe area, ou non
    // Si le nom commence par "iPhone1" (avec un zéro, ou autre derrière...)
    // ou "iPhone X" -> on fait
    // comme si iPhone X ou supérieur dans tous les cas
    // N.B. : iPhone 7 (simulateur) correspond à iPhone9,3 (réel)
    if ((deviceDescription.find("iPhone1") == 0)
        || (deviceDescription.find("iPhone X") == 0))
        safeArea = SafeAreaType::IphoneX;
    // les iPad à partir de 2018 n'ont plus de bouton d'accueil, et ont les coins arrondis
    // iPad pro du LARAS 9.7 pouces : iPad6,3 en réel, iPad Pro (9.7-inch) en simulateur
    else if (deviceDescription.find("iPad") != std::string::npos)
    {
        /*
        // Simulator values
        if (deviceDescription[4] == ' ')
        {
            if (deviceDescription.find("iPad Pro (11-inch)") == 0
                || deviceDescription.find("iPad Pro (11-inch)") == 0)
                safeArea = SafeAreaType::Ipad_NoMainButton;
            else
                safeArea = SafeAreaType::FullScreen;
        }
        // actual devices values
        else
        {
            int iPadFirstNumber = deviceDescription[4] - '0';
            if (iPadFirstNumber > 6) // 6 : iPad Pro rose du LARAS
                safeArea = SafeAreaType::Ipad_NoMainButton;
            else
                safeArea = SafeAreaType::FullScreen;
        }
        */
        // All iPads will loose space because of the iPads with rounded corners...
        safeArea = SafeAreaType::Ipad_NoMainButton;
    }
    else
        safeArea = SafeAreaType::FullScreen;
}

Rectangle<int> PlayerView::GetSafeBackgroundBounds(Rectangle<int> fullScreenBounds)
{
    // Bounds vont dépendre des écrans bizarres non-carrés, avec Safe Area
    auto backgroundBounds = fullScreenBounds;
    const int barMargin = 16;
    const int margin = 28;
    const int thinMargin = 8;
    
    switch (safeArea)
    {
        case Miam::SafeAreaType::FullScreen:
            break;
            
        case Miam::SafeAreaType::IphoneX:
            switch (Desktop::getInstance().getCurrentOrientation())
        {
                
            case juce::Desktop::upright:
            case juce::Desktop::upsideDown:
                backgroundBounds.removeFromTop(margin);
                backgroundBounds.removeFromBottom(margin);
                break;
                
            case juce::Desktop::rotatedClockwise:
                backgroundBounds.removeFromRight(margin);
                backgroundBounds.removeFromLeft(thinMargin);
                backgroundBounds.removeFromBottom(barMargin);
                break;
            case juce::Desktop::rotatedAntiClockwise:
                backgroundBounds.removeFromLeft(margin);
                backgroundBounds.removeFromRight(thinMargin);
                backgroundBounds.removeFromBottom(barMargin);
                break;
                
            case juce::Desktop::allOrientations:
                break;
        }
            break;
            
        case Miam::SafeAreaType::Ipad_NoMainButton:
            backgroundBounds.removeFromBottom(barMargin);
            backgroundBounds.removeFromLeft(thinMargin);
            backgroundBounds.removeFromRight(thinMargin);
            backgroundBounds.removeFromTop(thinMargin);
            break;
    }
    
    return backgroundBounds;
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


