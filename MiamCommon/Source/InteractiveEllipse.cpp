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

#include "InteractiveEllipse.h"
#include "SceneCanvasComponent.h"
#include "MiamMath.h"
using namespace Miam;

InteractiveEllipse::InteractiveEllipse(bptree::ptree & areaTree)
:
DrawableEllipse(areaTree)
{
    // Valeurs par défaut
    init();
    
    // Données facultatives (qui ont une valeur par défaut)
    try {
        isRound = areaTree.get<bool>("is_round");
    }
    catch (bptree::ptree_error&) {}
}

InteractiveEllipse::InteractiveEllipse(int64_t _Id) 
	: DrawableEllipse(_Id)
{
	init();
}
std::shared_ptr<IDrawableArea> InteractiveEllipse::Clone()
{
    auto clone = std::make_shared<InteractiveEllipse>(*this);
    clone->onCloned();
    return clone;
}
void InteractiveEllipse::onCloned()
{
    // On n'appelle que le "onCloned" de la forme générique.
    InteractiveArea::onCloned();
    // Pas le "onCloned" du parent graphique Drawable (parent concret)
}


InteractiveEllipse::InteractiveEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio)
	: DrawableEllipse(_Id, _center, _a, _b, _fillColour, _canvasRatio)
{
	init();
}
InteractiveEllipse::InteractiveEllipse(int64_t _Id, bpt _center, int _radiusInPixels, Colour _fillColour)
     : DrawableEllipse(_Id, _center, _radiusInPixels, _fillColour)
{
    init();
}

void InteractiveEllipse::init()
{
	// foyer, points touchables, ..
    //...
    isRound = false;
}

void InteractiveEllipse::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    DrawableEllipse::CanvasResized(_parentCanvas);
    InteractiveArea::CanvasResized(_parentCanvas);
	
	computeSurface(); 
}

void InteractiveEllipse::computeSurface()
{
	surface = a * b * M_PI;
}

bool InteractiveEllipse::HitTest(bpt T) const
{
	return (contour.contains((float)T.get<0>(),
                             (float)T.get<1>()));
}

double InteractiveEllipse::ComputeInteractionWeight(bpt /*T*/)
{
	// calculer l'interaction
	return 0.0;
}

void InteractiveEllipse::SetIsRound(bool _isround)
{
	isRound = _isround;
    if (isRound)
        KeepRatio(true); // forcé
}


// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> InteractiveEllipse::GetTree()
{
    auto inheritedTree = DrawableEllipse::GetTree();
    
    inheritedTree->put("is_round", isRound);
    
    // Renvoi de l'arbre complété par cette classe dérivée
    return inheritedTree;
}
