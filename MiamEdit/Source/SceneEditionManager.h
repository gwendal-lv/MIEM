/*
  ==============================================================================

    SceneEditionManager.h
    Created: 28 Mar 2016 5:27:18pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENEEDITIONMANAGER_H_INCLUDED
#define SCENEEDITIONMANAGER_H_INCLUDED


#include <vector>
#include <deque>
#include <map>

#include "EditablePolygon.h"

#include "Juceheader.h" // for : Point<float>, uint64

#include "SceneEditionComponent.h"
#include "SceneEditionMode.h"


namespace Miam {
    
    // Simple declarations
    class View;
    
    
    /// \brief Sub-module belonging to the Presenter module, which handles the editing
	/// of all of the EditableArea.
	///
	/// Actually owns all the EditablePolygons (and other EditableArea in the future).
	///
	/// References itself to the SceneEditionComponent and the several
	/// SceneCanvasComponents, for these components to transfer events to this sub-module
	/// directly, and not to the Presenter.
    class SceneEditionManager {
        
        
        // Graphical objects belong to the Presenter module, not to the View
        private :
        
        // internal states
        SceneEditionMode mode;
        
        // links back to the View module
        View* view;
        SceneEditionComponent* sceneEditionComponent;
        
        // Graphic objects instances
        std::vector<EditablePolygon> polygons;
        
        // Graphic objects management
        int64_t nextAreaId; // unique Id of the created (or pasted, or loaded...) area
        // maps a unique DrawableArea Id to its polygon vector index (for links with Spkrs Configs)
        std::map< int64_t, int > polygonsVectorIndexes;
        // TODO : a map for each kind of area !! if we can't find the Id, it's wasn't a polygon (maybe an ellipse, or something else...)
        
        // Display & editing attributes
        std::deque<int64_t> areasOrder; // area at index zero is the first drawn (lowest layer). Size = size of drawable areas = size of editable areas
        int64_t selectedAreaUniqueId = -1; // -1 means no area selected
        int64_t areaToCopy = -1; // vector index of the polygon to copy (idem considered value -1)
        
        
        public :
        /// \brief Construction (the whole Presenter module is built after the View).
        SceneEditionManager(View* _view);
        
        // ---- Getters and Setters -----
        protected :
		/// \brief Polygon referenced by its unique ID
		///
		/// Throws an std::range_error exception if unique ID not found.
        EditablePolygon& GetEditablePolygon(int64_t uniqueId);
        // Generic objects
        //EditableArea& GetEditableArea(int64_t uniqueId);
        public :
		/// \brief Amount of areas available for drawing.
        size_t GetDrawableAreasSize();
		/// \brief Get an area's reference for drawing.
        DrawableArea& GetDrawableArea(int position);
        
        
        // ------ areas managing ------
        protected :
		/// \brief Creates a polygon, adds it internally and performs all the
		/// necessary graphic updates.
        void addEditablePolygon(EditablePolygon& newPolygon, bool selectArea = false);
		/// \brief Deletes an area referenced by its unique ID.
        void deleteEditableArea(int64_t uniqueId);
		/// \brief Sets a new active area, and commands all necessary graphic updates.
        void setSelectedAreaUniqueId(int64_t newIndex);

		// ----- Running mode -----
		/// \brief Launches the necessary graphic updates, then changes the running mode.
        void setMode(SceneEditionMode newMode);
        
        
        // ----- Events from View -----
        public :
        
        void OnCanvasMouseDown(Point<int> clicLocation);
        void OnCanvasMouseDrag(Point<int> mouseLocation);
        void OnCanvasMouseUp();
        
        void OnAddPoint();
        void OnDeletePoint();
        
        void OnCopyArea();
        void OnPasteArea();
        void OnAddArea();
        void OnDeleteArea();
        
        void OnNewColour(Colour colour);
        
        void OnSendBackward();
        void OnSendToBack();
        void OnBringForward();
        void OnBringToFront();
        
    };
    
    
}


#endif  // SCENEEDITIONMANAGER_H_INCLUDED
