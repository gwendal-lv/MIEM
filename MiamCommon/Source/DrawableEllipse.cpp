/*
  ==============================================================================

    DrawableEllipse.cpp
    Created: 2 Feb 2017 2:17:07pm
    Author:  ayup1

  ==============================================================================
*/


#include <MiamMath.h> // plutôt que cmath simple, sinon problème avec M_PI sous VS2015

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
	
	xScale = (float) computeXScale(_canvasRatio);
	yScale = (float)computeYScale(_canvasRatio);

	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() + (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() + (b / 2)*yScale));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() - (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));

	float aScaled = (float)(a / 2.0) * xScale; // demi-axe, normalisé, rescalé si keepratio
	float bScaled = (float)(b / 2.0) * yScale; // demi-axe , (idem)

	int verticesCount = (ellipseVerticesCount + 1);
	//vertex_buffer = new float[verticesCount * 3]; // tout sommets + le centre * (x,y,z)
	vertex_buffer.resize(verticesCount * 3);
	vertex_buffer[0] = 0.0f;
	vertex_buffer[1] = 0.0f;
	vertex_buffer[2] = 0.0f;

	vertex_buffer[3] = aScaled * (float)std::cos(0.0);
	vertex_buffer[4] = bScaled * (float)std::sin(0.0);
	vertex_buffer[5] = 0.0f;

	for (int i = 1; i<ellipseVerticesCount; i++)
	{
		double normalizedAngle = (double)(i) / (double)(ellipseVerticesCount);
		vertex_buffer[(i + 1) * 3]     = aScaled * (float)std::cos(2.0 * M_PI * normalizedAngle);
		vertex_buffer[(i + 1) * 3 + 1] = bScaled * (float)std::sin(2.0 * M_PI * normalizedAngle);
		vertex_buffer[(i + 1) * 3 + 2] = 0.0f;
		
	}

	int indexCount = ellipseVerticesCount * 3;
	index_buffer.resize(indexCount);//index_buffer = new int[indexCount];
	for (int i = 0; i < ellipseVerticesCount; ++i)
	{
		index_buffer[i * 3] = i + 1;
		index_buffer[i * 3 + 1] = 0;
		index_buffer[i * 3 + 2] = i + 2;
	}

	modelParameters = Vector3D<float>(0.0f, 0.0f, 0.0f);

	createJucePolygon();
}

int DrawableEllipse::GetVerticesCount()
{
	return 0;// verticesCount;
}

int DrawableEllipse::GetIndexCount()
{
	return 0;//indexCount;
}

bool DrawableEllipse::hasVerticesChanged()
{
	return verticesChanged;
}

bool DrawableEllipse::hasPositionChanged()
{
	return positionChanged;
}

std::vector<float> DrawableEllipse::GetVertices()
{
	if (vertex_buffer.size() > 0)
		return vertex_buffer;
	return std::vector<float>();//nullptr;
}

std::vector<float> DrawableEllipse::GetOutline()
{
	return std::vector<float>();
}

std::vector<int> DrawableEllipse::GetIndex()
{
	if (index_buffer.size() > 0)
		return index_buffer;
	return std::vector<int>();
}

Vector3D<float> Miam::DrawableEllipse::GetModelParameters()
{
	return modelParameters;
}

void DrawableEllipse::createJucePolygon(int width, int height)
{
	contour.clear();

	if (keepRatio)
	{
		recreateContourPoints(width,height);
	}

    // À faire "à la main" avec un certain nombre de points... Sinon Juce en utilise 32 par défaut
    float centerX = (float)center.get<0>();
    float centerY = (float)center.get<1>();
    float aScaled = (float)(a/2.0) * xScale; // demi-axe, normalisé, rescalé si keepratio
    float bScaled = (float)(b/2.0) * yScale; // demi-axe , (idem)
    contour.startNewSubPath(centerX + aScaled * (float)std::cos(0.0),
                            centerY + bScaled * (float)std::sin(0.0));
    for (int i = 1; i<ellipseVerticesCount; i++)
    {
        double normalizedAngle = (double)(i)/(double)(ellipseVerticesCount);
        contour.lineTo(centerX + aScaled * (float)std::cos(2.0 * M_PI * normalizedAngle),
                       centerY + bScaled * (float)std::sin(2.0 * M_PI * normalizedAngle));
    }
    // Est-ce qu'il faut ABSOLUMENT faire le tour complet avec début=fin, avant de fermer le contour ?
    contour.closeSubPath();
    
    // ancien contour : ellipse Juce directe
    //contour.addEllipse((float)center.get<0>() -((float)a*xScale/2), (float)center.get<1>() -((float)b*yScale/2), (float)a*xScale, (float)b*yScale);

	contour.applyTransform(AffineTransform::scale((float)width, (float)height));

	contour.applyTransform(AffineTransform::rotation((float)rotationAngle, (float)center.get<0>() * (float)width, (float)center.get<1>() * (float)height));
}

DrawableEllipse::~DrawableEllipse()
{
}

void DrawableEllipse::setVerticesCount(int newVerticesCount)
{
	ellipseVerticesCount = newVerticesCount;
}

int DrawableEllipse::getVerticesCount()
{
	return ellipseVerticesCount;
}

void DrawableEllipse::setIsFilled(bool shouldBeFilled)
{
	isFilled = shouldBeFilled;
}


// Called by the parent component (which is a canvas)
void DrawableEllipse::Paint(Graphics& g)
{
	if (isFilled)
	{
		g.setColour(fillColour);
		g.setOpacity(GetAlpha());
		g.fillPath(contour);
	}

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
	
	newXScale = (float) computeXScale(newCanvasRatio);
	newYScale = (float) computeYScale(newCanvasRatio);

	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invTr(-center.get<0>(), -center.get<1>());
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> Tr(center.get<0>(), center.get<1>());

	bpolygon newContourPoints;
	boost::geometry::transform(contourPoints, newContourPoints, invTr);

	for (int i = 0; i < (int)newContourPoints.outer().size(); ++i)
		boost::geometry::multiply_point(newContourPoints.outer().at(i), bpt(newXScale / xScale, newYScale / yScale));

	contourPoints.clear();
	boost::geometry::transform(newContourPoints, contourPoints, Tr);

	xScale = newXScale;
	yScale = newYScale;
}

double DrawableEllipse::computeXScale(float _canvasRatio)
{
	return _canvasRatio > 1.0f ? 1.0f/_canvasRatio : 1.0f;
}

double DrawableEllipse::computeYScale(float _canvasRatio)
{
	return _canvasRatio > 1.0f ? 1.0f : 1.0f / _canvasRatio;
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



