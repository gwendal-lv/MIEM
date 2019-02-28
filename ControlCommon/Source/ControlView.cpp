/*
  ==============================================================================

    ControlView.cpp
    Created: 13 Nov 2017 2:40:41pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "ControlView.h"


using namespace Miam;

void ControlView::SetFullscreenState(bool shouldBeFullscreen)
{
#ifdef __MIAMOBILE
    // on iOS / Android
    assert(false); // we should not ask for such a fullscreen (kiosk-based) mode...
    return;
#else
    auto& desktop = Desktop::getInstance();
    if (shouldBeFullscreen)
        desktop.setKioskModeComponent(mainWindow);
    else
        desktop.setKioskModeComponent(nullptr);
#endif
}
bool ControlView::GetFullscreenState()
{
#ifdef __MIAMOBILE
    return true;
#else
    
    auto& desktop = Desktop::getInstance();
    auto kioskModeComponent = desktop.getKioskModeComponent();
    return kioskModeComponent != nullptr;
#endif
}

void ControlView::SetTitle(std::string title)
{
    mainWindow->setName(title);
}
