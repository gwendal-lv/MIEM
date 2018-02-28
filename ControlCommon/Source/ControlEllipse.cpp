/*
  ==============================================================================

    ControlEllipse.cpp
    Created: 28 May 2017 11:06:09am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "ControlEllipse.h"

using namespace Miam;


ControlEllipse::ControlEllipse(int64_t _Id) :
EditableEllipse(_Id)
{
    init();
}

ControlEllipse::ControlEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
EditableEllipse(_Id,_center,_a,_b,_fillColour,_canvasRatio)
{
    init();
}


void ControlEllipse::init()
{
    ControlArea::OnVirtualBaseClassesContructed();
}
