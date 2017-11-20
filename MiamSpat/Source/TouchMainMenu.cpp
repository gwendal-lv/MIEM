/*
  ==============================================================================

    TouchMainMenu.cpp
    Created: 16 Jun 2017 5:50:13pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "TouchMainMenu.h"
#include "SpatFileChoosers.h"

#include "Presenter.h"

#include <string>

using namespace Miam;

TouchMainMenu::TouchMainMenu(Presenter* presenter_) : presenter(presenter_)
{
    menu.addItem(Choices::Load, "Load");
}


void TouchMainMenu::ShowMenuAndSendUserAnswer()
{
    int userAnswer = menu.show();
    
    switch(userAnswer)
    {
        case Choices::Load :
            onLoad();
            break;
            
        default :
            break;
    }
}


void TouchMainMenu::onLoad()
{
	LoadFileChooser fileChooser;
    if ( fileChooser.browseForFileToOpen() )
    {
        File resultFile = fileChooser.getResult();
        presenter->LoadSession(resultFile.getFullPathName().toStdString());
    }
}

