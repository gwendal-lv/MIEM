/*
  ==============================================================================

    SubTriangle.cpp
    Created: 13 Apr 2016 5:08:53pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#include <cmath>

#include "SubTriangle.h"

#include "MiamMath.h"


using namespace Miam;


SubTriangle::SubTriangle(Point<double> _pointG, Point<double> _pointB, Point<double> _pointC)
{
    // Keep a reference to the center.
    G = _pointG;
    
    // We don't know whether B and C are in the trigonometric order
    // or not. Inversion possible
    Point<double> vectorGB = _pointB - _pointG;
    Point<double> vectorGC = _pointC - _pointG;
    double angle1 = Math::ComputePositiveAngle(vectorGB);
    double angle2 = Math::ComputePositiveAngle(vectorGC);
    if (std::abs(angle1 - angle2) > double_Pi) // A triangle cannot have such an angle
    {
        containsAngleZero = true;
        if (angle1 > angle2) // Sorted already : B before 0rad and C after 0rad
        {
            B = _pointB;
            beginAngle = angle1;
            C = _pointC;
            endAngle = angle2;
        }
        else // Inversion
        {
            B = _pointC;
            beginAngle = angle2;
            C = _pointB;
            endAngle = angle1;
        }
    }
    else
    {
        containsAngleZero = false;
        if (angle1 < angle2) // Sorted already
        {
            B = _pointB;
            beginAngle = angle1;
            C = _pointC;
            endAngle = angle2;
        }
        else // Inversion
        {
            B = _pointC;
            beginAngle = angle2;
            C = _pointB;
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



double SubTriangle::ComputeInteractionWeight(Point<double> T)
{
    /// T2 computing. T2 = projection (intersection) of (AT) on (BC)
    /// This is the only complicated thing to do... When having this projection,
    /// just a ratio left to do.
    
    // Planar lines cartesian representations (easier to compute intersection)
    // Line (AT) : a.x + b.y + c = 0
    // Line (BC) : alpha.X + Beta.Y + Gamma = 0

	/*
    double a, b, c, alpha, beta, gamma;
    if (B.getX() == C.getX()) // polygon has a perfect vertical side
    {
        a = 1.0;
        b = 0.0;
        c = -B.getX();
    }
    else // we can use usual affin function formulas
    {
        a = -(C.getY() - B.getY()) / (C.getX() - B.getX());
        b = 1.0;
        c = -B.getY() + B.getX() * (C.getY() - B.getY()) / (C.getX() - B.getX());
    }
    if (G.getX() == T.getX()) // vertical side
    {
        alpha = 1.0;
        beta = 0.0;
        gamma = -G.getX();
    }
    else // affin description
    {
        alpha = -(T.getY() - G.getY()) / (T.getX() - G.getX());
        beta = 1.0;
        gamma = -G.getY() + G.getX() * (T.getY() - G.getY()) / (T.getX() - G.getX());
    }
    
    // Intersection computing : simple 2d matrix inversion
    double determinant = a*beta-alpha*b; // It just can't be zero, if the center stays INSIDE the polygon
    Point<double> T2 = Point<double>( (-c * beta + gamma * b) / determinant,
                                      (alpha * c - a * gamma) / determinant   );
    */
	Point<double> T2 = Math::ComputeIntersectionPoint(B, C, G, T);
    // Weight computing... FINALLY, just a length ratio
    return (1.0 - std::sqrt(T.getDistanceSquaredFrom(G) / T2.getDistanceSquaredFrom(G)));
}

void SubTriangle::computeSurface()
{
	double g = sqrt(pow(B.getX() - C.getX(), 2) + pow(B.getY() - C.getY(), 2));
	double b = sqrt(pow(G.getX() - C.getX(), 2) + pow(G.getY() - C.getY(), 2));
	double c = sqrt(pow(B.getX() - G.getX(), 2) + pow(B.getY() - G.getY(), 2));

	double s = (g + b + c) / 2;

	surface = sqrt(s*(s - g)*(s - b)*(s - c));
	
}

double SubTriangle::getSurface()
{
	return surface;
}



