/*
  ==============================================================================

    CompletePolygon.h
    Created: 5 May 2017 11:39:08am
    Author:  ayup1

  ==============================================================================
*/

#ifndef COMPLETEPOLYGON_H_INCLUDED
#define COMPLETEPOLYGON_H_INCLUDED

#include "EditablePolygon.h"
#include "EditableEllipse.h"
#include "JuceHeader.h"
#include "SceneCanvasComponent.h"
//namespace Miam
//{
//	class SceneCanvasComponent;
//}

namespace Amusing
{
	class Follower;

	class CompletePolygon : public Miam::EditablePolygon
		//public AnimatedAppComponent
	{
	public:
		CompletePolygon(int64_t _Id);
		CompletePolygon(int64_t _Id,
			bpt _center, int pointsCount, float radius,
			Colour _fillColour, float _canvasRatio = 1.47);
		CompletePolygon(int64_t _Id,
			bpt _center, bpolygon& _contourPoints,
			Colour _fillColour);

		~CompletePolygon();

		virtual IDrawableArea* Clone() const override { return new CompletePolygon(*this); }

		void setReadingPosition(double p); // set the position in percent of the cursor on the path

		
		void CanvasResized(SceneCanvasComponent* _parentCanvas);
		void Paint(Graphics& g);

		void lengthToPercent();
		AreaEventType TryMovePoint(const Point<double>& newLocation);
		void setCursorVisible(bool isVisible);
		bpolygon getPolygon();
		std::vector<bpt> intersection(bpolygon hitPolygon);
		double getPercentage(bpt hitPoint);
	private:

		// attributes linked to the Cursor
		bool showCursor;
		bpt cursorCenter;
		double perimeter;
		std::vector<double> percentages; // percentages corresponding to each points
		std::shared_ptr<Miam::EditableEllipse> cursor;

		double pc; // si ca foire quand on bouge la forme en mm temps que le curseur doit tourner -> garder en memoire le poucentage ou se trouve le curseur et rappeler setreadingposition avec ce pourcentage pour le remettre au nouvel endroit.
	};
}

#endif
