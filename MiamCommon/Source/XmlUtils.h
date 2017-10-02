/*
  ==============================================================================

    XmlUtils.h
    Created: 12 Jun 2017 9:36:59am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <string>

#include "MiamExceptions.h"

namespace Miam
{
    class XmlUtils
    {
        public :
        
        /// \brief Checks if the 'childrenName' child nodes of the given property tree
        /// are properly indexed. Throws a Miam::XmlReadException if something absurd
        /// is found.
        ///
        /// i.e. detects the min and max indexes, and checks wether all indexes
        /// inbetween can be found.
        ///
        /// \return The number of elements (== max index + 1)
        static size_t CheckIndexes(const bptree::ptree& tree, const std::string& parentPath, const std::string& childrenPath, const std::string& indexPath = "<xmlattr>.index");
        
        static bool IsIpv4AddressValid(const std::string& ipAddressToCheck);
        private :
        static bool isBetween_0_255(std::string str);
        public :
        
    };
}
