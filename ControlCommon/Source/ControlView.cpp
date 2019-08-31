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
