/*
  ==============================================================================

    SpatPresenter.h
    Created: 6 Jun 2017 10:49:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "IPresenter.h"
#include "SpatView.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#define Miam_SessionFileExtension       "mspat"

namespace Miam {
    
    class SpatModel;
    class GraphicSpatSessionManager;
    
    /// \brief
    class SpatPresenter : public IPresenter
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        // Private links to abtractions of sub-modules
        SpatModel* model = 0;
        GraphicSpatSessionManager* graphicSessionManager = 0;
        // Private links to other modules
        SpatView* view;
        
        // - - - - - Back-ups of other spatialization data trees - - - - -
        std::shared_ptr<bptree::ptree> lastSpatStatesTree;
        std::shared_ptr<bptree::ptree> lastSpatScenesTree;
        std::string lastFilename;
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        // - - - - - Construction and Destruction (and init) - - - - -
        SpatPresenter(SpatView* view_);
        virtual ~SpatPresenter() {}
        void CompleteInitialisation(GraphicSpatSessionManager* _graphicSessionManager, SpatModel* _model);
        
        
        
        // - - - - - Self XML import/export - - - - -
        
        virtual std::shared_ptr<bptree::ptree> GetConfigurationTree()
        {return std::make_shared<bptree::ptree>(); } // empty tree....
        virtual void SetConfigurationFromTree(bptree::ptree&) = 0;
        
        
        // - - - - - XML import/export general management - - - - -
        
        virtual void LoadSession(std::string filename);
        /// \Brief Saves the current session to the given file name, or to the last
        /// used file name is file name is empty.
        virtual void SaveSession(std::string filename = "", bool forceDataRefresh = false);
        
        protected :
        virtual void updateSpatStatesTree(std::shared_ptr<bptree::ptree> newTree, bool autoSave = true);
        virtual void updateSpatScenesTree(std::shared_ptr<bptree::ptree> newTree, bool autoSave = true);
    };
    
    
    
}
