/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
    
    enum class SafeAreaType {
        FullScreen, ///< Normal rectangle screen -> no safe area needed
        IphoneX, ///< Quite large safe area, that depends on the orientation (need to consider the bar, etc.)
        Ipad_NoMainButton, ///< ipads without a physical main button (need space on the bottom)
        AndroidWithNotch, ///< Detection based on android version... can definitely be optimized
    };
    
    class PlayerView : public ControlView
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        PlayerBackgroundComponent* backgroundComponent;
        
        protected :
        /// \brief Boolean that indicate whether the actual contents must be placed inside a
        /// rectangle that is smaller than the screen, iPhone X
        SafeAreaType safeArea;
        
        String backgroundInfo;
        const int backgroundInfoAutoRedisplay_ms = 10000;
        /// \brief Indicate whether a normal info has been recently sent for display
        bool normalInfoTimerExpired = true;
        
        
        // = = = = = = = = = = Setters and Getters = = = = = = = = = =
        public :
        PlayerBackgroundComponent* GetBackgroundComponent() {return backgroundComponent;}
        SafeAreaType GetSafeAreaType() const {return safeArea;}
        Rectangle<int> GetSafeBackgroundBounds(Rectangle<int> fullScreenBounds);
        
        
        
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
        
        /// \brief Displays an info that can be temprarily replaced,
        /// but which automatically re-display itself after a timeout.
        ///
        /// It this function is called several, the last String only will be displayed.
        void DisplayBackgroundInfo(const String& message);
        
        /// \brief Will call functions of children components to properly update osc config data
        void SetOscConfigurationFromTree(bptree::ptree& oscTree);
        
        /// \brief Displays complementary information
        void DisplayComplementaryInfo(const String& message);
        
        
        void ShowHelpContents(bool showHelpContents);
        
        
        virtual void ForceResized() = 0;
        
        
        
    };
    
    
    
}
