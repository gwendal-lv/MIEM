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
#ifndef EDITABLESCENE_H_INCLUDED
#define EDITABLESCENE_H_INCLUDED

#include <memory>
#include <vector>
#include <string>
#include <initializer_list>

#include "InteractiveScene.h"

#include "AreaEvent.h"



using namespace Miam;


namespace Miam
{
    
    // pre-declarations
    class IEditableArea;
    class MultiSceneCanvasInteractor;
    
    
    /// \brief A graphical scene made of areas and exciters, to be drawn on a SceneCanvasComponent
    ///
    ///
    class EditableScene : public InteractiveScene
    {
     
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        bool allowAreaSelection;
        
        /// \brief The currently selected area (may be a null pointer)
        std::shared_ptr<IEditableArea> selectedArea = nullptr;
        
        // After a special action on "mouse down" event, we may want sometimes
        // to forget all events following this "mouse down" event, until the next
        // "mouse up" event.
        //
        // Example : after addition or deletion, nothing moves on drag anymore.
        bool bypassMouseEventsUntilMouseUp = false;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        std::shared_ptr<IEditableArea> GetEditableArea(size_t i);
        
        virtual std::shared_ptr<AreaEvent> SetSelectedArea(std::shared_ptr<IEditableArea> selectedArea_, bool changeMode = true);
        std::shared_ptr<IEditableArea> GetSelectedArea() {return selectedArea;}
        
        /// \brief The actual ID is there is a selected area, or -1 if there isn't.
        virtual int GetSelectedAreaSceneId();
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction and Destruction (and helpers) - - - - -
        EditableScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_, bool allowAreaSelection_ = true);
        virtual ~EditableScene();
        
        
        // - - - - - Areas Managing : Add and Delete - - - - -
                
        std::shared_ptr<AreaEvent> AddDefaultArea(uint64_t nextAreaId);
      
        /// \brief Deletes the selected area, and always returns its ID within the
        /// created AreaEvent
        ///
        /// Throws an exception if nothing is selected
        std::shared_ptr<AreaEvent> DeleteSelectedArea();
        
        protected :
        std::shared_ptr<AreaEvent> deleteAreaByUniqueId(uint64_t uidToDelete);

        
        // ------ areas managing : graphical attributes ------
        public :
        
        std::shared_ptr<AreaEvent> ChangeSelectedAreaColour(Colour& colour);
        
        void AddPoint();
        void DeletePoint();
        
        
        // ------ areas managing : Z-Order ------
        public : // public interfaces
        void SendSelectedAreaToBack();
        void SendSelectedAreaBackward();
        void BringSelectedAreaForward();
        void BringSelectedAreaToFront();
        
        
        
        // - - - - - Selection events managing (orders from parent manager) - - - - -
        public :
        virtual std::shared_ptr<MultiAreaEvent> OnSelection(bool resetExciters = true) override;
        virtual std::shared_ptr<MultiAreaEvent> OnUnselection(bool shutExcitersDown = true) override;
        
        
        // - - - - - Canvas (mouse) events managing - - - - -
        
        /// \brief Implements the beginning of the editing of a Miam::EditableArea,
        /// depending on the mouse/touch/pen events given
        ///
        /// Two different mouse (touch) events managing possibilites :
        ///
        /// n°1 : editon/manipulation/play with a unique selected area only (in
        /// the future, there may be several selected areas), with 1 touch input
        /// source (may also be changed)
        /// -> édition avec 1 entrée, 1 (ou plusieurs à l'avenir) aires contrôlées
        ///
        /// n°2 : editon/manipulation/play with any area (they all stay in the
        /// same state), with an arbitrary number of touch inputs
        /// -> édition multi-entrées, multi-aires
        ///
        ///
        /// --------------------------------------------------------------------
        /// --------------------------------------------------------------------
        /// --------------------------------------------------------------------
        /// --------------------------------------------------------------------
        /// --------------------------------------------------------------------
        /// TRANSFORMER LES PROTOTYPES : ON N'AURA QUE DES AREA EVENT
        /// --------------------------------------------------------------------
        /// --------------------------------------------------------------------
        /// --------------------------------------------------------------------
        /// --------------------------------------------------------------------
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE) override;
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE) override;
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE) override;

        
    };
    
    
    
}




#endif  // EDITABLESCENE_H_INCLUDED
