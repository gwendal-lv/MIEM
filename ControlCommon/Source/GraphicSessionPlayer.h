/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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

#include "MultiSceneCanvasPlayer.h"
#include "SceneCanvasComponent.h"

namespace Miam {
    
    // Simple declarations
    class PlayerView;
    class PlayerPresenter;
    
    
    /// \brief Sub-module belonging to the Presenter module, which handles the editing
	/// of all of the EditableArea.
	///
	/// Actually owns all the EditablePolygons (and any other EditableArea in the future).
	///
	/// References itself to some components, for these components to transfer events to this sub-module
	/// directly, and not to the Presenter.
    class GraphicSessionPlayer : public GraphicControlSessionManager {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        // Graphical objects belong to the Presenter module, not to the View
        private :
        
        PlayerPresenter* presenter;
        
        // links back to the View module
        PlayerView* view;
        
        
        
        // Graphic Manager mode = useless for now (will rely on the managed
        // canvases modes)
        //GraphicSessionMode mode;
        
        // Display & editing attributes for Areas
        
        std::shared_ptr<IEditableArea> areaToCopy = nullptr; ///< vector index of
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        /// \brief Construction (the whole Presenter module is built after the View).
        GraphicSessionPlayer(PlayerPresenter* presenter_, PlayerView* view_);
        
        /// \brief Destruction and the editor and the canvases
        ~GraphicSessionPlayer();
        
        
        
        // ---- Getters and Setters -----
        public :        
        
        /// \brief Gets the currently selected area, or nullptr if nothing selected
        std::shared_ptr<IEditableArea> GetSelectedArea();
        
        
        
        // ------ canvas managing ------
        public :
        
        
        protected :
        // plus utile, si ? EN tout cas : ne peut pas être généralisé
        //std::shared_ptr<MultiSceneCanvasPlayer> getSelectedCanvasAsManager();
        
        
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
        
        /// \brief Tries to select a scene of the currently selected canvas
        ///
        /// From remote on network. Returns true if the scene could be selected
        bool SelectSceneOfSelectedCanvas(size_t sceneId);
        
        
        
        // ----- Event to View -----
        public :
        void DisplayInfo(String info, int priority = 0) override;

        
        // - - - - - XML import/export - - - - -
        virtual void SetFromTree(bptree::ptree& graphicSessionTree) override;

    };
    
    
}


#endif  // GRAPHICSESSIONMANAGER_H_INCLUDED
