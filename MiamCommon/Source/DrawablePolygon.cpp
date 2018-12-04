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
    
    // ????
    // utile ?? Inséré avec commit fusion Amusing OpenGL
    /* ============================
	int W = areaTree.get<int>("<xmlattr>.width");
	int H = areaTree.get<int>("<xmlattr>.height");
     */
    
    
    float oldCanvasRation = 9.0f / 16.0f; // TOTALEMENT ARBITRAIREEEEEE
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


#ifdef __MIEM_VBO
    initBuffers();
#endif

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

#ifdef __MIEM_VBO
    initBuffers();
#endif

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
void DrawablePolygon::initBuffers()
{
    // OPTIMISATION NON NEGLIGEABLE POSSIBLE
    // cette fonction ne devrait être appellée à nouveau que si le NOMBRE DE POINTS
    // a CHANGE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // (en tout cas pour les indices)
    // Sinon elle est appelée à chaque refresh GL... gros gaspillage CPU
    
    // resize des buffers
    vertices_buffer.resize(GetVerticesBufferSize());
    indices_buffer.resize(GetIndicesBufferSize());
    coulours_buffer.resize(GetColoursBufferSize());
    
    // indices pour dessiner la surface de la forme 32 triangles
    const int vertexElmtOffset = DrawableArea::GetVerticesBufferElementsCount();
    int indexElmtOffset = DrawableArea::GetIndicesBufferElementsCount();
    const int actualPointsCount = (int) contourPoints.outer().size() - 1; // last contour point is the same as first (closed shape)
    for (int i = 0; i < actualPointsCount ; ++i) // forme réelle : définition des indices des triangles
    {
        // sera le centre par défaut
        indices_buffer[indexElmtOffset + i * 3 + 0] = vertexElmtOffset + 0;
        // 1ier point extérieur (tjs dans les bornes)
        indices_buffer[indexElmtOffset + i * 3 + 1] = vertexElmtOffset + i + 1;
        // 2nd point extérieur, qui peut être le dernier ou le premier du contour
        indices_buffer[indexElmtOffset + i * 3 + 2] = (i + 2) > actualPointsCount ?
                                                    (vertexElmtOffset + 1) : (vertexElmtOffset + i + 2);
    }
    // En tout : on a autant de triangles que de points sur le contour du polygone
    for (int ii = 3 * actualPointsCount; ii < 3 * numPointsPolygon; ++ii) // points du VBO alloués en mémoire mais inutiles
        indices_buffer[indexElmtOffset + ii] = 0;
    
    // indices pour dessiner le contour
    indexElmtOffset += 3 * numPointsPolygon; // on avait "numPointsPolygon" triangles à dessiner au max
    // indice de début du polygon "agrandi" pour dessiner le contour
    const int biggerPolyVertexElmtOffset = vertexElmtOffset + numVerticesPolygon;
    for (int i = 0; i < numPointsPolygon; ++i)
    {
        if (i < actualPointsCount)
        {
            // -> Triangle avec un côté en bordure intérieure
            // 1ier point de contour (tjs dans les bornes) (! point 0 est le centre)
            indices_buffer[indexElmtOffset + i * 6 + 0] = vertexElmtOffset + i + 1;
            // 2nd point de contour, qui peut être le dernier ou le premier du contour
            indices_buffer[indexElmtOffset + i * 6 + 1] = (i + 2) > actualPointsCount ?
                                                    (vertexElmtOffset + 1) : (vertexElmtOffset + i + 2);
            // pt de contour agrandi (tjs dans les bornes)
            indices_buffer[indexElmtOffset + i * 6 + 2] = biggerPolyVertexElmtOffset + i;
            
            // -> Triangle avec un côté en bordure extérieure
            // on repart du 2nd point de contour non-agrandi
            indices_buffer[indexElmtOffset + i * 6 + 3] = indices_buffer[indexElmtOffset + i * 6 + 1];
            // pt contour agrandi tjs dans les bornes
            indices_buffer[indexElmtOffset + i * 6 + 4] = indices_buffer[indexElmtOffset + i * 6 + 2];
            // pt contour agrandi avec possible dépassement (retour au départ)
            indices_buffer[indexElmtOffset + i * 6 + 5] = (i + 1) >= actualPointsCount ?
                                    biggerPolyVertexElmtOffset : biggerPolyVertexElmtOffset + i + 1;
        }
        else
        {
            indices_buffer[indexElmtOffset + i * 6 + 0] = 0;
            indices_buffer[indexElmtOffset + i * 6 + 1] = 0;
            indices_buffer[indexElmtOffset + i * 6 + 2] = 0;
            indices_buffer[indexElmtOffset + i * 6 + 3] = 0;
            indices_buffer[indexElmtOffset + i * 6 + 4] = 0;
            indices_buffer[indexElmtOffset + i * 6 + 5] = 0;
        }
    }
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




// Called by the parent component (which is a canvas)
void DrawablePolygon::Paint(Graphics& g)
{
	if (isFilled)
	{
		g.setColour(fillColour);
		g.setOpacity(GetAlpha());
		g.fillPath(contour);
	}
    
    //std::cout << "isFilled = " << isFilled << std::endl;
    
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
    
    initBuffers();
    
	// - - - surface de la forme - - -
	int vertexElmtOffset = DrawableArea::GetVerticesBufferElementsCount();
	float Zoffset = mainZoffset + MIEM_SHAPE_SURFACE_Z;
    // CENTRE AU ZERO du buffer
	vertices_buffer[3 * vertexElmtOffset + 0] = (float)centerInPixels.get<0>();
	vertices_buffer[3 * vertexElmtOffset + 1] = (float)centerInPixels.get<1>();
	vertices_buffer[3 * vertexElmtOffset + 2] = Zoffset;
    // puis les points du contour
	for (int i = 0; i<(int)contourPointsInPixels.outer().size() - 1; i++)
	{
        // toujours le +3 pour décaler du centre
		vertices_buffer[3 * vertexElmtOffset + (3) + i * 3 + 0] = (float)contourPointsInPixels.outer().at(i).get<0>();
		vertices_buffer[3 * vertexElmtOffset + (3) + i * 3 + 1] = (float)contourPointsInPixels.outer().at(i).get<1>();
		vertices_buffer[3 * vertexElmtOffset + (3) + i * 3 + 2] = Zoffset;
	}
    // puis les points inutiles
	for (int i = (int)contourPointsInPixels.outer().size(); i < numVerticesPolygon; ++i)
    {
        // toujours le +3 pour décaler du centre
        vertices_buffer[3 * vertexElmtOffset + (3) + i * 3 + 0] = MIEM_UNVISIBLE_COORDINATE;
        vertices_buffer[3 * vertexElmtOffset + (3) + i * 3 + 1] = MIEM_UNVISIBLE_COORDINATE;
        vertices_buffer[3 * vertexElmtOffset + (3) + i * 3 + 2] = 0.0f;
    }

    // - - - couleur de la forme - - -
	const float A = GetAlpha();
	const float R = fillColour.getRed() / 255.0f;
	const float G = fillColour.getGreen() / 255.0f;
	const float B = fillColour.getBlue() / 255.0f;
	for (int i = 0; i < numVerticesPolygon; ++i)
	{
		coulours_buffer[4 * vertexElmtOffset + i * 4 + 0] = R;
		coulours_buffer[4 * vertexElmtOffset + i * 4 + 1] = G;
		coulours_buffer[4 * vertexElmtOffset + i * 4 + 2] = B;
		coulours_buffer[4 * vertexElmtOffset + i * 4 + 3] = A;
	}


    
	// - - - contour de la forme - - -
    vertexElmtOffset += numVerticesPolygon;
    
    // ******************* à ré-écrire proprement **********************
    // ne va pas faire un beau contour, tel quel.... il faut réfléchir un peu à la géométrie
    
    // on va créer 1 deuxième forme en resizant le contour par Juce
    // puis on récupère les points. C'est du calcul 100% CPU donc pour le placement...
    // Mais ok évite du vertex shader
	float dist = (float)boost::geometry::distance(centerInPixels, contourPointsInPixels.outer().at(0));
	float newDist = dist + contourWidth;
	float resizeFactor = newDist / dist;

	const float Xoffset = (float)centerInPixels.get<0>();
	const float Yoffset = (float)centerInPixels.get<1>();
	Zoffset = mainZoffset + MIEM_SHAPE_CONTOUR_Z;

	for (int i = 0; i < (int)contourPointsInPixels.outer().size() - 1; ++i)
	{
		vertices_buffer[3 * vertexElmtOffset + i * 3 + 0] = Xoffset
            + resizeFactor * ((float)contourPointsInPixels.outer().at(i).get<0>() - Xoffset);
		vertices_buffer[3 * vertexElmtOffset + i * 3 + 1] = Yoffset
            + resizeFactor * ((float)contourPointsInPixels.outer().at(i).get<1>() - Yoffset);
        
		vertices_buffer[3 * vertexElmtOffset + i * 3 + 2] = Zoffset;
	}
	for (int ii = 3 * ((int)contourPointsInPixels.outer().size() - 1); ii < 3 * numPointsPolygon; ++ii)
		vertices_buffer[(3 * vertexElmtOffset) + ii] = 0.0f;
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

    

