/*
  ==============================================================================

    InterpolationTypes.cpp
    Created: 11 Mar 2018 8:26:08pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InterpolationTypes.h"
#include "MiamExceptions.h"

#include "boost/algorithm/string.hpp" // case-insensitive string comparisons

using namespace Miam;

const char* const InterpolationTypes::interpolationNames[];
const char* const InterpolationTypes::interpolationShortNames[];



InterpolationType InterpolationTypes::ParseName(std::string interpolationName)
{
    // On teste l'égalité avec toutes les chaînes de caractèrse possibles...
    // Ça crée bcp de strings et n'est pas très optimisé, mais pas critique pour nous...
    InterpolationType parsedType = InterpolationType::None;
    for (int i = (int)(InterpolationType::None) + 1 ;
         (i < (int)InterpolationType::InterpolationTypesCount) && (parsedType == InterpolationType::None) ;
         i ++)
    {
        if (boost::iequals(interpolationName,
                           std::string(interpolationShortNames[i])))
            parsedType = (InterpolationType) i;
    }
    
    if (parsedType != InterpolationType::None)
        return parsedType;
    else
        throw ParseException("The string '" + interpolationName + "' cannot be parsed into a Miam::InterpolationType");
}
