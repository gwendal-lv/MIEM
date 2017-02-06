/*
  ==============================================================================

    CartesianLine.cpp
    Created: 8 Apr 2016 11:48:11am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "CartesianLine.h"


using namespace Miam;





CartesianLine::CartesianLine(Point<double> point1, Point<double> point2)
{
    if (point2.x // cas particulier : droite verticale
        == point1.x) // en pratique, presque jamais sur des "double"....
    {
        b = 0.0;
        a = 1.0;
        c = -point1.x;
    }
    else
    {
        b = 1.0;
        a = (point1.y - point2.y) / (point2.x - point1.x);
        c = - 0.5 * (a * (point2.x+point1.x) + point2.y + point1.y);
    }

	//DBG((String)a + "x+" + (String)b + "y" +(String)c + " = 0");
}




bool CartesianLine::PointWentThrough(Point<double> previousLocation, Point<double> nextLocation)
{
    double resultAnciennePos = a*previousLocation.x + b*previousLocation.y + c;
    double resultNouvellePos = a*nextLocation.x + b*nextLocation.y + c;
    
    // Si les 2 résultats sont du même signe, c'est qu'on est resté dans le même demi-plan (demi-plan découpé par rapport à notre droite)
    // Sinon, c'est qu'on est passé à travers la ligne (donc on renvoie vrai)
    return ( (resultAnciennePos*resultNouvellePos) <= 0 );
}