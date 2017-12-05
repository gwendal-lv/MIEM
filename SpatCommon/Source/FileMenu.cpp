/*
  ==============================================================================

    FileMenu.cpp
    Created: 16 Jun 2017 5:50:13pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "FileMenu.h"

#include "SpatPresenter.h"

#include "SpatFileChoosers.h"

using namespace Miam;

FileMenu::FileMenu(SpatPresenter* _spatPresenter) : presenter(_spatPresenter)
{
    menu.addItem(Choices::Load, "Load");
    menu.addItem(Choices::Save, "Save"); // disabled for now...
    menu.addItem(Choices::SaveAs, "Save As"); // disabled for now...
}


void FileMenu::ShowMenuAndSendUserAnswer()
{
    int userAnswer = menu.show();
    
    switch(userAnswer)
    {
        case Choices::Load :
            onLoad();
            break;
            
        case Choices::Save :
            onSave();
            break;
            
        case Choices::SaveAs :
            onSaveAs();
            break;
            
        default :
            break;
    }
}


void FileMenu::onLoad()
{
    LoadFileChooser fileChooser;
    if ( fileChooser.browseForFileToOpen() )
    {
        File resultFile = fileChooser.getResult();
        presenter->LoadSession(resultFile.getFullPathName().toStdString());
    }
}
void FileMenu::onSave()
{
    // nom de fichier non-spécifié, par contre on force le data refresh
    presenter->SaveSession("", true);
}
void FileMenu::onSaveAs()
{
    SaveFileChooser fileChooser;
    if ( fileChooser.browseForFileToSave(true) )
    {
        File resultFile = fileChooser.getResult();
        // data refresh forcé
        presenter->SaveSession(resultFile.getFullPathName().toStdString(), true);
    }
}

