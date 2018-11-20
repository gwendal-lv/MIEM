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

	int W = areaTree.get<int>("<xmlattr>.width");
	int H = areaTree.get<int>("<xmlattr>.height");
	float oldCanvasRation = (float)W / (float)H;
	if (oldCanvasRation > 1.0F)
	{
		xScale = 1.0f / oldCanvasRation;
		yScale = 1.0f;
	}
	else
	{
		xScale = 1.0f;
		yScale = 1.0f / oldCanvasRation;
	}


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
            throw XmlReadException("Point " + boost::lexical_cast<std::string>(index) + ": ", e);
        }
    }
    // Ajout des points 1 par 1 dans le bon ordre
    for (auto& point : polygonPoints)
        contourPoints.outer().push_back(point);
    contourPoints.outer().push_back(polygonPoints[0]);// contour closing


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
	for (int i = 0; i < pointsCount; ++i)
	{
		indices_buffer[3 * decalage + i * 3] = decalage + i + 1;
		indices_buffer[3 * decalage + i * 3 + 1] = decalage + 0;
		indices_buffer[3 * decalage + i * 3 + 2] = decalage + i + 2 > decalage + pointsCount ? decalage + 1 : decalage + i + 2;
	}
	for (int i = 3 * pointsCount; i < 3 * numVerticesPolygon; ++i)
		indices_buffer[3 * decalage + i] = 0;

	for (int i = 0; i < pointsCount; ++i)
	{
		coulours_buffer[4 * decalage + i * 4] = fillColour.getRed() / 255.0f;
		coulours_buffer[4 * decalage + i * 4 + 1] = fillColour.getGreen() / 255.0f;
		coulours_buffer[4 * decalage + i * 4 + 2] = fillColour.getBlue() / 255.0f;
		coulours_buffer[4 * decalage + i * 4 + 3] = GetAlpha();
	}
	for (int i = 4 * pointsCount; i < 4 * numVerticesPolygon; ++i)
		coulours_buffer[4 * decalage + i] = 0.0f;

	// indices pour dessiner le contour
	int shapeBegin = DrawableArea::GetVerticesBufferSize();
	decalage += numVerticesPolygon;
	for (int i = 0; i < numPointsPolygon; ++i)
	{
		if (i < pointsCount)
		{
			indices_buffer[3 * decalage + i * 6] = shapeBegin + i + 1;
			indices_buffer[3 * decalage + i * 6 + 1] = decalage + i;
			indices_buffer[3 * decalage + i * 6 + 3] = decalage + i + 1 >= decalage + pointsCount ? decalage : decalage + i + 1;
			indices_buffer[3 * decalage + i * 6 + 2] = decalage + i + 1 >= decalage + pointsCount ? decalage : decalage + i + 1;
			indices_buffer[3 * decalage + i * 6 + 4] = shapeBegin + i + 1;
			indices_buffer[3 * decalage + i * 6 + 5] = shapeBegin + i + 2 >= shapeBegin + pointsCount + 1 ? shapeBegin + 1 : shapeBegin + i + 2;
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
	for (int i = 4 * pointsCount; i < 4 * numPointsPolygon; ++i)
		coulours_buffer[4 * decalage + i] = 1.0f;


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

	//vertex_buffer = new float[verticesCount * 3];// = new float[verticesCount * 3]; // tout sommets + le centre * (x,y,z)
	
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
	for (int i = 0; i < pointsCount; ++i)
	{
		indices_buffer[3 * decalage + i * 3]	= decalage + i + 1;
		indices_buffer[3 * decalage + i * 3 + 1] = decalage + 0;
		indices_buffer[3 * decalage + i * 3 + 2] = decalage + i + 2 > decalage + pointsCount ? decalage + 1 : decalage + i + 2;
	}
	for (int i = 3 * pointsCount; i < 3 * numVerticesPolygon; ++i)
		indices_buffer[3 * decalage + i] = 0;

	for (int i = 0; i < pointsCount; ++i)
	{
		coulours_buffer[4 * decalage + i * 4] = fillColour.getRed()/255.0f;
		coulours_buffer[4 * decalage + i * 4 + 1] = fillColour.getGreen()/255.0f;
		coulours_buffer[4 * decalage + i * 4 + 2] = fillColour.getBlue()/255.0f;
		coulours_buffer[4 * decalage + i * 4 + 3] = GetAlpha();
	}
	for (int i = 4 * pointsCount; i < 4 * numVerticesPolygon; ++i)
		coulours_buffer[4 * decalage + i] = 0.0f;

	// indices pour dessiner le contour
	int shapeBegin = DrawableArea::GetVerticesBufferSize();
	decalage += numVerticesPolygon;
	for (int i = 0; i < numPointsPolygon; ++i)
	{
		if (i < pointsCount)
		{
			indices_buffer[3 * decalage + i * 6] = shapeBegin + i + 1;
			indices_buffer[3 * decalage + i * 6 + 1] = decalage + i;
			indices_buffer[3 * decalage + i * 6 + 3] = decalage + i + 1 >= decalage + pointsCount ? decalage : decalage + i + 1;
			indices_buffer[3 * decalage + i * 6 + 2] = decalage + i + 1 >= decalage + pointsCount ? decalage : decalage + i + 1;
			indices_buffer[3 * decalage + i * 6 + 4] = shapeBegin + i + 1;
			indices_buffer[3 * decalage + i * 6 + 5] = shapeBegin + i + 2 >= shapeBegin + pointsCount + 1 ? shapeBegin + 1 : shapeBegin + i + 2;
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
	for (int i = 4 * pointsCount; i < 4 * numPointsPolygon; ++i)
		coulours_buffer[4 * decalage + i] = 1.0f;

	/*modelMatrix = Matrix3D<float>(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);*/

    // Definition of the Juce polygon
    createJucePolygon();
}


DrawablePolygon::DrawablePolygon(int64_t _Id, bpt _center, bpolygon& _bcontourPoints, Colour _fillColour) :
	DrawableArea(_Id, _center, _fillColour)
{
	contourPoints = _bcontourPoints;

	createJucePolygon();
}

int DrawablePolygon::GetIndicesBufferSize()
{
	return DrawableArea::GetIndicesBufferSize() + (3 * numVerticesPolygon + 3 * 2 * numPointsPolygon);
}

// Construction helpers
void DrawablePolygon::createJucePolygon(int width, int height)
{
    // Création de nouveaux points en coordonnées normalisées
	if (keepRatio)
		rescaleContourPoints(width, height);
    
    // Création du contour Juce, sachant que les coordonnées normalisées ont été modifiées
    // si le ratio était consevé
    contour.clear();
	contour.startNewSubPath((float)contourPoints.outer().at(0).get<0>(), (float)contourPoints.outer().at(0).get<1>());
	for (size_t i = 1; i<contourPoints.outer().size(); i++)
		contour.lineTo((float)contourPoints.outer().at(i).get<0>(), (float)contourPoints.outer().at(i).get<1>());
	contour.closeSubPath();
    
    contour.applyTransform(AffineTransform::scale((float)width, (float)height));
    
    // Puis Création du contour en tant que Polygone BOOST (points (en pixels) séparés)
    contourPointsInPixels.clear();
    boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(width, height);
    boost::geometry::transform(contourPoints, contourPointsInPixels, scaler);
    
}



void DrawablePolygon::rescaleContourPoints(int width, int height)
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

	for (int i = 0; i < (int)newContourPoints.outer().size(); ++i)
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



void DrawablePolygon::setIsFilled(bool shouldBeFilled)
{
	isFilled = shouldBeFilled;
}



// Called by the parent component (which is a canvas)
void DrawablePolygon::Paint(Graphics& g)
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




void DrawablePolygon::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    DrawableArea::CanvasResized(_parentCanvas);
    
    // JUCE contour points in pixels
    createJucePolygon(parentCanvas->getWidth(), parentCanvas->getHeight());
    
    // Internal BOOST contour points in pixels
    contourPointsInPixels.clear();
    boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(parentCanvas->getWidth(), parentCanvas->getHeight());
    boost::geometry::transform(contourPoints, contourPointsInPixels, scaler);
}

void DrawablePolygon::RefreshOpenGLBuffers()
{
	DrawableArea::RefreshOpenGLBuffers();
	// forme
	int decalage = DrawableArea::GetVerticesBufferSize();
	float Zoffset = mainZoffset + 0.0f;
	vertices_buffer[3 * decalage] = (float)centerInPixels.get<0>();
	vertices_buffer[3 * decalage + 1] = (float)centerInPixels.get<1>();
	vertices_buffer[3 * decalage + 2] = 0.0f;

	for (int i = 0; i<(int)contourPointsInPixels.outer().size() - 1; i++)
	{
		vertices_buffer[3 * decalage + 3 + i * 3] = (float)contourPointsInPixels.outer().at(i).get<0>();//radius*cosf(currentAngle);
		vertices_buffer[3 * decalage + 3 + i * 3 + 1] = (float)contourPointsInPixels.outer().at(i).get<1>();
		vertices_buffer[3 * decalage + 3 + i * 3 + 2] = Zoffset;
	}
	for (int i = 3 * decalage + 3 + 3 * ((int)contourPointsInPixels.outer().size() - 1); i < 3 * decalage + 3 * numVerticesPolygon; ++i)
		vertices_buffer[i] = 0.0f;



	float A = isFilled? GetAlpha() : 0.0f;
	const float R = fillColour.getRed() / 255.0f;
	const float G = fillColour.getGreen() / 255.0f;
	const float B = fillColour.getBlue() / 255.0f;
	for (int i = 0; i < numVerticesPolygon; ++i)
	{
		coulours_buffer[4 * decalage + i * 4] = R;
		coulours_buffer[4 * decalage + i * 4 + 1] = G;
		coulours_buffer[4 * decalage + i * 4 + 2] = B;
		coulours_buffer[4 * decalage + i * 4 + 3] = A;
	}
	/*for (int i = 4 * (contourPointsInPixels.outer().size() - 1); i < 4 * numVerticesPolygon; ++i)
	opaque_color_buffer[4 * decalage + i] = 0.0f;*/

	decalage += numVerticesPolygon;

	// contour
	//bpolygon outlinePolygon, tmpPolygon;
	float dist = (float)boost::geometry::distance(centerInPixels, contourPointsInPixels.outer().at(0));
	float newDist = dist + contourWidth;
	float resizeFactor = newDist / dist;


	const float Xoffset = (float)centerInPixels.get<0>();
	const float Yoffset = (float)centerInPixels.get<1>();
	Zoffset = mainZoffset + 0.1f;

	for (int i = 0; i < (int)contourPointsInPixels.outer().size() - 1; ++i)
	{
		vertices_buffer[3 * decalage + i * 3] = Xoffset + resizeFactor * ((float)contourPointsInPixels.outer().at(i).get<0>() - Xoffset);//radius*cosf(currentAngle);
		vertices_buffer[3 * decalage + i * 3 + 1] = Yoffset + resizeFactor * ((float)contourPointsInPixels.outer().at(i).get<1>() - Yoffset);
		vertices_buffer[3 * decalage + i * 3 + 2] = 0.0f;
	}
	for (int i = 3 * decalage + 3 * ((int)contourPointsInPixels.outer().size() - 1); i <3 * decalage + (3 * numPointsPolygon); ++i)
		vertices_buffer[i] = 0.0f;

	/*for (int i = 0; i < numPointsPolygon; ++i)
	{
	opaque_color_buffer[4 * decalage + i * 4] = contourColour.getRed();
	opaque_color_buffer[4 * decalage + i * 4 + 1] = contourColour.getGreen();
	opaque_color_buffer[4 * decalage + i * 4 + 2] = contourColour.getBlue();
	opaque_color_buffer[4 * decalage + i * 4 + 3] = contourColour.getAlpha();
	}*/
	/*for (int i = 4 * (outlinePolygon.outer().size() - 1); i < 4 * numPointsPolygon; ++i)
	opaque_color_buffer[4 * decalage + i] = 0.0f;*/

	//DBG("------ path -------");
	//for (int i = 3*(numVerticesRing + numVerticesPolygon); i < 3*((numVerticesRing + numVerticesPolygon + 2 * ((int)outlinePolygon.outer().size() - 1))); ++i)
	//{
	//	DBG((String)opaque_index_buffer[i] + " : (" + (String)opaque_vertex_buffer[3 * opaque_index_buffer[i]] + " " + (String)opaque_vertex_buffer[3 * opaque_index_buffer[i] + 1] + ")" + " : "
	//		+ (String)opaque_color_buffer[4 * opaque_index_buffer[i]] + ", " + (String)opaque_color_buffer[4 * opaque_index_buffer[i] + 1] + ", " + (String)opaque_color_buffer[4 * opaque_index_buffer[i] + 2] + ", " + (String)opaque_color_buffer[4 * opaque_index_buffer[i] + 3]);
	//}
	/*
	int test;
	for (int i = 0; i < GetIndexCount(); ++i)
	{
		if (opaque_vertex_buffer[3 * opaque_index_buffer[i]] == 0 || opaque_vertex_buffer[3 * opaque_index_buffer[i] + 1] == 0)
			test = 0;
		if (opaque_color_buffer[4 * opaque_index_buffer[i]] == 0 && opaque_color_buffer[4 * opaque_index_buffer[i] + 1] == 0 && opaque_color_buffer[4 * opaque_index_buffer[i] + 2] == 0 && opaque_color_buffer[4 * opaque_index_buffer[i] + 2] == 0)
			test = 0;
	}*/
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

    

