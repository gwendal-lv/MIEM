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

#include "SceneEditionComponent.h"
#include "GraphicSessionMode.h"

#include "MultiSceneCanvasEditor.h"
#include "SceneCanvasComponent.h"

namespace Miam {
    
    // Simple declarations
    class View;
    class Presenter;
    
    
    /// \brief Sub-module belonging to the Presenter module, which handles the editing
	/// of all of the EditableArea.
	///
	/// Actually owns all the EditablePolygons (and any other EditableArea in the future).
	///
	/// References itself to the SceneEditionComponent and the several
	/// SceneCanvasComponents, for these components to transfer events to this sub-module
	/// directly, and not to the Presenter.
    class GraphicSessionManager : public IGraphicSessionManager {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        // Graphical objects belong to the Presenter module, not to the View
        private :
        // links back to the View module
        View* view;
        SceneEditionComponent* sceneEditionComponent;
        
        // internal states
        GraphicSessionMode mode;
        
        
        
        // Display & editing attributes for Areas
        
        /// \brief Unique Id of the next created (or pasted, or loaded...) area
        int64_t nextAreaId;
        std::shared_ptr<IEditableArea> areaToCopy = nullptr; ///< vector index of
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        /// \brief Construction (the whole Presenter module is built after the View).
        GraphicSessionManager(View* _view, Presenter* presenter_);
        
        /// \brief Destruction and the editor and the canvases
        ~GraphicSessionManager();
        
        // Debug purposes only
        void __LoadDefaultTest();
       
        
        
        // ---- Getters and Setters -----
        public :
        uint64_t GetNextAreaId() override;
        
        
        /// \brief Gets the currently selected area, or nullptr if nothing selected
        std::shared_ptr<IEditableArea> GetSelectedArea();
        
        
        
        // ------ canvas managing ------
        public :
        /// \brief Sets the new active canvas and updates corresponding graphic objects. Called by the newly selected canvas itself.
        ///
        /// Tells other canvases to unselect any previously selected area
        void SetSelectedCanvas(MultiSceneCanvasInteractor*) override;
        
        protected :
        MultiSceneCanvasEditor* getSelectedCanvasAsEditable();
        
        
		// ----- Running mode -----
        protected :
		/// \brief Launches the necessary graphic updates, then changes the running mode.
        void setMode(GraphicSessionMode newMode);
        public :
        
        void CanvasModeChanged(CanvasManagerMode) override;
        
        
        
        
        // ----- Events from the Presenter itself -----
        
        virtual void HandleEventSync(std::shared_ptr<GraphicEvent> event_) override;
        
        
        // ----- Event to View -----
        void DisplayInfo(String info) override;
        
        
        // ----- Events from View -----
        public :
        
        void OnAddScene();
        void OnDeleteScene();
        void OnSceneLeft();
        void OnSceneRight();
        
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
        
        void OnSceneNameChange(std::string _name);
        
        
    };
    
    
}


#endif  // GRAPHICSESSIONMANAGER_H_INCLUDED
