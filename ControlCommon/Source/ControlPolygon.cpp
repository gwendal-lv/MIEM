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
