/*
  ==============================================================================

    CartesianLine.h
    Created: 8 Apr 2016 11:48:11am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef CARTESIANLINE_H_INCLUDED
#define CARTESIANLINE_H_INCLUDED


#include "JuceHeader.h"

#include "boost/geometry.hpp"
#include "boost/geometry/geometries/geometries.hpp"
#include "boost/geometry/geometries/polygon.hpp"
#include "DrawableArea.h"

//typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;
typedef boost::geometry::model::polygon<bpt> bpolygon;


namespace Miam {
    
    
    /// \brief 2-D line internally defined by an equation of the form a.x + b.y + c = 0
    class CartesianLine
    {
        
        private :
        double a, b, c; // coefficients de l'équation ax + by + c = 0
        
        
        
        public :
        /// \brief Contruction of the line equation from 2 points belonging to it.
        CartesianLine(bpt point1, bpt point2);
        /// \brief Tells wether a moving point has crossed this line during the given
        /// small displacement, or not.
        bool PointWentThrough(bpt previousLocation, bpt nextLocation);
        
        
    };
    
    
}



#endif  // CARTESIANLINE_H_INCLUDED
