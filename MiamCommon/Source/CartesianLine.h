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
