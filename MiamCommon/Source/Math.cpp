/*
  ==============================================================================

    Math.cpp
    Created: 7 Apr 2016 3:45:50pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "Math.h"

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