/*
  ==============================================================================

    FileMenu.cpp
    Created: 16 Jun 2017 5:50:13pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "FileMenu.h"

#include "SpatPresenter.h"

using namespace Miam;

FileMenu::FileMenu(SpatPresenter* _spatPresenter) : presenter(_spatPresenter)
{
    menu.addItem(Choices::Load, "Load");
    menu.addItem(Choices::Save, "Save", false); // disabled for now...
    menu.addItem(Choices::Save, "SaveAs", false); // disabled for now...
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
    FileChooser fileChooser("Chargement d'un fichier",
                            File::getSpecialLocation(File::SpecialLocationType::userMusicDirectory),
                            "*.miam",
                            true);
    fileChooser.browseForFileToOpen();
    File resultFile = fileChooser.getResult();
    presenter->LoadSession(resultFile.getFullPathName().toStdString());
}
void FileMenu::onSave()
{
    
}
void FileMenu::onSaveAs()
{
    
}

