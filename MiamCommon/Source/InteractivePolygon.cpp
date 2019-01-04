/*
  ==============================================================================

    InteractivePolygon.cpp
    Created: 13 Apr 2016 3:56:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InteractivePolygon.h"
#include "SceneCanvasComponent.h"

#include "MiamMath.h"

#include "InteractionParameters.h"

using namespace Miam;


// ===== CONSTRUCTORS & DESTRUCTORS =====

InteractivePolygon::InteractivePolygon(bptree::ptree & areaTree)
:
DrawablePolygon(areaTree)
{
    init();
}

InteractivePolygon::InteractivePolygon(int64_t _Id) :
DrawablePolygon(_Id)
{
    init();
}

InteractivePolygon::InteractivePolygon(int64_t _Id, bpt _center, int pointsCount, float radius, Colour _fillColour, float _canvasRatio) :
DrawablePolygon(_Id, _center, pointsCount, radius, _fillColour, _canvasRatio)
{
    init();
}

InteractivePolygon::InteractivePolygon(int64_t _Id, bpt _center, bpolygon& _contourPoints, Colour _fillColour) :
DrawablePolygon(_Id, _center, _contourPoints, _fillColour)
{
    init();
}


std::shared_ptr<IDrawableArea> InteractivePolygon::Clone()
{
    auto clone = std::make_shared<InteractivePolygon>(*this);
    clone->onCloned();
    return clone;
}

void InteractivePolygon::onCloned()
{
    // On n'appelle que le "onCloned" de la forme générique.
    InteractiveArea::onCloned();
    // Pas le "onCloned" du parent graphique Drawable (parent concret)
}


void InteractivePolygon::init()
{
    // updateSubTriangles(); NO : can't build them ! No contour Points yet
}




void InteractivePolygon::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    DrawablePolygon::CanvasResized(_parentCanvas);
    InteractiveArea::CanvasResized(_parentCanvas);
    
    // Finally, we update internal geometric data
    updateSubTriangles();
    updateEdgesHitBoxes();
}



// ===== Updates =====
void InteractivePolygon::updateSubTriangles()
{
    // Reinitializes the whole list
    subTriangles.clear();
	
	// We begin by the annoying one
    // ATTENTION les contour points forment un polygone boost FERMÉ
    // c-à-d que le premier et le dernier point sont les mêmes
	subTriangles.push_back(SubTriangle(centerInPixels,
                                       contourPointsInPixels.outer().at(contourPointsInPixels.outer().size()-2), // avant-dernier, car premier et dernier sont identiques
                                   contourPointsInPixels.outer().front()));
	// Then add the others
	for (size_t i = 0; i <contourPointsInPixels.outer().size() - 2; i++)
	{
		subTriangles.push_back(SubTriangle(centerInPixels, contourPointsInPixels.outer().at(i), contourPointsInPixels.outer().at(i+1)));
	}
    
    // Actualisations après création des triangles
    computeSurface();
    
    // Génération des splines pour le calcul des poids d'interaction
}

void InteractivePolygon::computeSurface()
{
	surface = 0;
	for (size_t i = 0; i < subTriangles.size(); ++i)
		surface += subTriangles[i].getSurface();
}


// ===== INTERACTION COMPUTING =====


bool InteractivePolygon::HitTest(bpt T) const
{
    return (contour.contains((float)T.get<0>(),
                             (float)T.get<1>()));
}



double InteractivePolygon::ComputeInteractionWeight(bpt T)
{
    double distanceFromCenter = boost::geometry::distance(centerInPixels,T);
    
    double weight = 0.0;
    bpt GT(T.get<0>() - centerInPixels.get<0>(), T.get<1>() - centerInPixels.get<1>());
    
    // if at center (to prevent 0/0 operations)
    if (distanceFromCenter < 0.5)
        weight = 1.0;
    // else, we can compute an angle using atan and the 4 quadrants
    else
    {
        // Angle à partir de l'axe x (horizontal vers la droite)
        // Dans le sens trigo avec l'axe y qui va vers le bas
        // (et donc dans le horaire avec les conventions math habituelles)
        double angle = Math::ComputePositiveAngle(GT);
        
        // poids sans distorsion via sous-triangle
        weight = findSubTriangle(angle).ComputeInteractionWeight(T);
        
        
    }
    
    return weight;
}

SubTriangle& InteractivePolygon::findSubTriangle(double angle)
{
    int i = 0;
    while (!subTriangles[i].ContainsAngle(angle))
        i++;
    
    return subTriangles[i];
}

void InteractivePolygon::updateEdgesHitBoxes()
{
    // !! optimisation possible !!
    // on pourrait ne rajouter/supprimer que le nombre qu'il faut ?
    // car les hit boxes n'ont toujours que 4 côtés
    edgesHitBoxes.clear();
    
    for (size_t i=0 ; i<contourPointsInPixels.outer().size()-1 ; i++)
    {
        // N.B. : le point [i+1] existe toujours (polygone fermé)
        
        // calcul de l'orientation de l'arête i -> i+1
        double orientation = std::atan2( contourPointsInPixels.outer()[i+1].get<1>()
                                         - contourPointsInPixels.outer()[i].get<1>(),
                                         contourPointsInPixels.outer()[i+1].get<0>()
                                         - contourPointsInPixels.outer()[i].get<0>());
        auto alpha = orientation + M_PI_2; // alpha = perpendiculaire de l'orientation de l'arête
        // création et placement du polygone et des 4 points
        bpolygon hitBoxRectangle;
        hitBoxRectangle.outer().push_back(bpt(contourPointsInPixels.outer()[i].get<0>()
                                              - std::cos(alpha)*elementInteractionRadius,
                                              contourPointsInPixels.outer()[i].get<1>()
                                              - std::sin(alpha)*elementInteractionRadius));
        hitBoxRectangle.outer().push_back(bpt(contourPointsInPixels.outer()[i].get<0>()
                                              + std::cos(alpha)*elementInteractionRadius,
                                              contourPointsInPixels.outer()[i].get<1>()
                                              + std::sin(alpha)*elementInteractionRadius));
        hitBoxRectangle.outer().push_back(bpt(contourPointsInPixels.outer()[i+1].get<0>()
                                              + std::cos(alpha)*elementInteractionRadius,
                                              contourPointsInPixels.outer()[i+1].get<1>()
                                              + std::sin(alpha)*elementInteractionRadius));
        hitBoxRectangle.outer().push_back(bpt(contourPointsInPixels.outer()[i+1].get<0>()
                                              - std::cos(alpha)*elementInteractionRadius,
                                              contourPointsInPixels.outer()[i+1].get<1>()
                                              - std::sin(alpha)*elementInteractionRadius));
        // fermeture pour test d'appartenance ('within') au contenu d'un contour fermé
        hitBoxRectangle.outer().push_back(hitBoxRectangle.outer()[0]);
        edgesHitBoxes.push_back(hitBoxRectangle);
    }
}


double InteractivePolygon::GetSurface()
{
	return surface;
}
