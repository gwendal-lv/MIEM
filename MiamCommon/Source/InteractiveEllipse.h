/*
  ==============================================================================

    InteractiveEllipse.h
    Created: 2 Feb 2017 11:52:00am
    Author:  ayup1

  ==============================================================================
*/

#ifndef INTERACTIVEELLIPSE_H_INCLUDED
#define INTERACTIVEELLIPSE_H_INCLUDED

#include "DrawableEllipse.h"
#include "InteractiveArea.h"

#define PI 3.14159265359

namespace Miam
{
	class InteractiveEllipse : public DrawableEllipse, public InteractiveArea
	{
		public :
			InteractiveEllipse(int64_t _Id);
			InteractiveEllipse(int64_t _Id, Point<double> _center, double _a, double _b, Colour _fillColour, float _canvasRatio);

			virtual ~InteractiveEllipse() {}
			virtual IDrawableArea* Clone() const override { return new InteractiveEllipse(*this); }

		private :
			void init();

		public :
			virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;

			// Interactions computing
		public:
			virtual bool HitTest(const Point<double>& hitPoint) override;
			double ComputeInteractionWeight(Point<double> T) override;

		private :
			void computeSurface();
		public :
			double GetSurface() { return surface; }

	
	};
}


#endif  // INTERACTIVEELLIPSE_H_INCLUDED

