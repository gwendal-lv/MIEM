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

#include "GraphicSpatSessionManager.h"

#include "EditablePolygon.h"

#include "JuceHeader.h" // for : Point<float>, uint64

#include "MultiSceneCanvasManager.h"
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
	/// References itself to some components, for these components to transfer events to this sub-module
	/// directly, and not to the Presenter.
    class GraphicSessionManager : public GraphicSpatSessionManager {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        // Graphical objects belong to the Presenter module, not to the View
        private :
        
        Presenter* presenter;
        
        // links back to the View module
        View* view;
        
        
        
        // Graphic Manager mode = useless for now (will rely on the managed
        // canvases modes)
        //GraphicSessionMode mode;
        
        // Display & editing attributes for Areas
        
        std::shared_ptr<IEditableArea> areaToCopy = nullptr; ///< vector index of
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        /// \brief Construction (the whole Presenter module is built after the View).
        GraphicSessionManager(Presenter* presenter_, View* view_);
        
        /// \brief Destruction and the editor and the canvases
        ~GraphicSessionManager();
        
        
        
        // ---- Getters and Setters -----
        public :        
        
        /// \brief Gets the currently selected area, or nullptr if nothing selected
        std::shared_ptr<IEditableArea> GetSelectedArea();
        
        
        
        // ------ canvas managing ------
        public :
        
        
        protected :
        std::shared_ptr<MultiSceneCanvasManager> getSelectedCanvasAsManager();
        
        
		// ----- Running mode -----
        public :
        
        void CanvasModeChanged(CanvasManagerMode) override;
        
        
        
        
        // ----- Events from the Presenter itself -----
        /// \brief Selects all event that might have an effect on the global spatialization
        /// state, translates it into data that the Model understands, then sends it
        /// to the Model via the Presenter lock-free queue.
        ///
        /// Seule info importante à transmettre :
        /// - changement d'excitation d'une aire graphique
        ///   (qu'on transmet le + vite possible !!)
        ///   -> cette info sera dans un event spécial déjà traité dans un CanvasInteractor
        ///
        /// Infos dont on se fout :
        /// - toute info géométrique sur une aire...
        virtual void HandleEventSync(std::shared_ptr<GraphicEvent> event_) override;
        
        private :
        // On ne sait pas si c'est un simple ou multi
        void handleAreaEventSync(const std::shared_ptr<AreaEvent>& areaE);
        // Là ça doit absolument être un simple
        void handleSingleAreaEventSync(const std::shared_ptr<AreaEvent>& areaE);
        
        public :
        // Déclenche des actualisations forcées, ...
        void OnModelStarted();
        
        
        
        // ----- Event to View -----
        public :
        void DisplayInfo(String info) override;

        
        // - - - - - XML import/export - - - - -
        virtual void SetFromTree(bptree::ptree& graphicSessionTree) override;

    };
    
    
}


#endif  // GRAPHICSESSIONMANAGER_H_INCLUDED
