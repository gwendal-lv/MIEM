/*
  ==============================================================================

    DrawableEllipse.cpp
    Created: 2 Feb 2017 2:17:07pm
    Author:  ayup1

  ==============================================================================
*/


#include <cmath>

#include "DrawableEllipse.h"
#include "SceneCanvasComponent.h"

#include "XmlUtils.h"


using namespace Miam;

DrawableEllipse::DrawableEllipse(bptree::ptree & areaTree)
:
DrawableArea(areaTree)
{
    // Chargement des quelques données spécifiques à l'ellipse
    try {
        a = areaTree.get<double>("geometry.axes.<xmlattr>.a");
        b = areaTree.get<double>("geometry.axes.<xmlattr>.b");
    }
    catch (bptree::ptree_error &e) {
        throw XmlReadException("DrawableEllipse construction : axes lengths a and/or b cannot be read : ", e);
    }
    try {
        rotationAngle = areaTree.get<double>("geometry.rotation");
    }
    catch (bptree::ptree_error&) {
        // Node non-obligatoire -> Valeur par défaut
        rotationAngle = 0.0;
    }
    
    // Actualisation graphique
    createJucePolygon();
}

DrawableEllipse::DrawableEllipse(int64_t _Id) :
	DrawableEllipse(_Id, bpt(0.5f, 0.5f),0.2f,0.2f,Colours::darkgrey)
{
	rotationAngle = 0.0;
}

DrawableEllipse::DrawableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
	DrawableArea(_Id,_center,_fillColour), a(_a), b(_b)
{
	rotationAngle = 0.0;
	//float xScale, yScale;
	if (_canvasRatio > 1.0f) // ratio of an landscape-oriented window
	{
		xScale = 1.0f / _canvasRatio;
		yScale = 1.0f;
	}
	else // ratio of an portrait-oriented window
	{
		xScale = 1.0f;
		yScale = 1.0f*_canvasRatio;
	}

	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() + (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() + (b / 2)*yScale));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() - (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));

	createJucePolygon();
}

void DrawableEllipse::createJucePolygon(int width, int height)
{
	contour.clear();

	if (keepRatio)
	{
		
		recreateContourPoints(width,height);
	}

	contour.addEllipse((float)center.get<0>() -((float)a*xScale/2), (float)center.get<1>() -((float)b*yScale/2), (float)a*xScale, (float)b*yScale);

	contour.applyTransform(AffineTransform::scale((float)width, (float)height));

	contour.applyTransform(AffineTransform::rotation((float)rotationAngle, (float)center.get<0>() * (float)width, (float)center.get<1>() * (float)height));
}

DrawableEllipse::~DrawableEllipse()
{
}

// Called by the parent component (which is a canvas)
void DrawableEllipse::Paint(Graphics& g)
{
	g.setColour(fillColour);
    g.setOpacity(GetAlpha());
	g.fillPath(contour);

	g.setColour(contourColour);
    g.setOpacity(GetAlpha());
	g.strokePath(contour, PathStrokeType(contourWidth));

	// Parent's drawings on top of these ones
	DrawableArea::Paint(g);
}

void DrawableEllipse::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
	DrawableArea::CanvasResized(_parentCanvas);

	createJucePolygon(parentCanvas->getWidth(), parentCanvas->getHeight());
        
    // Pixel contour points
    contourPointsInPixels.clear();
    boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(parentCanvas->getWidth(), parentCanvas->getHeight());
    boost::geometry::transform(contourPoints, contourPointsInPixels, scaler);

}

void DrawableEllipse::recreateContourPoints(int width, int height)
{
	// first calculate the distances and angles so we could apply recreate the same polygon, but with the new xScale and yScale
	float newCanvasRatio = (float)width / (float)height;
	float newXScale;
	float newYScale;
	if (newCanvasRatio > 1.0f) // ratio of an landscape-oriented window
	{
		newXScale = 1.0f / newCanvasRatio;
		newYScale = 1.0f;
	}
	else // ratio of an portrait-oriented window
	{
		newXScale = 1.0f;
		newYScale = 1.0f*newCanvasRatio;
	}

	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invTr(-center.get<0>(), -center.get<1>());
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> Tr(center.get<0>(), center.get<1>());

	bpolygon newContourPoints;
	boost::geometry::transform(contourPoints, newContourPoints, invTr);

	for (int i = 0; i < newContourPoints.outer().size(); ++i)
		boost::geometry::multiply_point(newContourPoints.outer().at(i), bpt(newXScale / xScale, newYScale / yScale));

	contourPoints.clear();
	boost::geometry::transform(newContourPoints, contourPoints, Tr);

	xScale = newXScale;
	yScale = newYScale;
}



// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> DrawableEllipse::GetTree()
{
    auto inheritedTree = DrawableArea::GetTree();
    bptree::ptree geomeTree; // LOL.
    bptree::ptree axesTree;
    bptree::ptree rotationTree;
    
    // Écriture des paramètres (non-négligeables seulement)
    axesTree.put("<xmlattr>.a", a);
    axesTree.put("<xmlattr>.b", b);
    geomeTree.add_child("axes", axesTree);
    if ( std::abs(rotationAngle) > 0.000001 )
        geomeTree.put("rotation", rotationAngle);

    // Renvoi de l'arbre complété par cette classe dérivée
    inheritedTree->put_child("geometry", geomeTree);
    return inheritedTree;
}



