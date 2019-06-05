/*
  ==============================================================================

    PlayerView.h
    Created: 1 Mar 2018 4:08:15pm
    Author:  Gwendal

  ==============================================================================
*/

#pragma once


#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


#include "PlayerAppMode.h"

#include "ControlView.h"



namespace Miam
{
    class PlayerBackgroundComponent;
    class MultiCanvasComponent;
    class GraphicSessionPlayer;
    
    class PlayerView : public ControlView
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        PlayerBackgroundComponent* backgroundComponent;
        
        protected :
        /// \brief Boolean that indicate whether the actual contents must be placed inside a
        /// rectangle that is smaller than the screen (e.g. on iPhone X, ...)
        bool shouldUseSafeArea;
        
        
        // = = = = = = = = = = Setters and Getters = = = = = = = = = =
        public :
        PlayerBackgroundComponent* GetBackgroundComponent() {return backgroundComponent;}
        
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        PlayerView(DocumentWindow* mainWindow_, PlayerBackgroundComponent* _backgroundComponent);
        virtual ~PlayerView() {}
        
        /// \brief Function called after both View and Presenter are contructed
        ///
        /// Ne fait vraiment un truc que dans la classe fille (qui devra se débrouiller avec le
        /// mainContentComponent de l'application réelle)
        virtual void CompleteInitialization(GraphicSessionPlayer*, MultiCanvasComponent*) = 0;
        
        
        
        
        // ----- Events from the Presenter -----
        
        /// \brief Obeys orders from the Presenter when the global PlayerAppMode has changed.
        ///
        /// \param newAppMode The application mode that should be displayed as quickly as possible.
        void ChangeAppMode(PlayerAppMode newAppMode);
        /// \brief Obeys orders from the Presenter when a informative message has to de displayed to
        /// the user.
        ///
        /// \param message Short sentence to be written in the upper uneditable text box.
        void DisplayInfo(const String& message, bool isImportant = false);
        
        /// \brief Will call functions of children components to properly update osc config data
        void SetOscConfigurationFromTree(bptree::ptree& oscTree);
        
        /// \brief Displays complementary information
        void DisplayComplementaryInfo(const String& message);
        
        
        void ShowHelpContents(bool showHelpContents);
        
        
        virtual void ForceResized() = 0;
        
        
        
    };
    
    
    
}
