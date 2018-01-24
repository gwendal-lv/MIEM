/*
  ==============================================================================

    InteractiveEllipse.cpp
    Created: 2 Feb 2017 11:52:00am
    Author:  ayup1

  ==============================================================================
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

void InteractiveEllipse::init()
{
	// foyer, points touchables, ..
    //...
    isRound = false;
}

void InteractiveEllipse::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
	DrawableEllipse::CanvasResized(_parentCanvas);

	// Finally, we update sub triangles
	
	computeSurface(); // mettre dans updateSubTriangle?
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
