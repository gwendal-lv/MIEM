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
    // - - - Reinitializes the whole lists of triangles and segments - - -
    subTriangles.clear();
    segments.clear();
	
	// We begin by the annoying one
    // ATTENTION les contour points forment un polygone boost FERMÉ
    // c-à-d que le premier et le dernier point sont les mêmes
    // last = avant-dernier, car premier et dernier sont identiques
    auto lastPt = contourPointsInPixels.outer().at(contourPointsInPixels.outer().size()-2);
	subTriangles.push_back(SubTriangle(centerInPixels, lastPt, contourPointsInPixels.outer().front()));
    segments.push_back(Segment(lastPt, contourPointsInPixels.outer().front()));
	// Then add the others
	for (size_t i = 0; i < contourPointsInPixels.outer().size() - 2; i++)
	{
		subTriangles.push_back(SubTriangle(centerInPixels, contourPointsInPixels.outer().at(i), contourPointsInPixels.outer().at(i+1)));
        segments.push_back(Segment(contourPointsInPixels.outer().at(i), contourPointsInPixels.outer().at(i+1)));
	}
    
    // Actualisations après création des triangles
    computeSurface();
    rawCenterWeight = computeRawSmoothInteractionWeight(centerInPixels);
    
    // Génération des splines pour le calcul des poids d'interaction
    // -> abandonnés (voir b-curve "maison" via segments)
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
    // Fonction optimisée par Juce
    return (contour.contains((float)T.get<0>(),
                             (float)T.get<1>()));
}



double InteractivePolygon::ComputeInteractionWeight(bpt T)
{
    double distanceFromCenter = boost::geometry::distance(centerInPixels,T);
    double weight = 0.0;
    
    // if at center (to prevent 0/0 operations)
    if (distanceFromCenter < 0.5)
        weight = 1.0;
    // else, we choose the old or the new (>= v1.2.0) continuous weight computation
    else
    {
        // TODO Allow selection of old or new method, with a dirty global variable...
        // (functionnality reserved for experiments, not for release versions)
        if (/* DISABLES CODE */ (false))
        {
            bpt GT(T.get<0>() - centerInPixels.get<0>(), T.get<1>() - centerInPixels.get<1>());
            // Angle à partir de l'axe x (horizontal vers la droite)
            // Dans le sens trigo avec l'axe y qui va vers le bas
            // (et donc dans le horaire avec les conventions math habituelles)
            double angle = Math::ComputePositiveAngle(GT);
            
            // poids sans distorsion via sous-triangle
            weight = findSubTriangle(angle).ComputeInteractionWeight(T);
        }
        else
        {
            weight = computeRawSmoothInteractionWeight(T) / rawCenterWeight;
            weight = Math::SplineDistortionC3(weight);
            // DOES NOT WORK CORRECTLY:
            std::cout << "interaction weight = " << weight << std::endl;
        }
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

double InteractivePolygon::computeRawSmoothInteractionWeight(bpt T)
{
    // from the python code (git: MIEM_Surfaces)
    bpt vectorFromCenter = Segment::SubtractPoints(T, centerInPixels);
    double distanceFromCenter = std::sqrt(Segment::DotProduct(vectorFromCenter, vectorFromCenter));
    std::cout << "dist from center = " << distanceFromCenter << std::endl;
    double center_weight = 1.0;
    if (distanceFromCenter < 1.0)
        center_weight = 1.0;
    else
        center_weight = 1.0 / distanceFromCenter;
    // center has 0.5 of all edges combined weight
    center_weight *= (double)(segments.size()) * 0.5;

    double segments_weight = 0.0;
    for (size_t i=0 ; i<segments.size() ; i++)
    {
        // DOES NOT WORK *********************************************************************
        double distanceFromSegment = segments[i].GetDistanceC1(T);
        std::cout << "dist from segment #" << i << " = " << distanceFromSegment << std::endl;
        if (distanceFromSegment < 1.0) // if we are very close to any segment: weight is just 0.0
            return 0.0;
        else
            segments_weight += (1.0 / distanceFromSegment);
    }
    return center_weight / (center_weight + segments_weight);
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
