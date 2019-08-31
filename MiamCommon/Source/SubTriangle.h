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

#ifndef SUBTRIANGLE_H_INCLUDED
#define SUBTRIANGLE_H_INCLUDED

#include "JuceHeader.h"



#include "MiamMath.h"


namespace Miam
{
    
    /// <summary>
    /// Geometric structure to make the precise interaction quantification
    /// easier and faster.
    /// </summary><remarks><para>
    /// The triangle i is shaped by points :
    ///     - G, P(i), P(i+1) for i &lt; N-1
    ///     - G, P(N-1), P(0) for i=N-1
    /// Which will be named G, B, C (see documentation for algorithm details)
    /// ((but points order is not guaranteed))
    ///
    /// </para><para>
    /// T is the interaction point.</para>
    /// </remarks>
    class SubTriangle
    {
        
        // Attributes
        protected :
        //Point<double> G, B, C; // B at beginAngle and C at endAngle
		bpt bG, bB, bC;
        double beginAngle, endAngle;
        
        private :
        bool containsAngleZero;
        
        
        
        // Construction
        public :
        /// <summary>
        /// Main job of the constructor : sort the points and angles to get
        /// a strict increasing order for angles.
        /// </summary>
        /// <param name="_G"></param>
        /// <param name="_B"></param>
        /// <param name="_C"></param>
        //SubTriangle(Point<double> _pointG, Point<double> _pointB, Point<double> _pointC);

		SubTriangle(bpt _pointG, bpt _pointB, bpt _pointC);
        
        
        // Interactions computing
        public :
        
        /// <summary>
        /// Whether the subtriangle contains the given angle or not
        /// </summary>
        /// <param name="angle">Angle in Radians, in [0;2pi[</param>
        /// <returns></returns>
        bool ContainsAngle(double angle);
        
        /// <summary>
        /// Realizes the weight computation for the given interaction point.
        ///
        /// Extern documentation and graphs are required to understand what will happen there.
        /// But it works, so the best would be not to modify this at all.
        /// </summary>
        /// <param name="T">Interaction location</param>
        /// <returns></returns>
        //double ComputeInteractionWeight(Point<double> T);
		double ComputeInteractionWeight(bpt T);

		// Area computing
		void computeSurface();
	private:
		double surface;

	public :
		double getSurface();
        double GetLongestDistanceFromG() const;
        
    };
}



#endif  // SUBTRIANGLE_H_INCLUDED
