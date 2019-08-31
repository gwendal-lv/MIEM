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

#include "XmlUtils.h"

#include <vector>
#include <regex>

#include "boost/lexical_cast.hpp"

using namespace Miam;

size_t XmlUtils::CheckIndexes(const bptree::ptree& tree, const std::string& parentPath, const std::string& childrenPath, const std::string& indexPath)
{
    std::vector< unsigned int > indexesCounts;
    for (auto& child : tree.get_child(parentPath))
    {
        // tag name check
        if (child.first == childrenPath)
        {
            // indexes counts check
            size_t indexFound;
            try {
                indexFound = child.second.get<size_t>(indexPath);
            }
            catch(bptree::ptree_error &e) {
                throw XmlReadException("<" + childrenPath + "> is missing a <" + indexPath + " child: ", e);
            }
            // resize() n'efface aucun élément si on agrandit le vecteur
            if (indexFound >= indexesCounts.size())
                indexesCounts.resize(indexFound+1, 0); // pushing back zeros
            (indexesCounts[indexFound])++;
        }
        for (size_t i=0 ; i<indexesCounts.size() ; i++)
            if (indexesCounts[i] != 1)
                throw XmlReadException("Each <" + childrenPath + "> child index attribute (named '" + indexPath + "') must be found exactly 1 time ( here, index values within [ 0 ; " + boost::lexical_cast<std::string>(indexesCounts.size()-1) + " ] )");
    }
    
    return indexesCounts.size();
}


bool XmlUtils::IsIpv4AddressValid(const std::string& ipAddressToCheck)
{
    bool isIpValid = true;
    // Regex pour retrouver les composantes de l'adresse IP,
    // qu'on vérifiera 1 par 1 juste après
    std::regex get2IpComponentsRegex("(.*?)\\.(.*?)");
    std::smatch matchResults;
    std::string remainingIpToCheck = ipAddressToCheck;
    int ipComponentsCount = 0;
    // While we find ip components (sub strings)
    while ( std::regex_search(remainingIpToCheck, matchResults, get2IpComponentsRegex)
        && isIpValid
        && ipComponentsCount < 4)
    {
        // Check for validity
        isIpValid = isBetween_0_255( *( ++(matchResults.begin()) ) );
        // Next step
        if (isIpValid)
        {
            ipComponentsCount++;
            remainingIpToCheck = matchResults.suffix();
        }
    }
    // Test of remaining string (last component, alone)
    isIpValid = isBetween_0_255(remainingIpToCheck);
    if (isIpValid)
        ipComponentsCount++;
    // Ip components counting
    if (ipComponentsCount != 4)
        isIpValid = false;
    
    return isIpValid;
}
bool XmlUtils::isBetween_0_255(std::string str)
{
    bool isBetween = true;
    int ipComponent = -1;
    try {
        ipComponent = std::stoi(str);
    }
    catch (std::exception) {isBetween = false;}
    
    // Numeric values check
    if (ipComponent < 0 || 256 <= ipComponent)
        isBetween = false;
    
    return isBetween;
}

