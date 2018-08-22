/*
  ==============================================================================

    SubTriangle.cpp
    Created: 13 Apr 2016 5:08:53pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#include <cmath>
#include <vector>
#include <algorithm> // max

#include "SubTriangle.h"






using namespace Miam;


SubTriangle::SubTriangle(bpt _pointG, bpt _pointB, bpt _pointC)
{
	// Keep a reference to the center.
	bG = _pointG;

	// We don't know whether B and C are in the trigonometric order
	// or not. Inversion possible
	//bpt vectorGB, vectorGC;
	//std::list<bpolygon> vectorGB, vectorGC;
	bpt vectorGB(_pointB.get<0>() - _pointG.get<0>(), _pointB.get<1>() - _pointG.get<1>());
	bpt vectorGC(_pointC.get<0>() - _pointG.get<0>(), _pointC.get<1>() - _pointG.get<1>());
	

	double angle1 = Math::ComputePositiveAngle(vectorGB);
	double angle2 = Math::ComputePositiveAngle(vectorGC);
	if (std::abs(angle1 - angle2) > double_Pi) // A triangle cannot have such an angle
	{
		containsAngleZero = true;
		if (angle1 > angle2) // Sorted already : B before 0rad and C after 0rad
		{
			bB = _pointB;
			beginAngle = angle1;
			bC = _pointC;
			endAngle = angle2;
		}
		else // Inversion
		{
			bB = _pointC;
			beginAngle = angle2;
			bC = _pointB;
			endAngle = angle1;
		}
	}
	else
	{
		containsAngleZero = false;
		if (angle1 < angle2) // Sorted already
		{
			bB = _pointB;
			beginAngle = angle1;
			bC = _pointC;
			endAngle = angle2;
		}
		else // Inversion
		{
			bB = _pointC;
			beginAngle = angle2;
			bC = _pointB;
			endAngle = angle1;
		}
	}
	computeSurface();
}




// ====== Interaction Computing =====


bool SubTriangle::ContainsAngle(double angle)
{
    if (containsAngleZero)
    {
        if ((beginAngle <= angle) || (angle <= endAngle))
            return true;
        else
            return false;
    }
    else
    {
        if ((beginAngle <= angle) && (angle <= endAngle))
            return true;
        else
            return false;
    }
}



double SubTriangle::ComputeInteractionWeight(bpt T)
{
	/// T2 computing. T2 = projection (intersection) of (AT) on (BC)
	/// This is the only complicated thing to do... When having this projection,
	/// there is just a ratio left to compute.

    // Bugguée....
	//bpt T2 = Math::ComputeIntersectionPoint(bB, bC, bG, T);
    
    // On passe pour l'instant par l'ancienne version qui fonctionne...
    // On passe pour l'instant par l'ancienne version qui fonctionne...
    // On passe pour l'instant par l'ancienne version qui fonctionne...
    Point<double> G(bG.get<0>(), bG.get<1>());
    Point<double> B(bB.get<0>(), bB.get<1>());
    Point<double> C(bC.get<0>(), bC.get<1>());
    Point<double> ptJuceT(T.get<0>(), T.get<1>());
    Point<double> pointJuceT2 = Math::ComputeIntersectionPoint(G, ptJuceT, B, C);
    bpt T2(pointJuceT2.getX(), pointJuceT2.getY());
    // On passe pour l'instant par l'ancienne version qui fonctionne...
    // On passe pour l'instant par l'ancienne version qui fonctionne...
    // On passe pour l'instant par l'ancienne version qui fonctionne...
    
	// Weight computing... FINALLY, just a length ratio
	
	return (1.0 - std::sqrt(pow((boost::geometry::distance(T, bG)),2) / pow(boost::geometry::distance(T2, bG),2)));
}

void SubTriangle::computeSurface()
{
	double g = sqrt(pow(bB.get<0>() - bC.get<0>(), 2) + pow(bB.get<1>() - bC.get<1>(), 2));
	double b = sqrt(pow(bG.get<0>() - bC.get<0>(), 2) + pow(bG.get<1>() - bC.get<1>(), 2));
	double c = sqrt(pow(bB.get<0>() - bG.get<0>(), 2) + pow(bB.get<1>() - bG.get<1>(), 2));

	double s = (g + b + c) / 2;

	surface = sqrt(s*(s - g)*(s - b)*(s - c));
	
}

double SubTriangle::getSurface()
{
	return surface;
}

double SubTriangle::GetLongestDistanceFromG() const
{
    return std::max( boost::geometry::distance(bG, bB), boost::geometry::distance(bG, bC) );
}


