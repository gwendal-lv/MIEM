/*
  ==============================================================================

    FileMenu.cpp
    Created: 16 Jun 2017 5:50:13pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "FileMenu.h"

#include "ControlPresenter.h"

#include "MiemFileChoosers.h"

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
    LoadFileChooser fileChooser({App::GetPurpose()});
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
    
    presenter->DisplayStatus(TRANS("Session saved.").toStdString());
}
void FileMenu::onSaveAs()
{
#ifndef __MIAMOBILE
    // Warning : App::GetPurpose() returns Multi for the Editor !
    // So we should not use it when saving (the type is already defined)
    AppPurpose sessionPurpose = presenter->GetSessionPurpose();
    SaveFileChooser fileChooser({sessionPurpose});
    if ( fileChooser.browseForFileToSave(true) )
    {
        File resultFile = fileChooser.getResult();
        // We check that the path is valid (we add the necessary
        // .mcs or .mspat suffix if necessary)
        std::string resultPath = resultFile.getFullPathName().toStdString();
        if( !PathUtils::CheckForExtensionAndPurposeCoherence(resultPath, sessionPurpose) )
            resultPath += PathUtils::GetSessionFileExtension(sessionPurpose);
        // data refresh forcé
        presenter->SaveSession(resultPath, true);
        
        presenter->DisplayStatus(TRANS("Session saved.").toStdString());
    }
    
#else
    /* This function uses pop-ups and
     * must not be executed form a mobile platform.
     */
    assert(0);
#endif
}

