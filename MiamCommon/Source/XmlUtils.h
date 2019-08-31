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
