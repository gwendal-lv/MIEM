//
//  DrawablePolygon.cpp
//  MiamEdit
//
//  Created by Gwendal Le Vaillant on 18/02/2016.
//
//

#include "DrawablePolygon.h"

#include "SceneCanvasComponent.h"

#include "XmlUtils.h"

using namespace Miam;

DrawablePolygon::DrawablePolygon(bptree::ptree & areaTree)
:
DrawableArea(areaTree)
{
    // Vérification préliminaire de présence des points du polygone
    size_t pointsCount = XmlUtils::CheckIndexes(areaTree, "geometry", "point");
    // Pré-chargement de chaque point de contour de <geometry>
    std::vector< bpt > polygonPoints;
    polygonPoints.resize(pointsCount);
    for (auto& point : areaTree.get_child("geometry"))
    {
        auto index = point.second.get<size_t>("<xmlattr>.index");
        try {
            polygonPoints[index].set<0>( point.second.get<double>("<xmlattr>.x") );
            polygonPoints[index].set<1>( point.second.get<double>("<xmlattr>.y") );
        }
        catch (bptree::ptree_error &e) {
            throw XmlReadException("Point " + std::to_string(index) + ": ", e);
        }
    }
    // Ajout des points 1 par 1 dans le bon ordre
    for (auto& point : polygonPoints)
        contourPoints.outer().push_back(point);
    contourPoints.outer().push_back(polygonPoints[0]);// contour closing
    // Actualisation graphique
    createJucePolygon();
}

DrawablePolygon::DrawablePolygon(int64_t _Id) :
    DrawablePolygon(_Id, bpt(0.5f,0.5f), 3, 0.1f, Colours::darkgrey)
{
}


DrawablePolygon::DrawablePolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
    DrawableArea(_Id, _center, _fillColour)
{

    if (_canvasRatio > 1.0f) // ratio of an landscape-oriented window
    {
        xScale = 1.0f/_canvasRatio;
        yScale = 1.0f;
    }
    else // ratio of an portrait-oriented window
    {
        xScale = 1.0f;
        yScale = 1.0f*_canvasRatio;
    }
    
    // Computation of the points
    float currentAngle;
    for (int i=0; i<pointsCount ; i++)
    {
        currentAngle = 2.0f*float_Pi*(float)(i)/(float)(pointsCount);
		contourPoints.outer().push_back(bpt(center.get<0>() + radius*xScale*cosf(currentAngle),
			center.get<1>() + radius*yScale*sinf(currentAngle)));
    }
	 // to close the boost polygon
	contourPoints.outer().push_back(bpt(center.get<0>() + radius*xScale, center.get<1>()));

    // Definition of the Juce polygon
    createJucePolygon();
}


DrawablePolygon::DrawablePolygon(int64_t _Id, bpt _center, bpolygon& _bcontourPoints, Colour _fillColour) :
	DrawableArea(_Id, _center, _fillColour)
{
	contourPoints = _bcontourPoints;

	createJucePolygon();
}


// Construction helpers
void DrawablePolygon::createJucePolygon(int width, int height)
{
	if (keepRatio)
		recreateContourPoints(width, height);
    contour.clear();
	contour.startNewSubPath((float)contourPoints.outer().at(0).get<0>(), (float)contourPoints.outer().at(0).get<1>());
	for (size_t i = 1; i<contourPoints.outer().size(); i++)
		contour.lineTo((float)contourPoints.outer().at(i).get<0>(), (float)contourPoints.outer().at(i).get<1>());
	contour.closeSubPath();
    
    contour.applyTransform(AffineTransform::scale((float)width, (float)height));
}

void DrawablePolygon::recreateContourPoints(int width, int height)
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

// Default destructor
DrawablePolygon::~DrawablePolygon()
{
}






// Called by the parent component (which is a canvas)
void DrawablePolygon::Paint(Graphics& g)
{
    g.setColour(fillColour);
    g.setOpacity(enableLowOpacityMode ? getLowFillOpacity() : fillOpacity);
    g.fillPath(contour);
    
    g.setColour(contourColour);
    g.setOpacity(enableLowOpacityMode ? getLowFillOpacity() : fillOpacity);
    g.strokePath(contour, PathStrokeType(contourWidth));
    
    // Parent's drawings on top of these ones
    DrawableArea::Paint(g);
}


void DrawablePolygon::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    DrawableArea::CanvasResized(_parentCanvas);
    
    createJucePolygon(parentCanvas->getWidth(), parentCanvas->getHeight());
}


// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> DrawablePolygon::GetTree()
{
    auto inheritedTree = DrawableArea::GetTree();
    bptree::ptree geomeTree; // LOL.
    // Lecture du polygone boost
    std::vector<bpt> const& points = contourPoints.outer(); // ring
    for (size_t i=0 ; i<points.size()-1 ; i++) // so we don't read the last point
    {
        bptree::ptree pointTree;
        pointTree.put("<xmlattr>.index", i);
        pointTree.put("<xmlattr>.x", points[i].get<0>());
        pointTree.put("<xmlattr>.y", points[i].get<1>());
        geomeTree.add_child("point", pointTree);
    }
    inheritedTree->put_child("geometry", geomeTree);
    return inheritedTree;
}

    

