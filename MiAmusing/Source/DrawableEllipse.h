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

namespace Amusing
{
	class DrawableEllipse : public Miam::DrawableArea
	{
		protected :
			std::vector<Point<double>> contourPoints;
			Path contour;

		public :
			DrawableEllipse(int64_t _Id);
			DrawableEllipse(int64_t _Id, Point<double> _center, double _a, double _b, Colour _fillColour, float _canvasRatio = 1.77777777f);
			virtual IDrawableArea* Clone() const override { return new DrawableEllipse(*this); }

		private:
			void createJucePolygon(int width = 160, int height = 90);

	public:
		/// \brief Destructor.
		virtual ~DrawableEllipse();




		// Display functions
	public:
		virtual void Paint(Graphics& g) override;
		virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;

		protected:
			double a, b; // grand axe et petit axe
			float xScale, yScale;
	};
}



#endif  // DRAWABLEELLIPSE_H_INCLUDED
