/*
  ==============================================================================

    CartesianLine.cpp
    Created: 8 Apr 2016 11:48:11am
    Author:  Gwendal Le Vaillant

  ==============================================================================
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