/*
  ==============================================================================

    Math.cpp
    Created: 7 Apr 2016 3:45:50pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "MiamMath.h"

using namespace Miam;


int Math::Modulo(int inputValue, int modulo)
{
    if (modulo < 0)
        return -1;
    
    else
    {
        int value = inputValue % modulo;
        while(value<0)
            value += modulo;
        
        return value;
    }
}




double Math::ComputePositiveAngle(Point<double> p)
{
    double angle;
    if (p.getX() == 0.0) // To avoid using try/catch blocks
    {
        if (p.getY() > 0.0)
            angle = double_Pi / 2;
        else
            angle = -double_Pi / 2;
    }
    else // Usual atan computing
    {
        double ratio = p.getY() / p.getX();
        if (p.getX() > 0.0)
            angle = std::atan(ratio);
        else
        {
            angle = double_Pi + std::atan(ratio);
        }
    }
    // Recalibrating into [0;2pi[
    if (angle < 0.0)
        angle += 2 * double_Pi;
    return angle;
}

Point<double> Math::ComputeIntersectionPoint(Point<double> A, Point<double> B, Point<double> C, Point<double> D)
{
	// Planar lines cartesian representations (easier to compute intersection)
	// Line (AT) : a.x + b.y + c = 0
	// Line (BC) : alpha.X + Beta.Y + Gamma = 0
	double a, b, c, alpha, beta, gamma;
	if (A.getX() == B.getX()) // polygon has a perfect vertical side
	{
		a = 1.0;
		b = 0.0;
		c = -B.getX();
	}
	else // we can use usual affin function formulas
	{
		a = -(A.getY() - B.getY()) / (A.getX() - B.getX());
		b = 1.0;
		c = -B.getY() + B.getX() * (A.getY() - A.getY()) / (A.getX() - B.getX());
	}
	if (C.getX() == D.getX()) // vertical side
	{
		alpha = 1.0;
		beta = 0.0;
		gamma = -D.getX();
	}
	else // affin description
	{
		alpha = -(D.getY() - C.getY()) / (D.getX() - C.getX());
		beta = 1.0;
		gamma = -C.getY() + C.getX() * (D.getY() - C.getY()) / (D.getX() - C.getX());
	}

	// Intersection computing : simple 2d matrix inversion
	double determinant = a*beta - alpha*b; // It just can't be zero, if the center stays INSIDE the polygon
	Point<double> I = Point<double>((-c * beta + gamma * b) / determinant,
		(alpha * c - a * gamma) / determinant);
	return I;
}