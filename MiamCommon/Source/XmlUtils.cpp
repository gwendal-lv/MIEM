/*
 ==============================================================================
 
 XmlUtils.cpp
 Created: 12 Jun 2017 9:36:59am
 Author:  Gwendal Le Vaillant
 
 ==============================================================================
 */

#include "XmlUtils.h"

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
                throw XmlReadException("Each <" + childrenPath + "> child index attribute (named '" + indexPath + "') must be found exactly 1 time ( here, index values within [ 0 ; " + std::to_string(indexesCounts.size()-1) + " ] )");
    }
    
    return indexesCounts.size();
}

