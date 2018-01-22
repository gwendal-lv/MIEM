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
#include "Cursors.h"
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

		void SetActive(bool activate);

		void SetAlpha(float newAlpha) override;

		bool shouldShowOptions();

		
		void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
		AreaEventType TryBeginMultiTouchAction(const Point<double>& newLocation);
		AreaEventType TryMoveMultiTouchPoint(const Point<double>& newLocation);
		AreaEventType EndMultiTouchPointMove();
		void Paint(Graphics& g) override;

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
		double getAngularPercentage(bpt hitPoint);
		double getLinearPercentage(bpt hitPoint);
		bool getAllPercentages(int idx, double &value);
		bool getAllDistanceFromCenter(int idx, int &value);

		// return center Position in Pixels;
		bpt getCenter();

		// return normalized center position
		bpt getCenterNormalized();

		// return normalized radius
		float getNormalizedRadius();

		void setCursorsSpeed(int idx, double newSize);
		bpt computeLinearCursorCenter(double p);
		bpt computeAngularCursorCenter(double p);
		float computeCursorAlpha(double p, bpt _center);
		boost::geometry::model::segment<bpt> getSegment(bpt hitPoint);
		boost::geometry::model::segment<bpt> getSegmentInPixels(bpt hitPointInPixels);

		/*void AddCursor();
		void AddCursor(float _canvasRatio);
		void AddCursor(float _canvasRatio, double p);*/
		void linkTo(std::shared_ptr<Cursor> cursor);
		int getCursorsCount();
		std::shared_ptr<Cursor> getCursor(int idx);
		bool contains(bpt point);
		void deleteAllCursors();
		//void getCursorsCount();
		void addChordPoints(double chordsAnglePercentage, std::shared_ptr<CompletePolygon> areaForChord);
		void setChordFlag(bpt chordPt, bool isTrue, std::shared_ptr<CompletePolygon> areaForChord);
		void resetChords();
		bool getChordParameters(int idx, std::shared_ptr<CompletePolygon> &chordArea, double &pC);
		
	private:
		JUCE_LEAK_DETECTOR(CompletePolygon)
		// attributes linked to the Cursor
		bool showCursor;
		
		double perimeter;
		std::vector<double> percentages; // percentages corresponding to each points
		std::vector<double> anglesPercentages;
		//std::shared_ptr<Cursor> cursor;//std::shared_ptr<Miam::EditableEllipse> cursor;
		std::vector<std::shared_ptr<Cursor>> cursors;
		double orientationAngle; // final orientation of the area after rotation
		bool multiTouchActionBegun;
		double currentTouchRotation; // to keep track of the rotation during multitouch action
		
		float initCursorSize;
		float cursorSize;

		// attributes linked to the bull's eye surrounding the area
		bool useBullsEye;
		bool showBullsEye;
		bpt bullsEyeCenter;
		double interval;
		double startRadius;
		static const int Nradius = 5;
		std::vector<EditableEllipse> bullsEye;
		double radius[Nradius];
		bool circlesToShow[Nradius];
		void CreateBullsEye();
		void PaintBullsEye(Graphics& g);
		void CanvasResizedBullsEye(SceneCanvasComponent* _parentCanvas);
		std::vector<int> OnCircles;
		
		// flags and invisible points for chords
		std::vector<bool> chordFlag;
		std::vector<std::shared_ptr<CompletePolygon>> chordAreaForFlag;
		std::vector<std::shared_ptr<CompletePolygon>>  chordAreaForPercentage;
		std::vector<double> chordsAnglePercentage; // no need of chordsOnCircle, the audio manager will compute the chords


		double pc; // si ca foire quand on bouge la forme en mm temps que le curseur doit tourner -> garder en memoire le poucentage ou se trouve le curseur et rappeler setreadingposition avec ce pourcentage pour le remettre au nouvel endroit.
	};
}

#endif
