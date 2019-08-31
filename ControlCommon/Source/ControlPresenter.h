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

#include <iostream>
#include <string>
#include <memory>

#include "IPresenter.h"
#include "ControlView.h"

#include "MiemFileChoosers.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

namespace Miam {
    
    class ControlModel;
    class GraphicControlSessionManager;
    
    /// \brief
    class ControlPresenter : public IPresenter
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        // Private links to abtractions of sub-modules
        ControlModel* model = 0;
        GraphicControlSessionManager* graphicSessionManager = 0;
        // Private links to other modules
        ControlView* view;
        
        protected :
        // - - - - - Back-ups of other spatialization data trees - - - - -
        std::shared_ptr<bptree::ptree> lastSpatStatesTree;
        std::shared_ptr<bptree::ptree> lastSpatScenesTree;
        std::string lastFilename;
        URL lastURL;
        
        
        // - - - - - Lock-free fast communication - - - - -
        /// \brief Used to know whether the model has really stopped or not
        std::atomic<bool> hasModelActuallyStopped;
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        // - - - - - Construction and Destruction (and init) - - - - -
        ControlPresenter(ControlView* view_);
        virtual ~ControlPresenter() {}
        void CompleteInitialisation(GraphicControlSessionManager* _graphicSessionManager, ControlModel* _model);
        
        
        /// \brief Method for displaying a status (important info), to be overriden
        virtual void DisplayStatus(std::string) { assert(false); /* no default implementation */ }
        
        
        // - - - - - Graphical reccurent update - - - - -
        virtual void Update() override;
        
        
        
        // - - - - - Events from Model - - - - -
        protected :
        virtual void processParamChangeFromModel(AsyncParamChange const & paramChange);
        public :
        
        /// \brief Method that must be called from the Model
        /// to notify that is has actually stopped
        void OnModelStopped();
        
        
        // - - - - - Self XML import/export - - - - -
        public :
        
        virtual std::shared_ptr<bptree::ptree> GetConfigurationTree()
        {return std::make_shared<bptree::ptree>(); } // empty tree....
        virtual void SetConfigurationFromTree(bptree::ptree&) = 0;
        
        
        // - - - - - XML import/export general management - - - - -
        
        /// \brief Load the file at filename, then parses the XML inside and actually
        /// loads the controller session.
        ///
        /// Will load the default session if the filename is DefaultSessions::GetDefaultSessionCode()
        virtual void LoadSession(std::string filename, URL fileUrl = URL());
        /// \Brief Saves the current session to the given file name, or to the last
        /// used file name is file name is empty.
        virtual void SaveSession(std::string filename = "", bool forceDataRefresh = false);
        
        protected :
        virtual void updateSpatStatesTree(std::shared_ptr<bptree::ptree> newTree, bool autoSave = true);
        virtual void updateSpatScenesTree(std::shared_ptr<bptree::ptree> newTree, bool autoSave = true);
    };
    
    
    
}
