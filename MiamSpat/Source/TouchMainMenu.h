/*
  ==============================================================================

    TouchMainMenu.h
    Created: 15 Jun 2017 7:08:16pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace Miam
{
    class Presenter;
    
    /// \brief Class that displays a pop-up menu for loading/saving/etc...
    /// files, and that also gets the user info about the file's location
    /// and sends it to a Miam::SpatPresenter
    ///
    /// UN JOUR PEUT-ÊTRE on pourrait stocker dans une liste les fichiers récents,
    /// ou juste le nom du dernier dossier utilisé...
    /// Le tout dans le SpecialLocationType::commonApplicationDataDirectory
    ///
    /// Pour l'instant "mes documents"
    /// sera le départ
    class TouchMainMenu
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        
        enum Choices : int {
            Load = 1,
            CommunicationSettings,
        };
        
        // links
        Presenter* presenter;
        
        // Graphical objects
        PopupMenu menu;
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        
        public :
        TouchMainMenu(Presenter* presenter_);
        
        void ShowMenuAndSendUserAnswer();
        
        private :
        void onLoad();
        
    };
}
