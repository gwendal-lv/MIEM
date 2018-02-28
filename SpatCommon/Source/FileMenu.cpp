/*
  ==============================================================================

    FileMenu.cpp
    Created: 16 Jun 2017 5:50:13pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "FileMenu.h"

#include "ControlPresenter.h"

#include "SpatFileChoosers.h"

using namespace Miam;

FileMenu::FileMenu(ControlPresenter* _spatPresenter) : presenter(_spatPresenter)
{
    menu.addItem(Choices::Load, "Load");
    menu.addItem(Choices::Save, "Save");
    menu.addItem(Choices::SaveAs, "Save As");
}


void FileMenu::ShowMenuAndSendUserAnswer()
{
#ifndef __MIAMOBILE
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
#else
    /* This function uses pop-ups and
     * must not be executed form a mobile platform.
     */
    assert(0);
#endif
}


void FileMenu::onLoad()
{
#ifndef __MIAMOBILE
    LoadFileChooser fileChooser;
    if ( fileChooser.browseForFileToOpen() )
    {
        File resultFile = fileChooser.getResult();
        presenter->LoadSession(resultFile.getFullPathName().toStdString());
    }
#else
    /* This function uses pop-ups and
     * must not be executed form a mobile platform.
     */
    assert(0);
#endif
}
void FileMenu::onSave()
{
    // nom de fichier non-spécifié, par contre on force le data refresh
    presenter->SaveSession("", true);
}
void FileMenu::onSaveAs()
{
#ifndef __MIAMOBILE
    SaveFileChooser fileChooser;
    if ( fileChooser.browseForFileToSave(true) )
    {
        File resultFile = fileChooser.getResult();
        // data refresh forcé
        presenter->SaveSession(resultFile.getFullPathName().toStdString(), true);
    }
#else
    /* This function uses pop-ups and
     * must not be executed form a mobile platform.
     */
    assert(0);
#endif
}

