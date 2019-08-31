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

#include "CartesianLine.h"


using namespace Miam;





CartesianLine::CartesianLine(bpt point1, bpt point2)
{
    if (point2.get<0>() // cas particulier : droite verticale
        == point1.get<0>()) // en pratique, presque jamais sur des "double"....
    {
        b = 0.0;
        a = 1.0;
        c = -point1.get<0>();
    }
    else
    {
        b = 1.0;
        a = (point1.get<1>() - point2.get<1>()) / (point2.get<0>() - point1.get<0>());
        c = - 0.5 * (a * (point2.get<0>() +point1.get<0>()) + point2.get<1>() + point1.get<1>());
    }

	//DBG((String)a + "x+" + (String)b + "y" +(String)c + " = 0");
}




bool CartesianLine::PointWentThrough(bpt previousLocation, bpt nextLocation)
{
    double resultAnciennePos = a*previousLocation.get<0>() + b*previousLocation.get<1>() + c;
    double resultNouvellePos = a*nextLocation.get<0>() + b*nextLocation.get<1>() + c;
    
    // Si les 2 résultats sont du même signe, c'est qu'on est resté dans le même demi-plan (demi-plan découpé par rapport à notre droite)
    // Sinon, c'est qu'on est passé à travers la ligne (donc on renvoie vrai)
    return ( (resultAnciennePos*resultNouvellePos) <= 0 );
}