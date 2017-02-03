/*
==============================================================================

GraphicSessionManager.h
Created: 28 Mar 2016 5:27:18pm
Author:  Gwendal Le Vaillant

==============================================================================
*/

#ifndef GRAPHICSESSIONMANAGER_H_INCLUDED
#define GRAPHICSESSIONMANAGER_H_INCLUDED


#include <memory>
#include <vector>
#include <deque>
#include <map>
#include <string>

#include "IGraphicSessionManager.h"

#include "EditablePolygon.h"

#include "Juceheader.h" // for : Point<float>, uint64

#include "MultiSceneCanvasManager.h"
#include "SceneCanvasComponent.h"

#include "editScene.h"

namespace Miam
{
	class IPresenter;
}

class EditScene;

namespace Amusing {

	// Simple declarations
	class View;
	class Presenter;


	/// \brief Sub-module belonging to the Presenter module, which handles the editing
	/// of all of the EditableArea.
	///
	/// Actually owns all the EditablePolygons (and any other EditableArea in the future).
	///
	/// References itself to some components, for these components to transfer events to this sub-module
	/// directly, and not to the Presenter.
	class GraphicSessionManager : public Miam::IGraphicSessionManager,
		public Thread
	{

	public :
		// =========================================
		void playSound();
		void run();
		double vitesse;
		// =========================================

		// = = = = = = = = = = ATTRIBUTES = = = = = = = = = =

		// Graphical objects belong to the Presenter module, not to the View
	private:
		// links back to the View module
		View* view;
		EditScene *editScene;
		Presenter *myPresenter;

		// Graphic Manager mode = useless for now (will rely on the managed
		// canvases modes)
		//GraphicSessionMode mode;

		// Display & editing attributes for Areas

		std::shared_ptr<IEditableArea> areaToCopy = nullptr; ///< vector index of




															 // = = = = = = = = = = METHODS = = = = = = = = = =

	public:
		/// \brief Construction (the whole Presenter module is built after the View).
		GraphicSessionManager(Presenter* presenter_, View* view_);
		//GraphicSessionManager(IPresenter* presenter_, View* view_);

		/// \brief Destruction and the editor and the canvases
		~GraphicSessionManager();

		// Debug purposes only
		void __LoadDefaultTest();



		// ---- Getters and Setters -----
	public:

		/// \brief Gets the currently selected area, or nullptr if nothing selected
		std::shared_ptr<IEditableArea> GetSelectedArea();



		// ------ canvas managing ------
	public:
		/// \brief Sets the new active canvas and updates corresponding graphic objects. Called by the newly selected canvas itself.
		///
		/// Tells other canvases to unselect any previously selected area
		

	protected:
		std::shared_ptr<MultiSceneCanvasManager> getSelectedCanvasAsManager();

	protected:
		std::shared_ptr<MultiSceneCanvasEditor> getSelectedCanvasAsEditable();


		// ----- Running mode -----
	public:

		void CanvasModeChanged(CanvasManagerMode) override;

		


		// ----- Events from the Presenter itself -----
		virtual void HandleEventSync(std::shared_ptr<GraphicEvent> event_) override;



		// ----- Event to View -----
		void DisplayInfo(String info) override;

		void OnAddArea();
		void OnAddSquare();
		void OnAddTriangle();
		void OnAddCircle();
		void OnAddTrueCircle();

		void OnPlayClicked();
		void OnPauseClicked();
		void OnStopClicked();

	};


}


#endif  // GRAPHICSESSIONMANAGER_H_INCLUDED
