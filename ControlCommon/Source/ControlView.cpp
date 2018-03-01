/*
  ==============================================================================

    ControlView.cpp
    Created: 13 Nov 2017 2:40:41pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "ControlView.h"


using namespace Miam;


void ControlView::SetTitle(std::string title)
{
    mainWindow->setName(title);
}
