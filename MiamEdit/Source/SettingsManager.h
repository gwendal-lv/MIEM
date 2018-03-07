/*
  ==============================================================================

    SettingsManager.h
    Created: 25 May 2017 11:09:10am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <memory>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#include "AppPurpose.h"

namespace Miam {
    
    // Forward declarations
    class Presenter;
    
    class Model;
    
    class View;
    class HardwareConfigurationComponent;
    
    
    
    /// \brief Sub-Module of the Miam::Presenter
    ///
    /// Not intended to be re-employed in the Miam spat player
    class SettingsManager
    {
        private :
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        // Pointer to the unique View module
        View* view = 0;
        HardwareConfigurationComponent* configurationComponent;
        // Pointer to the unique Model module
        Model* model = 0;
        // Pointer to the unique Presenter module
        Presenter* presenter = 0;
        
        
        bool linksInitialized = false;
        
        // Settings concerning Presenter (to backup internally)
        bool allowKeyboardEdition = false;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        
        public :
        AppPurpose GetSessionPurpose();
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // - - - - - Contruction and Destruction - - - - -
        // Only constructor
        SettingsManager(View* _view, Presenter* _presenter);
        
        /// \brief To be called from the Miam::Model when it is being
        /// constructed
        void CompleteInitialisation(Model* _model);
        
        
        // - - - - - Callbacks from View, retransmitted - - - - -
        void OnInOutChannelsCountChanged(int inputsCount, int outputsCount);
        void OnInOutNamesDisplayedChanged(bool areInputNamesVisible, bool areOutputNamesVisible);
        void OnUdpPortChanged(int udpPort);
        void OnIpAddressChanged(std::string ipAddress);
        // - - - - - Callbacks from View, concerning Presenter - - - - -
        void OnAllowKeyboardEdition(bool allow);
        
        // - - - - - - Property tree - - - - - -
        // - - - - - - For XML import/export, or Model data update - - - - - -
        /// \brief Returns the property tree describing the settings data
        ///
        /// This sub-tree does not know its own "master name tag" = <settings>
        std::shared_ptr<bptree::ptree> GetTree();
        /// \brief Sets internal data, and sets the View from given data tree.
        ///
        /// No checks performed on model-related data
        void SetFromTree(bptree::ptree& tree);

        
    };
    
    
    
}

