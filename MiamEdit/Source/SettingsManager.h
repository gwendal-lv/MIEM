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
#include "InterpolationTypes.h"

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
        
        /// \brief Configures the GUI for it to correspond to current session's Miam::AppPurpose
        void ConfigureGuiForSessionPurpose();
        
        
        // ----- Events from the Presenter itself -----
        /// \brief Updates the data that may have been modified during AppMode
        void OnEnterSettingsEdition();
        /// \brief Traite les objets qui ne déclenchent une action qu'à la fermeture du mode
        /// (et pas en direct suite à des modifs de l'utilisateur)
        void OnLeaveSettingsEdition(); // pas d'arbre retourné
        
        
        // - - - - - Callbacks from View concerning Model, retransmitted - - - - -
        // On a 2 grands types de données :
        // - celles qui sont retransmises en direct lorsque l'instant
        // du choix utilisateur est facilement identifiable (changement de
        // slider, choix de combo box, etc...)
        // - celles qui sont transmises lorsqu'on quitte le mode, typiquement les
        // text editors. Mais quand on édite, on met en gras les valeurs OK pour informer user
        
        void OnInOutChannelsCountChanged(int inputsCount, int outputsCount);
        void OnInOutNamesDisplayedChanged(bool areInputNamesVisible, bool areOutputNamesVisible);
        void OnInterpolationTypeChanged(InterpolationType type);
        private : // sont maintenant cachés, inaccessibles depuis l'extérieur
        void OnUdpPortChanged(int udpPort);
        void OnIpAddressChanged(std::string ipAddress);
        public :
        
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

