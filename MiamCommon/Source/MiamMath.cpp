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

#include <cmath>
#include <deque>


#include "boost/geometry/algorithms/intersection.hpp"

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


double Math::ComputePositiveAngle(bpt p)
{
	double angle;
	if (p.get<0>() == 0.0) // To avoid using try/catch blocks
	{
		if (p.get<1>() > 0.0)
			angle = double_Pi / 2;
		else
			angle = -double_Pi / 2;
	}
	else // Usual atan computing
	{
		double ratio = p.get<1>() / p.get<0>();
		if (p.get<0>() > 0.0)
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
	// Line (AB) : a.x + b.y + c = 0
	// Line (CD) : alpha.X + Beta.Y + Gamma = 0
	double a, b, c, alpha, beta, gamma;
	if (A.getX() == B.getX()) // polygon has a perfect vertical side
	{
		a = 1.0;
		b = 0.0;
		c = -A.getX();
	}
	else // we can use usual affin function formulas
	{
		a = -(B.getY() - A.getY()) / (B.getX() - A.getX());
		b = 1.0;
		c = -A.getY() + A.getX() * (B.getY() - A.getY()) / (B.getX() - A.getX());
	}
	if (C.getX() == D.getX()) // vertical side
	{
		alpha = 1.0;
		beta = 0.0;
		gamma = -C.getX();
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

bpt Math::ComputeIntersectionPoint(bpt A, bpt B, bpt C, bpt D)
{
	boost::geometry::model::segment<bpt> L1(A,B), L2(C,D);
	std::deque<bpt> J;//bpt J;
	boost::geometry::intersection(L1, L2, J);
	// Planar lines cartesian representations (easier to compute intersection)
	// Line (AT) : a.x + b.y + c = 0
	// Line (BC) : alpha.X + Beta.Y + Gamma = 0
	double a, b, c, alpha, beta, gamma;
	if (A.get<0>() == B.get<0>()) // polygon has a perfect vertical side
	{
		a = 1.0;
		b = 0.0;
		c = -B.get<0>();
	}
	else // we can use usual affin function formulas
	{
		a = -(A.get<1>() - B.get<1>()) / (A.get<0>() - B.get<0>());
		b = 1.0;
		c = -B.get<1>() + B.get<0>() * (A.get<1>() - A.get<1>()) / (A.get<0>() - B.get<0>());
	}
	if (C.get<0>() == D.get<0>()) // vertical side
	{
		alpha = 1.0;
		beta = 0.0;
		gamma = -D.get<0>();
	}
	else // affin description
	{
		alpha = -(D.get<1>() - C.get<1>()) / (D.get<0>() - C.get<0>());
		beta = 1.0;
		gamma = -C.get<1>() + C.get<0>() * (D.get<1>() - C.get<1>()) / (D.get<0>() - C.get<0>());
	}

	// Intersection computing : simple 2d matrix inversion
	double determinant = a*beta - alpha*b; // It just can't be zero, if the center stays INSIDE the polygon
	bpt I = bpt((-c * beta + gamma * b) / determinant,
		(alpha * c - a * gamma) / determinant);

	assert(false); // Fonction bugguée, GUILLAUME C POUR TOI : à re-transcrire devient la version avec points Juce (qui est maintenant corrigée depuis le 17/10/2017). Contient aussi du code inutile juste là-dessous.... -> ???");

	return I;
}


