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
    // auto osType = SystemStats::getOperatingSystemType(); // android...
    std::string osName = SystemStats::getOperatingSystemName().toStdString();
    Logger::outputDebugString("[View - DeviceDescription] " + deviceDescription
        + " OSname='" + osName + "'");

    // full is default (might be changed)
    safeArea = SafeAreaType::FullScreen;

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
    else if (osName.find("Android ") == 0)
    {
        // the OS name returns the Linux Kernel version
        // see https://en.wikipedia.org/wiki/Android_version_history#Code_names
        if (osName.length() >= 10)
        if (osName[8] == '4')
        {
            // all versions from Android Nougat (7.0) API 24
            if (osName[9] == '.')
                safeArea = SafeAreaType::AndroidWithNotch;
        }
    }
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
        case SafeAreaType::FullScreen:
            break;

        case SafeAreaType::AndroidWithNotch:
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
                    break;
                case juce::Desktop::rotatedAntiClockwise:
                    backgroundBounds.removeFromLeft(margin);
                    backgroundBounds.removeFromRight(thinMargin);
                    break;

                case juce::Desktop::allOrientations:
                    break;
            }
            break;
            
        case SafeAreaType::IphoneX:
            switch (Desktop::getInstance().getCurrentOrientation())
        {
                
            case juce::Desktop::upright:
            case juce::Desktop::upsideDown:
                backgroundBounds.removeFromTop(margin);
                backgroundBounds.removeFromBottom(margin);
                break;
                
                // Nouveau problème depuis le landscape forcé dans Juce...
                // Ces deux cas sont mélangés... donc on
                // les rend identiques....
            case juce::Desktop::rotatedClockwise:
                backgroundBounds.removeFromRight(margin);
                //backgroundBounds.removeFromLeft(thinMargin);
                backgroundBounds.removeFromLeft(margin);
                backgroundBounds.removeFromBottom(barMargin);
                break;
            case juce::Desktop::rotatedAntiClockwise:
                backgroundBounds.removeFromLeft(margin);
                //backgroundBounds.removeFromRight(thinMargin);
                backgroundBounds.removeFromRight(margin);
                backgroundBounds.removeFromBottom(barMargin);
                break;
                
            case juce::Desktop::allOrientations:
                break;
        }
            break;
            
        case SafeAreaType::Ipad_NoMainButton:
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
    normalInfoTimerExpired = false;
    backgroundComponent->DisplayInfo(message, isImportant);
    Timer::callAfterDelay(backgroundInfoAutoRedisplay_ms,
                          [this] {
                              this->normalInfoTimerExpired = true;
                              this->backgroundComponent->DisplayInfo(backgroundInfo);
                          } );
}
void PlayerView::DisplayBackgroundInfo(const String& message)
{
    backgroundInfo = message;
    // we display now if the timer has expired.
    if (normalInfoTimerExpired)
        backgroundComponent->DisplayInfo(backgroundInfo);
    // If not expired, the DisplayInfo() will update the backgroundinfo from
    // a lambda function
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


