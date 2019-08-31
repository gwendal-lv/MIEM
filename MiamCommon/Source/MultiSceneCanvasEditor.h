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

#ifndef MULTISCENECANVASEDITOR_H_INCLUDED
#define MULTISCENECANVASEDITOR_H_INCLUDED



#include "MultiSceneCanvasInteractor.h"


using namespace Miam;

namespace Miam {
    // Pre-declaration of pointers
    
    
    
    
    /// \brief Allows the edition of the canvas itself
    class MultiSceneCanvasEditor : public MultiSceneCanvasInteractor
    {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        
        
        public :
        
        // Construction and destruction
        MultiSceneCanvasEditor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId);
        virtual ~MultiSceneCanvasEditor();
        
        
        // ----- Running Mode -----
        public :
        /// \brief Implements special behaviors reserved to editing phases
        virtual void SetMode(CanvasManagerMode _mode) override;
        
        
        
        // ------ Setters and Getters ------
        
        virtual std::shared_ptr<IEditableArea> GetSelectedArea();
        virtual void SetSelectedSceneName(std::string _name);
        
        std::vector<std::shared_ptr<EditableScene>>::iterator GetSelectedSceneIt();
        
        
        
        // ------ Scenes managing ------
        
        /// \brief Moves the selected scene towards the first position, if possible
        ///
        /// \return Wether the move was actually done or not
        virtual bool MoveSelectedSceneTowardsFirst();
        /// \brief Moves the selected scene towards the last position, if possible
        ///
        /// \return Wether the move was actually done or not
        virtual bool MoveSelectedSceneTowardsLast();
        
        
        
        // ------ Areas direct managing : Add and Delete ------
        // Possible because there should always be at least one selected scene
        
        virtual void AddArea(std::shared_ptr<IEditableArea> newArea);
        virtual void AddDefaultArea(uint64_t nextAreaId);
        virtual void SetSelectedArea(std::shared_ptr<IEditableArea> newSelectedArea);
        virtual void DeleteSelectedArea();
        
        
        // ---------- Events from a canvas component (from View) ----------
        public :
        
        virtual void OnNewColour(Colour colour);
        
        virtual void OnSendBackward();
        virtual void OnSendToBack();
        virtual void OnBringForward();
        virtual void OnBringToFront();
        
        
        
    };
    
}





#endif  // MULTISCENECANVASEDITOR_H_INCLUDED
