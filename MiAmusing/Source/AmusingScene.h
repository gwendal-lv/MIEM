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
#include "IntersectionPolygon.h"
#include "Cursors.h"
#include "IDrawableArea.h"
#include "../JuceLibraryCode/JuceHeader.h"

using namespace Miam;

namespace Amusing
{
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

		

		std::shared_ptr<AreaEvent> AddNedgeArea(uint64_t nextAreaId, int N);
		void AddIntersections(std::shared_ptr<IDrawableArea> m_area);
		void AddAllIntersections();
		size_t getIntersectionDrawingIndex(size_t intersectionVectorIndex);
		// override mouse callback

		std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE) override;
		std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE) override;
		std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE) override;
		std::shared_ptr<GraphicEvent> OnCanvasMouseDoubleClick(const MouseEvent& mouseE);

		std::shared_ptr<GraphicEvent> resetAreaPosition();

		std::shared_ptr<AreaEvent> AddTrueCircle(uint64_t nextAreaId);
		std::shared_ptr<AreaEvent> AddCompleteArea(uint64_t);

		
		int Nfollower;
		std::shared_ptr<Amusing::CompletePolygon> getFirstCompleteArea();
		std::shared_ptr<Miam::MultiAreaEvent> SetAllAudioPositions(double position);

		//std::shared_ptr<Amusing::AnimatedPolygon> hitPolygon(Point<double> hitPoint, std::shared_ptr<Amusing::AnimatedPolygon> polygon);
		
		/*
		virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE) override;
		virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE) override;
		virtual std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE) override;
		*/
		// private attribut

		//Model *model;

		int getNumberArea();
		std::shared_ptr<AreaEvent> OnDelete();
		bool deleting;
		std::shared_ptr<AreaEvent> deleteEvent;
		

		std::vector< std::shared_ptr<Amusing::IntersectionPolygon> > currentIntersectionsAreas;

		std::shared_ptr<Amusing::IntersectionPolygon> getNextChildOf(std::shared_ptr<Amusing::CompletePolygon> parent, int index);

		void addChords(std::shared_ptr<Amusing::CompletePolygon> parent1, std::shared_ptr<Amusing::CompletePolygon> parent2, std::shared_ptr<MultiAreaEvent> multiE);

		// map with 2 entry = the parents polygons, and 1 output = the vector of overlap area between the two parent polygon
		std::map<std::pair<std::shared_ptr<Amusing::CompletePolygon>, std::shared_ptr<Amusing::CompletePolygon>>, std::vector<std::shared_ptr<Amusing::CompletePolygon>>> parentTochildArea;
		bool getParents(std::shared_ptr<IEditableArea> child, std::shared_ptr<IEditableArea> &parent1, std::shared_ptr<IEditableArea> &parent2);
		std::shared_ptr<Amusing::CompletePolygon> getConcernedIntersection(std::shared_ptr<Amusing::CompletePolygon> parent1, std::shared_ptr<Amusing::CompletePolygon> parent2, bpt hitPoint);
		std::shared_ptr<AreaEvent> DeleteSelectedArea();
		std::shared_ptr<AreaEvent> DeleteCursor(std::shared_ptr<Cursor> cursorToDelete);

		bool alreadyCursorInScene;
		void AddCursor();
		std::shared_ptr<AreaEvent> AddCursor(std::shared_ptr<IDrawableArea> area);
		

		std::shared_ptr<AreaEvent> SetSelectedArea(std::shared_ptr<IEditableArea> selectedArea_, bool changeMode = true) override;
		std::shared_ptr<AreaEvent> SetSelectedAreaCursor(int idx, double newSize);
		std::shared_ptr<AreaEvent> SetSelectedAreaOpacity(double newOpacity);
		std::shared_ptr<AreaEvent> SetSelectedAreaColour(Colour newColour);
		bool isDrew(std::shared_ptr<Cursor> cursor);
		std::shared_ptr<AreaEvent> checkCursorPosition(std::shared_ptr<Cursor> cursor, int &areaId);
		void lookForAreasToUpdate(Colour concernedColour);
		std::shared_ptr<bptree::ptree> GetTree() const override;
		
		virtual std::shared_ptr<MultiAreaEvent> OnSelection(bool resetExciters = true) override; // on écrase resetExciters dans la fonction pour pas réinitialiser les exciters
		virtual std::shared_ptr<MultiAreaEvent> OnUnselection(bool shutExcitersDown = true) override; // on remet aussi le paramètre par défaut à 0
	private:
		std::map<int, std::shared_ptr<Amusing::CompletePolygon>> mouseIdxToArea;
		bpt previousAreaLocation;
	};


}



#endif  // AMUSINGSCENE_H_INCLUDED
