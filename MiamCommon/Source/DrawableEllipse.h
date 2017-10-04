/*
  ==============================================================================

    DrawableEllipse.h
    Created: 2 Feb 2017 2:17:07pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef DRAWABLEELLIPSE_H_INCLUDED
#define DRAWABLEELLIPSE_H_INCLUDED

#include "JuceHeader.h"
#include "DrawableArea.h"

#include "boost/geometry.hpp"
#include "boost/geometry/geometries/geometries.hpp"
#include "boost/geometry/geometries/polygon.hpp"

//typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;
typedef boost::geometry::model::polygon<bpt> bpolygon;

namespace Miam
{
	class DrawableEllipse : public Miam::DrawableArea
	{
		protected :
			bpolygon contourPoints;
			Path contour;

		public :
			DrawableEllipse(int64_t _Id);
			DrawableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio = 1.77777777f);
			virtual IDrawableArea* Clone() const override { return new DrawableEllipse(*this); }

		private:
			void createJucePolygon(int width = 160, int height = 90);

	public:
		/// \brief Destructor.
		virtual ~DrawableEllipse();
        
        // - - - - - XML import/export - - - - -
        // RAJOUTÉ PAR GWENDAL POUR QUE ÇA COMPILE, À MODIFIER SANS DOUTE
        virtual std::string GetTypeAsString() const override {return "Ellipse";};



		// Display functions
	public:
		virtual void Paint(Graphics& g) override;
		virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
	
	private:
		void recreateContourPoints(int width, int height);

		protected:
			double a, b; // grand axe et petit axe
			double rotationAngle; // angle d'inclinaison de l'ellipse
			float xScale, yScale;
	};
}



#endif  // DRAWABLEELLIPSE_H_INCLUDED
