/*
  ==============================================================================

    SceneEditionManager.h
    Created: 28 Mar 2016 5:27:18pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENEEDITIONMANAGER_H_INCLUDED
#define SCENEEDITIONMANAGER_H_INCLUDED


#include <memory>
#include <vector>
#include <deque>
#include <map>

#include "EditablePolygon.h"

#include "Juceheader.h" // for : Point<float>, uint64

#include "SceneEditionComponent.h"
#include "SceneEditionMode.h"

#include "SceneCanvasManager.h"
#include "SceneCanvasComponent.h"

namespace Miam {
    
    // Simple declarations
    class View;
    
    
    /// \brief Sub-module belonging to the Presenter module, which handles the editing
	/// of all of the EditableArea.
	///
	/// Actually owns all the EditablePolygons (and any other EditableArea in the future).
	///
	/// References itself to the SceneEditionComponent and the several
	/// SceneCanvasComponents, for these components to transfer events to this sub-module
	/// directly, and not to the Presenter.
    class SceneEditionManager {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        // Graphical objects belong to the Presenter module, not to the View
        private :
        // links back to the View module
        View* view;
        SceneEditionComponent* sceneEditionComponent;
        
        protected :
        // internal states
        SceneEditionMode mode;
        
        // Graphic objects instances
        std::vector< std::shared_ptr<EditablePolygon> > polygons;
        
        // Graphic objects management
        int64_t nextAreaId; // unique Id of the created (or pasted, or loaded...) area
        // maps a unique DrawableArea Id to its polygon vector index (for links with Spkrs Configs)
        std::map< int64_t, int > polygonsVectorIndexes;
        // TODO : a map for each kind of area !! if we can't find the Id, it's wasn't a polygon (maybe an ellipse, or something else...)
        
        
        // Display & editing attributes for Canvases
        std::vector< SceneCanvasManager* > canvasManagers;
        SceneCanvasManager* selectedCanvas = 0;
        
        
        // Display & editing attributes for Areas
        
        std::shared_ptr<EditableArea> areaToCopy = nullptr; ///< vector index of the polygon to copy (idem for value -1)
        
        /// \brief Indicates the drawing order of areas (contents of each std::deque
        /// element), for a given canvas (canvas id is an position of the std::vector)
        ///
        /// Area at index zero is the first drawn (lowest layer). Size of a second-level
        /// deque = size of drawable areas of the corresponding canvas = size of
        /// editable areas of the corresponding canvas
        std::vector<std::deque<int64_t>> areasOrder;
        
        /// \brief Indicates the SceneCanvasComponent::Id of the canvas on which an area
        /// (referenced by its unique ID) is drawn.
        std::map< int64_t, int > areaCanvasId;
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        /// \brief Construction (the whole Presenter module is built after the View).
        SceneEditionManager(View* _view);
        
        /// \brief Destruction and the editor and the canvases
        ~SceneEditionManager();
        
        // Debug purposes only
        void __LoadDefaultTest();
       
        // ---- Getters and Setters -----
        public :
        /// \brief When a new area is to be created, this gets the value of the next area ID, and increments it
        uint64_t GetNextAreaId();
        protected :
		/// \brief Polygon referenced by its unique ID, from any canvas (or copied/deleted)
		///
		/// Returns a nullptr if nothing found
        std::shared_ptr<EditablePolygon> GetEditablePolygon(int64_t uniqueId);
        /// \brief Generic editable area, referenced by its unique ID
        ///
        /// Returns a nullptr if nothing found
        //std::shared_ptr<EditableArea> GetEditableArea(int64_t uniqueId);
        public :
        /// \brief Gets the currently selected area, or nullptr if nothing selected
        std::shared_ptr<EditableArea> GetSelectedArea();
        
        
        
        // ------ canvas managing ------
        public :
        /// \brief Sets the new active canvas and updates corresponding graphic objects. Called by the newly selected canvas itself.
        ///
        /// Tells other canvases to unselect any previously selected area
        void SetSelectedCanvas(SceneCanvasManager*);
        
        
		// ----- Running mode -----
        protected :
		/// \brief Launches the necessary graphic updates, then changes the running mode.
        void setMode(SceneEditionMode newMode);
        public :
        /// \brief The reaction of this manager when one its canvas sub-components
        /// changes itself its working mode
        void CanvasModeChanged(SceneCanvasMode);
        
        // ----- Events from View -----
        public :
        
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
