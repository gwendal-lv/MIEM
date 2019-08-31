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

#ifndef PRESENTER_H_INCLUDED
#define PRESENTER_H_INCLUDED

#include <iostream>

#include "ControlPresenter.h"

#include "AppMode.h"
#include "AppPurpose.h"
#include "SpatType.h"

#include "GraphicSessionManager.h"
#include "SpatStatesEditionManager.h"
#include "SettingsManager.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


namespace Miam {
    // Simple declarations : we don't need the entire description
    class Model;
    class View;
    

    
    /// \brief One of the three main modules of the MiamEdit program, that defines the behavior
	/// between the user and displayed GUI elements. Miam-specific graphical objects belong to it.
	///
	/// Read <a href="http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter">MPV scheme</a>
	/// for a better understanding of MiamEdit functionnalities between the Model, the Presenter 
	/// and the View : "The presenter acts upon the model and the view. It retrieves data from 
	/// repositories (the model), and formats it for display in the view".
	///
	/// \remark Usual GUI controls (created from the Projucer) however belong to the View.
    class Presenter : public ControlPresenter
    {
        private :
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =

        // Pointer to the unique View module
        View* view = 0;
        // Pointer to the unique Model module
        Model* model = 0;
        
        AppMode appMode;
        
        
        // Sub-modules
        GraphicSessionManager graphicSessionManager;
        SpatStatesEditionManager spatStatesEditionManager;
        SettingsManager settingsManager;
        
        /// \brief We must keep a thread-safe copy of this info... which is hard
        /// keep perfectly up-to-date across all sub-modules of this Presenter...
        bool isModelConnected = false;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        
        public :
        
        AppMode getAppMode() {return appMode;};
        
        GraphicSessionManager* getGraphicSessionManager() {return &graphicSessionManager;}
        SettingsManager* GetSettingsManager() {return &settingsManager;}
        SpatStatesEditionManager* GetSpatStatesManager() {return &spatStatesEditionManager;}
        
        virtual AppPurpose GetSessionPurpose() const override;
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // - - - - - Contruction and Destruction - - - - -
        // Only constructor
        Presenter(View* _view);
        /// \brief To be called from the Miam::Model when it is being
        /// constructed (happens after the construction of this class)
        ///
        /// Finished self-contruction, and also the construction of sub-modules
        void CompleteInitialisation(Model* _model);
        
        // Events from the View
        AppMode appModeChangeRequest(AppMode newAppMode);
        
        
        
        void Update() override;
        
        
        /// \brief Status will be simply displayed as low-priority info in View
        virtual void DisplayStatus(std::string infoStr) override;
        
        
        /// \brief Fonction qui définit le comportement initial de l'application :
        /// on peut ouvrir la session passée en ligne de commande, ou bien créer une nouvelle
        /// session (vide, ou à partir de la session par défaut)
        void ManageInitialSession(std::string commandLine);
        /// \brief Tries to connect the model with the current available configuration
        ///
        /// Will manage and display info if exceptions occur.
        void TryConnectModelToRemote();
        void TryModelPlay();
        void TryModelStop();
        /// \brief Comportement final de l'application
        void OnShutdownRequest();
        
        // - - - - -  XML import/export - - - - -
        virtual void LoadSession(std::string filename, URL fileUrl = URL()) override;
        virtual void SaveSession(std::string filename = "", bool forceDataRefresh = false) override;
        
        /// \brief Crée une nouvelle session, en commençant par la sauvegarder. 'filename' doit être
        /// complet (y compris extension du fichier)
        ///
        /// Si la session à créer n'est pas vide, alors elle sere créée à partir d'une copie de la
        /// session par défaut.
        void CreateSession(std::string filename, AppPurpose sessionPurpose, bool isEmpty);
        
        virtual std::shared_ptr<bptree::ptree> GetConfigurationTree() override;
        virtual void SetConfigurationFromTree(bptree::ptree&) override;

        
        
    };
    
    
    
}



#endif  // PRESENTER_H_INCLUDED
