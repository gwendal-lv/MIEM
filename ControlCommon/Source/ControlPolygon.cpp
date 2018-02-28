/*
  ==============================================================================

    ControlPolygon.cpp
    Created: 28 May 2017 11:06:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "ControlPolygon.h"

using namespace Miam;

// ========== CONSTRUCTION ==========

ControlPolygon::ControlPolygon(bptree::ptree & areaTree)
:
EditablePolygon(areaTree) //, ControlArea(areaTree)
{
    init();
}

ControlPolygon::ControlPolygon(int64_t _Id) :
EditablePolygon(_Id),
ControlArea()
{
    init();
}

ControlPolygon::ControlPolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
EditablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio),
ControlArea()
{
    init();
}

ControlPolygon::ControlPolygon(int64_t _Id, bpt _center, bpolygon& _contourPoints, Colour _fillColour) :
EditablePolygon(_Id, _center, _contourPoints, _fillColour),
ControlArea()
{
    init();
}

void ControlPolygon::init()
{
    ControlArea::OnVirtualBaseClassesContructed();
}



// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> ControlPolygon::GetTree()
{
    auto inheritedTree = EditablePolygon::GetTree();
    inheritedTree->add_child("spatstate", *getStateTree());
    return inheritedTree;
}
