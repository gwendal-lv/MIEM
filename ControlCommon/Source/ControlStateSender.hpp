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

#ifndef CONTROLSTATESENDER_H_INCLUDED
#define CONTROLSTATESENDER_H_INCLUDED

#include <memory>
#include <string>
#include "ControlState.hpp"


#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


namespace Miam
{
    
    
    
    
    /// \brief An abstract class to be inherited by any class that is able
    /// to send (midi, OSC, network, camel, pony, ...) some MIEM Control (generic, spat, etc...)
    /// data.
    template<typename T>
    class ControlStateSender
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :

        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        virtual std::string GetTypeAsString() const = 0;
        virtual std::string GetAddressAsString() const = 0;

        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
       
        
        // - - - - - Construction / destruction - - - - -
        virtual ~ControlStateSender() {}
        
        
        // - - - - - Communication - - - - -
        virtual void SendState(ControlState<double>& state) = 0;
        /// \brief Envoie un état de manière en optimisant la bande passante du réseau : on
        /// ne transmet que les paramètres qui ont suffisamment varié depuis le dernier envoi
        virtual void SendStateModifications(ControlState<T>& state) = 0;
        
        // - - - - - XML import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetConfigurationTree()
        {
            auto configurationTree = std::make_shared<bptree::ptree>();
            configurationTree->put("<xmlattr>.type", GetTypeAsString());
            return configurationTree;
        }
        virtual void SetConfigurationFromTree(bptree::ptree& /*tree*/, bool sendExceptionOnMissingData = false)
		{
			boost::ignore_unused(sendExceptionOnMissingData); 
		}
    };
    
}



#endif  // CONTROLSTATESENDER_H_INCLUDED
