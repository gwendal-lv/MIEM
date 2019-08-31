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


#include <MiamMath.h> // plutôt que cmath simple, sinon problème avec M_PI sous VS2015

#include "DrawableEllipse.h"
#include "SceneCanvasComponent.h"

#include "XmlUtils.h"


using namespace Miam;

DrawableEllipse::DrawableEllipse(bptree::ptree & areaTree)
:
DrawableArea(areaTree)
{
    // Data expected if different, depending on the value of "resize when parent size changes",
    // which is mandatory in the tree, and which must have been read by the drawable area XML reader
    if (proportionalToParent)
    {
        try {
            a = areaTree.get<double>("geometry.axes.<xmlattr>.a");
            b = areaTree.get<double>("geometry.axes.<xmlattr>.b");
        }
        catch (bptree::ptree_error &e) {
            throw XmlReadException("DrawableEllipse construction : axes lengths a and/or b cannot be read : ", e);
        }
    }
    else
    {
        try {
            aInPixels = areaTree.get<int>("geometry.axes.<xmlattr>.a_in_pixels");
            bInPixels = areaTree.get<int>("geometry.axes.<xmlattr>.b_in_pixels");
        }
        catch (bptree::ptree_error &e) {
            throw XmlReadException("DrawableEllipse construction : axes lengths [in pixels] a and/or b cannot be read : ", e);
        }
    }
    
    try {
        rotationAngle = areaTree.get<double>("geometry.rotation");
    }
    catch (bptree::ptree_error&) {
        // Node non-obligatoire -> Valeur par défaut
        rotationAngle = 0.0;
    }
    
    // Actualisations graphiques
    init();
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

    // Actualisations graphiques
    init();
}

DrawableEllipse::DrawableEllipse(int64_t _Id, bpt _center, int _radiusInPixels, Colour _fillColour)
    : DrawableArea(_Id, _center, _fillColour),
    aInPixels(_radiusInPixels),
    bInPixels(_radiusInPixels)
{
    // default non sense values... because the radius in pixels alone must be used
    a = 0.1;
    b = 0.1;
    xScale = 1.0;
    yScale = 1.0;
    
    // Fixed size in pixels
    proportionalToParent = false;
    
    // Initial geometry ?
    // All of this code is intended to work on VBO mode only....
    // the Juce classical drawing needs to be implemented for fixed-size items for
    // mixed Juce/CPU/OpenGL rendering
    #ifndef __MIEM_VBO
    assert(false); // see remark above ! cannot construct a fixed-size ellipse in non-VBO mode
    #endif
    
    // Contour points : fake coordinates around the center (code copied from a real ctor)
    boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));
    boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() + (a / 2)*xScale, center.get<1>()));
    boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() + (b / 2)*yScale));
    boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() - (a / 2)*xScale, center.get<1>()));
    boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));
    
    // Graphical updates
    init();
}

void DrawableEllipse::init()
{
    createJucePolygon();
#ifdef __MIEM_VBO
    // resize des buffers
    vertices_buffer.resize(GetVerticesBufferSize());
    indices_buffer.resize(GetIndicesBufferSize());
    coulours_buffer.resize(GetColoursBufferSize());
    
    initSubBuffers();
#endif // def __MIEM_VBO
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
    for (int i = 1; i<numPointsPolygon; i++)
    {
        double normalizedAngle = (double)(i)/(double)(numPointsPolygon);
        contour.lineTo(centerX + aScaled * (float)std::cos(2.0 * M_PI * normalizedAngle),
                       centerY + bScaled * (float)std::sin(2.0 * M_PI * normalizedAngle));
    }
    // Est-ce qu'il faut ABSOLUMENT faire le tour complet avec début=fin, avant de fermer le contour ?
    contour.closeSubPath();

	contour.applyTransform(AffineTransform::scale((float)width, (float)height));

	contour.applyTransform(AffineTransform::rotation((float)rotationAngle, (float)center.get<0>() * (float)width, (float)center.get<1>() * (float)height));
}

void DrawableEllipse::initSubBuffers()
{
    // INDEXES will be the same for a polygon or an ellipse...
    initSurfaceAndContourIndexSubBuffer(DrawableArea::GetVerticesBufferElementsCount(),
                                        DrawableArea::GetIndicesBufferElementsCount(),
                                        numPointsPolygon); // all points used
}


DrawableEllipse::~DrawableEllipse()
{
	
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

    //  If it auto-resizes : we update the a/b in pixels
    if (proportionalToParent)
    {
        // - - - - TO BE TESTED - - - - -
        // - - - - TO BE TESTED - - - - -
        aInPixels = (int)(a * (double)parentCanvas->getWidth() * xScale);
        bInPixels = (int)(b * (double)parentCanvas->getWidth() * xScale); // référence à x... car rotation possible  ?
        // - - - - TO BE TESTED - - - - -
        // - - - - TO BE TESTED - - - - -
    }
    // else, we update the a/b in relative coordinates
    else
    {
        // - - - - !!! DOES NOT TAKE ROTATION INTO ACCOUNT - - - - - ! ! !
        // - - - - !!! DOES NOT TAKE ROTATION INTO ACCOUNT - - - - - ! ! !
        a = (double)(aInPixels) / (double)(parentCanvas->getWidth());
        b = (double)(bInPixels) / (double)(parentCanvas->getHeight());
        // - - - - !!! DOES NOT TAKE ROTATION INTO ACCOUNT - - - - - ! ! !
        // - - - - !!! DOES NOT TAKE ROTATION INTO ACCOUNT - - - - - ! ! !
    }
    
	createJucePolygon(parentCanvas->getWidth(), parentCanvas->getHeight());
        
    // Pixel contour points
    contourPointsInPixels.clear();
    boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(parentCanvas->getWidth(), parentCanvas->getHeight());
    boost::geometry::transform(contourPoints, contourPointsInPixels, scaler);

}

void DrawableEllipse::RefreshOpenGLBuffers()
{
	DrawableArea::RefreshOpenGLBuffers();

	// - - - - - Surface de l'ellipse - - - - -
	const int surfaceVertexBufElmtOffset = DrawableArea::GetVerticesBufferElementsCount();
    float Zoffset = mainZoffset + MIEM_SHAPE_SURFACE_Z;
    // vertex du centre
	const float Cx = (float)centerInPixels.get<0>();
	const float Cy = (float)centerInPixels.get<1>();
	vertices_buffer[3 * surfaceVertexBufElmtOffset + 0] = Cx;
    vertices_buffer[3 * surfaceVertexBufElmtOffset + 1] = Cy;
    vertices_buffer[3 * surfaceVertexBufElmtOffset + 2] = Zoffset;
    // puis tous les points de contour
	for (int i = 0; i < numPointsPolygon; i++)
	{
		const double normalizedAngle = (double)(i) / (double)(numPointsPolygon);
		vertices_buffer[3 * (surfaceVertexBufElmtOffset + 1 + i) + 0] = Cx
                    + (float)aInPixels * (float)std::cos(2.0 * M_PI * normalizedAngle);
		vertices_buffer[3 * (surfaceVertexBufElmtOffset + 1 + i) + 1] = Cy
                    + (float)bInPixels * (float)std::sin(2.0 * M_PI * normalizedAngle);
		vertices_buffer[3 * (surfaceVertexBufElmtOffset + 1 + i) + 2] = Zoffset;
	}
    // pas de remplissage... On utilise toute la place dispo dans le buffer

    // couleurs de la surface (toujours mises à jour)
	const float A = isFilled? GetAlpha() : 0.0f;
	const float R = fillColour.getRed() / 255.0f;
	const float G = fillColour.getGreen() / 255.0f;
	const float B = fillColour.getBlue() / 255.0f;
	for (int i = 0; i < numVerticesPolygon; ++i)
	{
		coulours_buffer[4 * (surfaceVertexBufElmtOffset + i) + 0] = R;
		coulours_buffer[4 * (surfaceVertexBufElmtOffset + i) + 1] = G;
		coulours_buffer[4 * (surfaceVertexBufElmtOffset + i) + 2] = B;
		coulours_buffer[4 * (surfaceVertexBufElmtOffset + i) + 3] = A;
	}

	// - - - - - contour de l'ellipse - - - - -
    const int contourRingVertexElmtOffset = surfaceVertexBufElmtOffset + numVerticesPolygon;
    const int biggerContourRingVertexElmtOffset = contourRingVertexElmtOffset + numPointsPolygon;
    // rayon supplémentaire pour l'épaisseur du contour
    const float dR = contourWidth;
	const float extA = float(aInPixels) + dR;
	const float extB = float(bInPixels) + dR;
	const float Xoffset = (float)centerInPixels.get<0>();
	const float Yoffset = (float)centerInPixels.get<1>();
	Zoffset = mainZoffset + MIEM_SHAPE_CONTOUR_Z;
	for (int i = 0; i < numPointsPolygon; i++)
	{
        const double normalizedAngle = (double)(i) / (double)(numPointsPolygon);
        
        // Contour intérieur : recopié depuis buffer précédent (mais les couleurs seront différentes)
        vertices_buffer[3 * (contourRingVertexElmtOffset + i) + 0]
            = vertices_buffer[3 * (surfaceVertexBufElmtOffset + 1 + i) + 0];
        vertices_buffer[3 * (contourRingVertexElmtOffset + i) + 1]
            = vertices_buffer[3 * (surfaceVertexBufElmtOffset + 1 + i) + 1];
        vertices_buffer[3 * (contourRingVertexElmtOffset + i) + 2]
            = vertices_buffer[3 * (surfaceVertexBufElmtOffset + 1 + i) + 2];
        
        // contour extérieur agrandi
        vertices_buffer[3 * (biggerContourRingVertexElmtOffset + i) + 0] = Xoffset
                                + extA * (float)std::cos(2.0 * M_PI * normalizedAngle);
        vertices_buffer[3 * (biggerContourRingVertexElmtOffset + i) + 1] = Yoffset
                                + extB * (float)std::sin(2.0 * M_PI * normalizedAngle);
        vertices_buffer[3 * (biggerContourRingVertexElmtOffset + i) + 2] = Zoffset;
	}
    
    // Couleur à mettre à jour également, change très souvent avec les excitateurs (animés)
    const float Ac = GetAlpha();
    const float Rc = contourColour.getRed() / 255.0f;
    const float Gc = contourColour.getGreen() / 255.0f;
    const float Bc = contourColour.getBlue() / 255.0f;
    for (int i = 0; i < numPointsPolygonContour; ++i)
    {
        coulours_buffer[4 * (contourRingVertexElmtOffset + i) + 0] = Rc;
        coulours_buffer[4 * (contourRingVertexElmtOffset + i) + 1] = Gc;
        coulours_buffer[4 * (contourRingVertexElmtOffset + i) + 2] = Bc;
        coulours_buffer[4 * (contourRingVertexElmtOffset + i) + 3] = Ac;
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
    
    // Écriture des paramètres (non-négligeables, et utiles seulement)
    if (proportionalToParent)
    {
        axesTree.put("<xmlattr>.a", a);
        axesTree.put("<xmlattr>.b", b);
    }
    else
    {
        axesTree.put("<xmlattr>.a_in_pixels", aInPixels);
        axesTree.put("<xmlattr>.b_in_pixels", bInPixels);
    }
    geomeTree.add_child("axes", axesTree);
    if ( std::abs(rotationAngle) > 0.000001 )
        geomeTree.put("rotation", rotationAngle);

    // Renvoi de l'arbre complété par cette classe dérivée
    inheritedTree->put_child("geometry", geomeTree);
    return inheritedTree;
}



