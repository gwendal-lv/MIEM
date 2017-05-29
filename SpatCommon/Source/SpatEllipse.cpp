/*
  ==============================================================================

    SpatEllipse.cpp
    Created: 28 May 2017 11:06:09am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatEllipse.h"

using namespace Miam;


SpatEllipse::SpatEllipse(int64_t _Id) :
EditableEllipse(_Id)
{
    init();
}

SpatEllipse::SpatEllipse(int64_t _Id, Point<double> _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
EditableEllipse(_Id,_center,_a,_b,_fillColour,_canvasRatio)
{
    init();
}


void SpatEllipse::init()
{
    SpatArea::OnVirtualBaseClassesContructed();
}
