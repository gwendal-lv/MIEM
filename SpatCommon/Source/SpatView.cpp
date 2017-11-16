/*
  ==============================================================================

    SpatView.cpp
    Created: 13 Nov 2017 2:40:41pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatView.h"


using namespace Miam;


void SpatView::SetTitle(std::string title)
{
    mainWindow->setName(title);
}
