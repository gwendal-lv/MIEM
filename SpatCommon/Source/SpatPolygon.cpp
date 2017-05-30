/*
  ==============================================================================

    SpatPolygon.cpp
    Created: 28 May 2017 11:06:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatPolygon.h"

using namespace Miam;

// ========== CONSTRUCTION ==========

SpatPolygon::SpatPolygon(int64_t _Id) :
EditablePolygon(_Id),
SpatArea()
{
    init();
}

SpatPolygon::SpatPolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
EditablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio),
SpatArea()
{
    init();
}

SpatPolygon::SpatPolygon(int64_t _Id, bpt _center, bpolygon& _contourPoints, Colour _fillColour) :
EditablePolygon(_Id, _center, _contourPoints, _fillColour),
SpatArea()
{
    init();
}

void SpatPolygon::init()
{
    SpatArea::OnVirtualBaseClassesContructed();
}
