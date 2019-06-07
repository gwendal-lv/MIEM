/*
  ==============================================================================

    FileMenu.h
    Created: 15 Jun 2017 7:08:16pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "AppPurpose.h"

namespace Miam
{
    class ControlPresenter;
    
    /// \brief Class that displays a pop-up menu for loading/saving/etc...
    /// files, and that also gets the user info about the file's location
    /// and sends it to a Miam::ControlPresenter
    ///
    /// UN JOUR PEUT-ÊTRE on pourrait stocker dans une liste les fichiers récents,
    /// ou juste le nom du dernier dossier utilisé...
    /// Le tout dans le SpecialLocationType::commonApplicationDataDirectory
    ///
    /// Pour l'instant "mes documents"
    /// sera le départ
    class FileMenu
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        
        enum Choices : int {
            Load = 1,
            Save,
            SaveAs,
        };
        
        // links
        ControlPresenter* presenter;
        
        // Graphical objects
        PopupMenu menu;
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        
        public :
        FileMenu(ControlPresenter* _presenter);
        
        void ShowMenuAndSendUserAnswer();
        
        // public for direct use via BackgroundComponent
        public :
        void onLoad();
        void onSave();
        void onSaveAs();
        
    };
}
