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

	// ajout de la forme et du contour !
	verticesBufferSize += (3 * numVerticesPolygon + 3 * numPointsPolygon);
	indicesBufferSize += (3 * numVerticesPolygon + 3 * 2 * numPointsPolygon);
	couloursBufferSize += (4 * numVerticesPolygon + 4 * numPointsPolygon);


	// resize des buffers
	vertices_buffer.resize(verticesBufferSize);
	indices_buffer.resize(indicesBufferSize);
	coulours_buffer.resize(couloursBufferSize);
	
	// indices pour dessiner la forme
	int decalage = DrawableArea::GetVerticesBufferSize();
	for (int i = 0; i < ellipseVerticesCount; ++i)
	{
		indices_buffer[3 * decalage + i * 3] = decalage + i + 1;
		indices_buffer[3 * decalage + i * 3 + 1] = decalage + 0;
		indices_buffer[3 * decalage + i * 3 + 2] = decalage + i + 2 > decalage + ellipseVerticesCount? decalage + 1 : decalage + i + 2;
	}
	for (int i = 3*ellipseVerticesCount; i <3 * numVerticesPolygon; ++i)
	{
		indices_buffer[3 * decalage + i] = 0;
	}

	for (int i = 0; i < ellipseVerticesCount; ++i)
	{
		coulours_buffer[4 * decalage + i * 4] = fillColour.getRed() / 255.0f;
		coulours_buffer[4 * decalage + i * 4 + 1] = fillColour.getGreen() / 255.0f;
		coulours_buffer[4 * decalage + i * 4 + 2] = fillColour.getBlue() / 255.0f;
		coulours_buffer[4 * decalage + i * 4 + 3] = GetAlpha();
	}
	for (int i = 4 * ellipseVerticesCount; i < 4 * numVerticesPolygon; ++i)
		coulours_buffer[4 * decalage + i] = 0.0f;

	// indices pour dessiner le contour
	int shapeBegin = DrawableArea::GetVerticesBufferSize();
	decalage += numVerticesPolygon;
	for (int i = 0; i < numPointsPolygon; ++i)
	{
		if (i < ellipseVerticesCount)
		{
			indices_buffer[3 * decalage + i * 6] = shapeBegin + i + 1;
			indices_buffer[3 * decalage + i * 6 + 1] = decalage + i;
			indices_buffer[3 * decalage + i * 6 + 3] = decalage + i + 1 >= decalage + ellipseVerticesCount ? decalage : decalage + i + 1;
			indices_buffer[3 * decalage + i * 6 + 2] = decalage + i + 1 >= decalage + ellipseVerticesCount ? decalage : decalage + i + 1;
			indices_buffer[3 * decalage + i * 6 + 4] = shapeBegin + i + 1;
			indices_buffer[3 * decalage + i * 6 + 5] = shapeBegin + i + 2 >= shapeBegin + ellipseVerticesCount + 1 ? shapeBegin + 1 : shapeBegin + i + 2;
		}
		else
		{
			indices_buffer[3 * decalage + i * 6] = 0;
			indices_buffer[3 * decalage + i * 6 + 1] = 0;
			indices_buffer[3 * decalage + i * 6 + 2] = 0;
			indices_buffer[3 * decalage + i * 6 + 3] = 0;
			indices_buffer[3 * decalage + i * 6 + 4] = 0;
			indices_buffer[3 * decalage + i * 6 + 5] = 0;
		}
	}

	for (int i = 0; i < numPointsPolygon; ++i)
	{
		coulours_buffer[4 * decalage + i * 4] = contourColour.getRed() / 255.0f;
		coulours_buffer[4 * decalage + i * 4 + 1] = contourColour.getGreen() / 255.0f;
		coulours_buffer[4 * decalage + i * 4 + 2] = contourColour.getBlue() / 255.0f;
		coulours_buffer[4 * decalage + i * 4 + 3] = contourColour.getAlpha() / 255.0f;
	}
	

	createJucePolygon();
}



int DrawableEllipse::GetIndicesBufferSize()
{
	return DrawableArea::GetIndicesBufferSize() + (3 * numVerticesPolygon + 3 * 2 * numPointsPolygon);
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

	//float dR = (float)sqrt(2) * contourWidth / 2.0f;
	//vertex_buffer[0] = centerInPixels.get<0>();
	//vertex_buffer[1] = centerInPixels.get<1>();
	//vertex_buffer[2] = 0.0f;
	//for (int i = 0; i < ellipseVerticesCount; i++)
	//{
	//	double normalizedAngle = (double)(i) / (double)(ellipseVerticesCount);
	//	vertex_buffer[3 + i * 3] = centerX + aScaled * (float)std::cos(2.0 * M_PI * normalizedAngle);
	//	vertex_buffer[3 + i * 3 + 1] = centerY + bScaled * (float)std::sin(2.0 * M_PI * normalizedAngle);
	//	vertex_buffer[3 + i * 3 + 2] = 0.0f;
	//	outline_vertex_buffer[i * 3] = centerX + (aScaled + dR) * (float)std::cos(2.0 * M_PI * normalizedAngle);
	//	outline_vertex_buffer[i * 3 + 1] = centerY + (bScaled + dR) * (float)std::sin(2.0 * M_PI * normalizedAngle);
	//	outline_vertex_buffer[i * 3 + 2] = 0.0f;
	//}
    
    // ancien contour : ellipse Juce directe
    //contour.addEllipse((float)center.get<0>() -((float)a*xScale/2), (float)center.get<1>() -((float)b*yScale/2), (float)a*xScale, (float)b*yScale);

	contour.applyTransform(AffineTransform::scale((float)width, (float)height));

	contour.applyTransform(AffineTransform::rotation((float)rotationAngle, (float)center.get<0>() * (float)width, (float)center.get<1>() * (float)height));
}

DrawableEllipse::~DrawableEllipse()
{
	
}

void DrawableEllipse::setVerticesCount(int /*newVerticesCount*/)
{
	ellipseVerticesCount = 32;//newVerticesCount;
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

void DrawableEllipse::RefreshOpenGLBuffers()
{
	DrawableArea::RefreshOpenGLBuffers();
	int aInPixels = (int)(a * (double)parentCanvas->getWidth() * xScale/2.0);
	int bInPixels = (int)(b * (double)parentCanvas->getWidth() * xScale/2.0);

	// forme
	int decalage = DrawableArea::GetVerticesBufferSize();
	float dR = (float)sqrt(2) * contourWidth / 2.0f;
	const float Cx = (float)centerInPixels.get<0>();
	const float Cy = (float)centerInPixels.get<1>();
	float Zoffset = mainZoffset + 0.0f;

	float* verticesPtr = &vertices_buffer[3 * decalage];

	verticesPtr[0] = Cx;
	verticesPtr[1] = Cy;
	verticesPtr[2] = 0.0f;
	verticesPtr += 3;
	for (int i = 0; i < ellipseVerticesCount; i++)
	{
		double normalizedAngle = (double)(i) / (double)(ellipseVerticesCount);
		verticesPtr[i * 3] = Cx + (float)aInPixels * (float)std::cos(2.0 * M_PI * normalizedAngle);
		verticesPtr[i * 3 + 1] = Cy + (float)bInPixels * (float)std::sin(2.0 * M_PI * normalizedAngle);
		verticesPtr[i * 3 + 2] = Zoffset;
	}
	for (int i = 3 * decalage + 3 * (ellipseVerticesCount + 1); i< 3 * decalage + 3 * numVerticesPolygon; ++i)
	{
		vertices_buffer[i] = 0.0f;
	}

	float A = isFilled? GetAlpha() : 0.0f;
	float R = fillColour.getRed() / 255.0f;
	float G = fillColour.getGreen() / 255.0f;
	float B = fillColour.getBlue() / 255.0f;
	float *couloursPtr = &coulours_buffer[4 * decalage];
	for (int i = 0; i < numVerticesPolygon; ++i)
	{
		couloursPtr[i * 4] = R;
		couloursPtr[i * 4 + 1] = G;
		couloursPtr[i * 4 + 2] = B;
		couloursPtr[i * 4 + 3] = A;
	}

	decalage += numVerticesPolygon;

	// contour
	const float extA = float(aInPixels + dR);
	const float extB = float(bInPixels + dR);
	const float Xoffset = (float)centerInPixels.get<0>();
	const float Yoffset = (float)centerInPixels.get<1>();
	Zoffset = mainZoffset + 0.1f;
	float *vertexPtr = &vertices_buffer[3 * decalage];
	for (int i = 0; i < ellipseVerticesCount; i++)
	{
		double normalizedAngle = (double)(i) / (double)(ellipseVerticesCount);
		vertexPtr[i * 3] = Xoffset + extA * (float)std::cos(2.0 * M_PI * normalizedAngle);
		vertexPtr[i * 3 + 1] = Yoffset + extB * (float)std::sin(2.0 * M_PI * normalizedAngle);
		vertexPtr[i * 3 + 2] = Zoffset;
	}

	for (int i = 3 * decalage + 3 * ellipseVerticesCount; i< 3 * decalage + (3 * numPointsPolygon); ++i)
	{
		vertexPtr[i] = 0.0f;
	}
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



