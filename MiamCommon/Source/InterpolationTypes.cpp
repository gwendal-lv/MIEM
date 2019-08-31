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

#include "InterpolationTypes.h"
#include "MiamExceptions.h"

#include "boost/algorithm/string.hpp" // case-insensitive string comparisons

using namespace Miam;

// Définition des variables statiques déclarées dans le header... ??
// VS2015 disait "il faut initialiser directement les types const" lorsqu'il
// n'y avait pas "constexpr". Avec le constexpr, ça compile....
constexpr const char* const InterpolationTypes::interpolationNames[];
constexpr const char* const InterpolationTypes::interpolationShortNames[];
constexpr const char* const ParamInterpolationTypes::interpolationNames[];
constexpr const char* const ParamInterpolationTypes::interpolationShortNames[];



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



ParamInterpolationType ParamInterpolationTypes::ParseName(std::string interpolationName)
{
    // On teste l'égalité avec toutes les chaînes de caractèrse possibles...
    // Ça crée bcp de strings et n'est pas très optimisé, mais pas critique pour nous...
    ParamInterpolationType parsedType = ParamInterpolationType::None;
    for (int i = (int)(ParamInterpolationType::None) + 1 ;
         (i < (int)ParamInterpolationType::InterpolationTypesCount) && (parsedType == ParamInterpolationType::None) ;
         i ++)
    {
        if (boost::iequals(interpolationName,
                           std::string(interpolationShortNames[i])))
            parsedType = (ParamInterpolationType) i;
    }
    
	if (parsedType != ParamInterpolationType::None)
		return parsedType;
	// Si problème de parse -> interp linéaire classique (pour rétro compatibilité...)
	else
		return ParamInterpolationType::Independant_Linear;
        //throw ParseException("The string '" + interpolationName + "' cannot be parsed into a Miam::ParamInterpolationType");
}
