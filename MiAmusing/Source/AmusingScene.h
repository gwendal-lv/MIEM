/*
  ==============================================================================

    AmusingScene.h
    Created: 8 Dec 2016 2:56:07pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef AMUSINGSCENE_H_INCLUDED
#define AMUSINGSCENE_H_INCLUDED

#include "EditableScene.h"
#include "AnimatedPolygon.h"
#include "Follower.h"
#include "Cursors.h"

using namespace Miam;

namespace Amusing
{
	class AnimatedPolygon;
	class CompletePolygon;
	class Follower;
}

namespace Miam
{

	class MultiSceneCanvasInteractor;

	class AmusingScene : public EditableScene/*,
											 public std::enable_shared_from_this<AmusingScene> */
	{
	public:
		AmusingScene(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager, SceneCanvasComponent* _canvasComponent);
		virtual ~AmusingScene();

		void AddAnimatedArea(uint64_t nextAreaId);
		std::shared_ptr<AreaEvent> AddNedgeArea(uint64_t nextAreaId, int N);
		// override mouse callback

		std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE) override;
		std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE) override;
		std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE) override;
		std::shared_ptr<AreaEvent> AddTrueCircle(uint64_t nextAreaId);
		std::shared_ptr<AreaEvent> AddFollower(uint64_t nextAreaId);
		std::shared_ptr<AreaEvent> AddCompleteArea(uint64_t);

		std::vector< std::shared_ptr<Amusing::Follower> > followers;
		int Nfollower;
		std::shared_ptr<Amusing::AnimatedPolygon> getFirstArea();
		std::shared_ptr<Amusing::AnimatedPolygon> getNextArea();
		std::shared_ptr<Amusing::CompletePolygon> getFirstCompleteArea();
		std::shared_ptr<Miam::MultiAreaEvent> SetAllAudioPositions(double position);

		//std::shared_ptr<Amusing::AnimatedPolygon> hitPolygon(Point<double> hitPoint, std::shared_ptr<Amusing::AnimatedPolygon> polygon);
		Point<double> hitPolygon(Point<double> hitPoint, std::shared_ptr<Amusing::AnimatedPolygon> polygon, Point<double> P, Point<double> Q);
		/*
		virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE) override;
		virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE) override;
		virtual std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE) override;
		*/
		// private attribut

		//Model *model;

		double testDephasage;

		int getNumberArea();
		std::shared_ptr<AreaEvent> OnDelete();
		bool deleting;
		std::shared_ptr<AreaEvent> deleteEvent;
		std::shared_ptr<Amusing::Follower> getFollowers(std::shared_ptr<Amusing::AnimatedPolygon> masterArea);
		std::shared_ptr<AreaEvent> resendArea(int idx);

		std::vector< std::shared_ptr<Amusing::CompletePolygon> > currentIntersectionsAreas;
		std::shared_ptr<AreaEvent> AddIntersectionArea(std::shared_ptr<Amusing::CompletePolygon> parent1, std::shared_ptr<Amusing::CompletePolygon> parent2, std::shared_ptr<Amusing::CompletePolygon> newIntersection);

		// map with 2 entry = the parents polygons, and 1 output = the vector of overlap area between the two parent polygon
		std::map<std::pair<std::shared_ptr<Amusing::CompletePolygon>, std::shared_ptr<Amusing::CompletePolygon>>, std::vector<std::shared_ptr<Amusing::CompletePolygon>>> parentTochildArea;
		bool getParents(std::shared_ptr<IEditableArea> child, std::shared_ptr<IEditableArea> &parent1, std::shared_ptr<IEditableArea> &parent2);
		bool lookForAreasInteractions(std::shared_ptr<Amusing::CompletePolygon> currentPolygon);
		void AddAllIntersections(std::shared_ptr<Amusing::CompletePolygon> parent1, std::shared_ptr<Amusing::CompletePolygon> parent2, std::shared_ptr<MultiAreaEvent> multiE);
		void ApplyFusion(std::shared_ptr<Amusing::CompletePolygon> currentPolygon, std::shared_ptr<Amusing::CompletePolygon> hitPolygon, std::shared_ptr<AreaEvent> singleE);
		void DeleteIntersections(std::shared_ptr<Amusing::CompletePolygon> parent);
		std::shared_ptr<AreaEvent> DeleteSelectedArea();
		std::shared_ptr<AreaEvent> DeleteCursor(std::shared_ptr<Cursor> cursorToDelete);

		void AddCursor();
		std::shared_ptr<AreaEvent> AddCursor(std::shared_ptr<IDrawableArea> area);
		std::map<std::shared_ptr<Cursor>, std::shared_ptr<IDrawableArea>> associateArea;
		//std::vector<std::shared_ptr<Cursor>> cursors;
		std::shared_ptr<IDrawableArea> getAssociateArea(std::shared_ptr<Cursor> cursor);

		std::shared_ptr<AreaEvent> SetSelectedArea(std::shared_ptr<IEditableArea> selectedArea_, bool changeMode = true) override;
		std::shared_ptr<AreaEvent> SetSelectedAreaCursor(int idx, double newSize);
		std::shared_ptr<AreaEvent> SetSelectedAreaOpacity(double newOpacity);
	};


}



#endif  // AMUSINGSCENE_H_INCLUDED
