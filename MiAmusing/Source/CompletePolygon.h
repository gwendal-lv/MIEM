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
		
		CompletePolygon(int64_t _Id,
			bpt _center, bpolygon& _contourPoints,
			std::vector<int> circles, std::vector<double> percent,
			Colour _fillColour);

		~CompletePolygon();

		virtual IDrawableArea* Clone() const override { return new CompletePolygon(*this); }

		void Copy(std::shared_ptr<CompletePolygon> polygonToCopy);

		std::shared_ptr<AreaEvent> setReadingPosition(double p); // set the position in percent of the cursor on the path

		
		void CanvasResized(SceneCanvasComponent* _parentCanvas);
		void Paint(Graphics& g);

		void lengthToPercent();
		void angleToPercent();
		AreaEventType TryBeginPointMove(const Point<double>& newLocation);
		AreaEventType TryMovePoint(const Point<double>& newLocation);
		AreaEventType EndPointMove();
		void setCursorVisible(bool isVisible, SceneCanvasComponent* _parentCanvas);
		bpolygon getPolygon();
		std::shared_ptr<CompletePolygon> fusion(std::shared_ptr<CompletePolygon> polyToFusion, int Id);
		std::shared_ptr<AreaEvent> intersection(std::shared_ptr<CompletePolygon> hitPolygon, int Id, int N);
		bool getUnion(bpolygon hitPolygon, bpolygon &output);
		double getPercentage(bpt hitPoint);
		bool getAllPercentages(int idx, double &value);
		bool getAllDistanceFromCenter(int idx, int &value);

		bpt getCenter();
		void setCursorSize(double newSize);
	private:

		// attributes linked to the Cursor
		bool showCursor;
		bpt cursorCenter;
		double perimeter;
		std::vector<double> percentages; // percentages corresponding to each points
		std::vector<double> anglesPercentages;
		std::shared_ptr<Miam::EditableEllipse> cursor;
		float initCursorSize;
		float cursorSize;

		// attributes linked to the bull's eye surrounding the area
		bool useBullsEye;
		bool showBullsEye;
		bpt bullsEyeCenter;
		double interval;
		double startRadius;
		static const int Nradius = 7;
		std::vector<EditableEllipse> bullsEye;
		double radius[Nradius];
		bool circlesToShow[Nradius];
		void CreateBullsEye();
		void PaintBullsEye(Graphics& g);
		void CanvasResizedBullsEye(SceneCanvasComponent* _parentCanvas);
		std::vector<int> OnCircles;
		

		double pc; // si ca foire quand on bouge la forme en mm temps que le curseur doit tourner -> garder en memoire le poucentage ou se trouve le curseur et rappeler setreadingposition avec ce pourcentage pour le remettre au nouvel endroit.
	};
}

#endif
