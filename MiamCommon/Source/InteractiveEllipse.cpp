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

	
	// Pixel contour points
	contourPointsInPixels.clear();
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(parentCanvas->getWidth(), parentCanvas->getHeight());
	boost::geometry::transform(contourPoints, contourPointsInPixels, scaler);

	// Finally, we update sub triangles
	
	computeSurface(); // mettre dans updateSubTriangle?
}

void InteractiveEllipse::computeSurface()
{
	surface = a * b * M_PI;
}

bool InteractiveEllipse::HitTest(double x, double y)
{
	return (contour.contains((float)x, (float)y));
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
