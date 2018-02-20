/*
  ==============================================================================

    IntersectionPolygon.cpp
    Created: 6 Feb 2018 8:49:07am
    Author:  ayup1

  ==============================================================================
*/
#include <vector>
#include "IntersectionPolygon.h"
using namespace Amusing;

IntersectionPolygon::IntersectionPolygon(int64_t _Id, std::shared_ptr<CompletePolygon> m_parent1, std::shared_ptr<CompletePolygon> m_parent2, Colour _fillColour) : 
	Miam::InteractivePolygon(_Id)
{
	fillColour = _fillColour;
	isFilled = true;
	displayCenter = false;
	isNameVisible = false;
	SetOpacityMode(OpacityMode::DependingOnExcitement);
	SetAlpha(1.0f);
	
	threshold = 0.0;
	parent1 = m_parent1;
	parent2 = m_parent2;
}

IntersectionPolygon::~IntersectionPolygon()
{
}

void IntersectionPolygon::Paint(Graphics & g)
{
	if (inter.size() > 0)
		DrawablePolygon::Paint(g);
}

void IntersectionPolygon::CanvasResized(SceneCanvasComponent * _parentCanvas)
{
	parentCanvas = _parentCanvas;
	// On part des polygones parents pour créer le polygone

	// compute the intersection
	bpolygon poly1, poly2;
	poly1 = parent1->getPolygon();
	poly2 = parent2->getPolygon();
	boost::geometry::correct(poly1);
	boost::geometry::correct(poly2);
	
	inter.clear();
	boost::geometry::intersection(poly1, poly2, inter);
	
	// threshold
	std::vector<int> idToDelete;
	for (int i = 0; i < (int)inter.size(); ++i)
		if (boost::geometry::area(inter[i]) < threshold)
			idToDelete.push_back(i);

	for (int i = (int)idToDelete.size(); i != 0; --i)
		inter.erase(inter.begin() + idToDelete[i]);

	//rescale
	bpolygon tmp;
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(_parentCanvas->getWidth(), _parentCanvas->getHeight());
	for (int i = 0; i < inter.size(); ++i)
	{
		boost::geometry::transform(inter[i], tmp, scaler);
		inter[i].clear();
		inter[i] = tmp;
		tmp.clear();
	}

	// creation of JUCE polygon
	contour.clear();
	for (int j = 0; j < inter.size(); ++j)
	{
		contour.startNewSubPath((float)inter.at(j).outer().at(0).get<0>(), (float)inter.at(j).outer().at(0).get<1>());
		for (size_t i = 1; i<inter.at(j).outer().size(); i++)
			contour.lineTo((float)inter.at(j).outer().at(i).get<0>(), (float)inter.at(j).outer().at(i).get<1>());
		contour.closeSubPath();
	}

}

void IntersectionPolygon::setSurfaceThreshold(double m_threshold)
{
	threshold = m_threshold;
}

bool IntersectionPolygon::isChild(std::shared_ptr<CompletePolygon> m_parent)
{
	if (m_parent == parent1 || m_parent == parent2)
		return true;
	return false;
}

std::shared_ptr<CompletePolygon> Amusing::IntersectionPolygon::getNearestParent(bpt location)
{
	double d1 = boost::geometry::distance(location, parent1->getCenter());
	double d2 = boost::geometry::distance(location, parent2->getCenter());
	if (d1 < d2)
		return parent1;
	else
		return parent2;
}
