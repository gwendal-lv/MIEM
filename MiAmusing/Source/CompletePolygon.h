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

		CompletePolygon(bptree::ptree& areaTree);
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

		virtual std::shared_ptr<IDrawableArea> Clone() override
		{
			auto clone = std::make_shared<CompletePolygon>(*this);
			clone->onCloned();
			return clone;
		}

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

		void setCursorsBaseNote(int idx, double newBaseNote);
		void setCursorsSpeed(int idx, double newSize);
		bpt computeLinearCursorCenter(double p);
		bpt computeAngularCursorCenter(double p);
		float computeCursorAlpha(double p, bpt _center,float a);
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

		virtual std::shared_ptr<bptree::ptree> GetTree() override;

		void showAllTarget(bool shouldBeShowed);

		void Translate(const Point<double>& translation);

		void DisableTranslation(bool shouldBeDisabled);

		double GetFullSceneRatio();

		bool SizeChanged(double _size, bool minSize);

		virtual void RefreshOpenGLBuffers() override;
		void RefreshTargetOpenGLBuffers();

		const int GetVerticesBufferSize() override {					// points du contour					manipulationLine	manipulationPoint
			return EditablePolygon::GetVerticesBufferSize() + Nradius * bullsEye[0].GetVerticesBufferSize();
		}
		int GetCouloursBufferSize() override { return EditablePolygon::GetCouloursBufferSize() + Nradius * bullsEye[0].GetCouloursBufferSize(); }
		int GetIndicesBufferSize() override { return EditablePolygon::GetIndicesBufferSize() + Nradius * bullsEye[0].GetIndicesBufferSize(); }
//        void setVisible(bool shoulBeVisible) override{ DBG("no more visible !!"); areaIsVisible = shoulBeVisible; }
//        bool isVisible() override { return areaIsVisible; }
		
	private:
		int numAngles;
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
		double currentTouchSize;
		
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
		bool showAllCircles;
		bool deleteOldCircles;
		
		// flags and invisible points for chords
		std::vector<bool> chordFlag;
		std::vector<std::shared_ptr<CompletePolygon>> chordAreaForFlag;
		std::vector<std::shared_ptr<CompletePolygon>>  chordAreaForPercentage;
		std::vector<double> chordsAnglePercentage; // no need of chordsOnCircle, the audio manager will compute the chords

		int previousSizeToShow; // nombre de cercle qu'il fallait dessiner précédemment

		double pc; // si ca foire quand on bouge la forme en mm temps que le curseur doit tourner -> garder en memoire le poucentage ou se trouve le curseur et rappeler setreadingposition avec ce pourcentage pour le remettre au nouvel endroit.

		bool onlyRotationAllowed;

		bool areaIsVisible;
		
	};
}

#endif
