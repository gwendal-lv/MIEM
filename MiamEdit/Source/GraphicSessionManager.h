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

#include "GraphicControlSessionManager.h"

#include "EditablePolygon.h"

#include "JuceHeader.h" // for : Point<float>, uint64

#include "SceneEditionComponent.h"
#include "GraphicSessionMode.h"
#include "AppPurpose.h"

#include "MultiSceneCanvasEditor.h"
#include "SceneCanvasComponent.h"

#include "StatesInterpolator.hpp"

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
    class GraphicSessionManager : public GraphicControlSessionManager {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        
        // links back to the View module
        View* view;
        Presenter* presenter;
        SceneEditionComponent* sceneEditionComponent;
        
        // internal states
        GraphicSessionMode mode;
        
        
        
        // Display & editing attributes for Areas
        std::shared_ptr<IEditableArea> areaToCopy = nullptr; ///< vector index of
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        /// \brief Construction (the whole Presenter module is built after the View).
        GraphicSessionManager(View* _view, Presenter* presenter_);
        
        /// \brief Destruction and the editor and the canvases
        virtual ~GraphicSessionManager();
        
        
        // ---- Getters and Setters -----
        public :
        uint64_t GetNextAreaId() override;
        AppPurpose GetSessionPurpose();
        
        /// \brief Gets the currently selected area, or nullptr if nothing selected
        ///
        /// Throws an exception is the area cannot be casted to Miam::ControlArea
        std::shared_ptr<ControlArea> GetSelectedArea();
        
        
        
        // ------ canvas managing ------
        public :
        /// \brief Sets the new active canvas and updates corresponding graphic objects. Called by the newly selected canvas itself.
        ///
        /// Tells other canvases to unselect any previously selected area
        void SetSelectedCanvas(std::shared_ptr<MultiSceneCanvasInteractor>) override;
        
        protected :
        std::shared_ptr<MultiSceneCanvasEditor> getSelectedCanvasAsEditable();
        
        
		// ----- Running mode -----
        protected :
		/// \brief Launches the necessary graphic updates, then changes the running mode.
        ///
        /// Cette fonction est parfois appelée de manière indirecte : en appelant
        /// SetMode(...) du canevas sélectionné, qui retransmet son mode à lui
        void setMode(GraphicSessionMode newMode);
        public :
        
        void CanvasModeChanged(CanvasManagerMode) override;
        
        
        
        
        // ----- Events from the Presenter itself -----
        /// \brief Updates the data that may have been modified during AppMode
        void OnEnterSpatScenesEdition();
        std::shared_ptr<bptree::ptree> OnLeaveSpatScenesEdition();
        
        virtual void HandleEventSync(std::shared_ptr<GraphicEvent> event_) override;
        
        
        // ----- Event to View -----
        virtual void DisplayInfo(String info, int priority = 0) override;
        
        
        // ----- Events from View, transmitted to the selected Canvas Manager -----
        public :
        
        void OnAddScene();
        void OnDeleteScene();
        void OnSceneLeft();
        void OnSceneRight();
        void OnSceneNameChange(std::string _name);
        
        void OnAddPoint();
        void OnDeletePoint();
        
        void OnAddArea(int areaType = AreaDefaultType::Polygon+1); // default = square
        void OnDeleteArea();
        
        void OnSendBackward();
        void OnSendToBack();
        void OnBringForward();
        void OnBringToFront();
        
        void OnNewColour(Colour colour);
        
        void OnAddExciter();
        void OnDeleteExciter();
        
        // ----- Events from View, internally processed -----
        // Events treated by this class, and not by the selected CanvasManager
        // because it is specifically related to spatialization
        public :
        
        void OnCopyArea();
        void OnPasteArea();
        
        void OnSpatStateChanged(int spatStateIdx);
        
        void OnEnterExcitersEdition();
        void OnQuitExcitersEdition();
        
        
        
    };
    
    
}


#endif  // GRAPHICSESSIONMANAGER_H_INCLUDED
