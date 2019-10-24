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
/*
  ==============================================================================

    MiemSegment.h
    Created: 23 Oct 2019 9:30:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#pragma once

#include <cmath>

namespace Miam
{
    /// \brief A minimal class for representing a segment,
    /// optimized for computing the MIEM smooth geometric weights
    class Segment
    {
        // = = = = = ATTRIBUTES = = = = =
        bpt beginPt;
        bpt endPt;
        bpt directionVector; ///< Normalized. Stored as a point for easiness of use.
        double length;
        
        
        // = = = = = METHODS = = = = =
        public :
        Segment(bpt _beginPt, bpt _endPt)
        {
            beginPt = _beginPt;
            endPt = _endPt;
            
            // will be normalized after length computation
            directionVector = SubtractPoints(endPt, beginPt);
            length = std::sqrt(DotProduct(directionVector, directionVector));
            directionVector = MultiplyPoint(directionVector, 1.0 / length);
        }
        /// \brief Computes the distance between the given point pt and this segment, using a C1 method.
        inline double GetDistanceC1(bpt pt)
        {
            // from Original Python code
            
            double dot_product_along_segment = DotProduct(directionVector, SubtractPoints(pt, beginPt));
            // pt projection is inside this segment -> length of projection
            if ( (0.0 < dot_product_along_segment)
                && (dot_product_along_segment < length) )
            {
                bpt ptOnSegment(beginPt.get<0>() + dot_product_along_segment * directionVector.get<0>(),
                                beginPt.get<1>() + dot_product_along_segment * directionVector.get<1>());
                auto vector_from_projection = SubtractPoints(pt, ptOnSegment);
                return std::sqrt(DotProduct(vector_from_projection, vector_from_projection));
            }
            // pt projection is outside the segment -> distance from the proper segment end
            else
            {
                bpt vector_from_segment_pt;
                if (dot_product_along_segment <= 0.0) // pt projection is located before the beginning of this segment
                    vector_from_segment_pt = SubtractPoints(pt, beginPt);
                else // pt projection is located after the end of this segment
                    vector_from_segment_pt = SubtractPoints(pt, endPt);
                // because of this, computation is C1 only (curvature radius is not continuous)
                return std::sqrt(DotProduct(vector_from_segment_pt, vector_from_segment_pt));
            }
        }
        /// \brief Returns a 2D point = pt1 - pt2
        ///
        /// (boost subtract_point modifies the inputs)
        inline static bpt SubtractPoints(bpt pt1, bpt pt2)
        {
            return bpt(pt1.get<0>() - pt2.get<0>(),
                       pt1.get<1>() - pt2.get<1>());
        }
        /// \brief Computes a dot product on 2D boost points (considered as vectors)
        inline static double DotProduct(bpt pt1, bpt pt2)
        {
            return pt1.get<0>() * pt2.get<0>() + pt1.get<1>() * pt2.get<1>();
        }
        /// \brief Multiply point...... has meaning if the bpt is a vector
        inline static bpt MultiplyPoint(bpt pt, double factor)
        {
            return bpt(pt.get<0>() * factor, pt.get<1>() * factor);
        }
    };
}
