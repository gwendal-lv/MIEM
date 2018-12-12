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
#include "MiamMath.h"

#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace Miam;

DrawablePolygon::DrawablePolygon(bptree::ptree & areaTree)
:
DrawableArea(areaTree)
{
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
    
    // TEST TRACE ERREUR
    //g_vertex_ring.SetIndexToTrack(60); // le 66 n'a jamais l'air écrit...
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
    
    initSurfaceAndContourIndexSubBuffer(DrawableArea::GetVerticesBufferElementsCount(),
                                        DrawableArea::GetIndicesBufferElementsCount(),
                                        (int)contourPoints.outer().size() - 1);
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
    const int baseVertexElmtOffset = DrawableArea::GetVerticesBufferElementsCount();
    const int actualPointsCount = (int)contourPointsInPixels.outer().size() - 1;
    
	float Zoffset = mainZoffset + MIEM_SHAPE_SURFACE_Z;
    // CENTRE AU ZERO du buffer
	vertices_buffer[3 * baseVertexElmtOffset + 0] = (float)centerInPixels.get<0>();
	vertices_buffer[3 * baseVertexElmtOffset + 1] = (float)centerInPixels.get<1>();
	vertices_buffer[3 * baseVertexElmtOffset + 2] = Zoffset;
    // puis les points du contour
	for (int i = 0; i<actualPointsCount; i++)
	{
        // toujours le +3 pour décaler du centre
		vertices_buffer[3 * (baseVertexElmtOffset + 1 + i) + 0] =
                                (float)contourPointsInPixels.outer().at(i).get<0>();
		vertices_buffer[3 * (baseVertexElmtOffset + 1 + i) + 1] =
                                (float)contourPointsInPixels.outer().at(i).get<1>();
		vertices_buffer[3 * (baseVertexElmtOffset + 1 + i) + 2] = Zoffset;
	}
    // puis les points inutiles
	for (int i = actualPointsCount; i < numVerticesPolygon; ++i)
    {
        // toujours le +3 pour décaler du centre
        vertices_buffer[3 * (baseVertexElmtOffset + 1 + i) + 0] = MIEM_UNVISIBLE_COORDINATE;
        vertices_buffer[3 * (baseVertexElmtOffset + 1 + i) + 1] = MIEM_UNVISIBLE_COORDINATE;
        vertices_buffer[3 * (baseVertexElmtOffset + 1 + i) + 2] = 0.0f;
    }

    
	// - - - contour de la forme - - -
    const int contourVertexElmtOffset = baseVertexElmtOffset + numVerticesPolygon;
    
    // on va créer 1 deuxième forme par un calcul maison (voir schémas papiers...)
    // puis on récupère les points. C'est du calcul 100% CPU donc pour le placement !
    // Mais ok évite du vertex shader
	Zoffset = mainZoffset + MIEM_SHAPE_CONTOUR_Z;

    // actual internal points
	for (int i = 0; i < actualPointsCount; ++i)
	{
        // Point de l'intérieur du contour (recopiés à partir des précédents)
        vertices_buffer[3 * (contourVertexElmtOffset + i) + 0] = vertices_buffer[3 * (baseVertexElmtOffset + 1 + i) + 0];
        vertices_buffer[3 * (contourVertexElmtOffset + i) + 1] = vertices_buffer[3 * (baseVertexElmtOffset + 1 + i) + 1];
        vertices_buffer[3 * (contourVertexElmtOffset + i) + 2] = Zoffset;
	}
    // actual exterior points
    refreshExternalContourVerticesSubBuffer(contourVertexElmtOffset + numPointsPolygon, Zoffset);
    // unused points
	for (int ii = 3 * actualPointsCount; ii < 3 * numPointsPolygon; ++ii)
    {
		vertices_buffer[3 * (contourVertexElmtOffset) + ii] = 0.0f; // point intérieur du contour blanc
        vertices_buffer[3 * (contourVertexElmtOffset + numPointsPolygon) + ii] = 0.0f; // point extérieur du contour blanc
    }
    
    // - - - colours are managed by the DrawableArea mother class
    initSurfaceAndContourColourSubBuffer(DrawableArea::GetVerticesBufferElementsCount());
}

void DrawablePolygon::refreshExternalContourVerticesSubBuffer(int externalContourVertexElmtOffset, int posZ)
{
    using namespace boost::numeric;
    
    auto& A = contourPointsInPixels.outer();
    // - - 0. Construction d'un tableau utilitaire pour les indices cycliques - -
    // on travaillera parfois sur les indices de 1 à N(équivalent à 0)
    const size_t N = A.size() - 1; // actual points count
    std::vector<size_t> idx;
    idx.resize(N+2);
    for (size_t i=0; i < idx.size() ; i++)
        idx[i] = i % N;
    
    // - - 1. Calcul des vecteurs directeurs u de chaque segment - -
    std::vector<ublas::vector<double>> u(N);
    for (size_t i=0 ; i < N ; i++)
    {
        u[i].resize(2);
        u[i][0] = A[idx[i+1]].get<0>() - A[i].get<0>(); // x
        u[i][1] = A[idx[i+1]].get<1>() - A[i].get<1>(); // y
        u[i] /= ublas::norm_2(u[i]);
    }
    // - - 2. Calcul des vecteurs normaux n - -
    // (pour les points dans le sens horaire et pas trigo !!)
    std::vector<ublas::vector<double>> n(N);
    for (size_t i=0 ; i < N ; i++)
    {
        n[i].resize(2);
        n[i][0] = u[i][1]; // x
        n[i][1] = - u[i][0]; // y
        // pas besoin de re-normaliser
    }
    // - - 3. Calcul des vecteurs directeurs m des nouveaux points extérieurs - -
    std::vector<ublas::vector<double>> m(N);
    for (size_t i=0 ; i < N ; i++)
    {
        // va travailler ici sur le i+1
        m[idx[i+1]].resize(2);
        m[idx[i+1]] = n[idx[i+1]] + n[i];
        m[idx[i+1]] /= ublas::norm_2(m[idx[i+1]]);
    }
    // - - 4. Calcul des angles alpha - -
    // - - 5. Calcul des distances h selon les vecteurs m (du 3.) - -
    std::vector<double> alpha(N);
    std::vector<double> h(N);
    for (size_t i=0 ; i < N ; i++)
    {
        // calcul du cos alpha via produit scalaire
        double cos_alpha = ublas::inner_prod(-u[i], u[idx[i+1]]);
        // puis du sin demi via formule trigo classique
        double sin_half_alpha = std::sqrt((1 - cos_alpha) / 2.0);
        h[idx[i+1]] = (double) contourWidth / sin_half_alpha;
    }
    
    // - - Final : application dans le VBO - -
    for (int i = 0; i < N; ++i)
    {
        // Nouveau Point : faisant partie de l'extérieur du contour
        vertices_buffer[3 * (externalContourVertexElmtOffset + i) + 0] = A[i].get<0>() + h[i] * m[i][0];
        vertices_buffer[3 * (externalContourVertexElmtOffset + i) + 1] = A[i].get<1>() + h[i] * m[i][1];
        vertices_buffer[3 * (externalContourVertexElmtOffset + numPointsPolygon + i) + 2] = posZ;
    }
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

    

