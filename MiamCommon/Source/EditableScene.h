/*
  ==============================================================================

    EditableScene.h
    Created: 17 Nov 2016 4:54:53pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef EDITABLESCENE_H_INCLUDED
#define EDITABLESCENE_H_INCLUDED

#include <memory>
#include <vector>
#include <string>

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
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        std::shared_ptr<IEditableArea> GetEditableArea(size_t i);
        
        virtual void SetSelectedArea(std::shared_ptr<IEditableArea> selectedArea_, bool changeMode = true);
        virtual std::shared_ptr<IEditableArea> GetSelectedArea() {return selectedArea;}
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction and Destruction (and helpers) - - - - -
        EditableScene(MultiSceneCanvasInteractor* canvasManager_, SceneCanvasComponent* canvasComponent_, bool allowAreaSelection_ = true);
        virtual ~EditableScene();
        
        
        // - - - - - Areas Managing : Add and Delete - - - - -
        
        //void AddArea(std::shared_ptr<IEditableArea> newArea, bool selectArea = false);
        
        void AddDefaultArea(uint64_t nextAreaId);
      
        void DeleteSelectedArea();
        
        protected :
        void deleteAreaByUniqueId(uint64_t uidToDelete);

        
        // ------ areas managing : graphical attributes ------
        public :
        
        void ChangeSelectedAreaColour(Colour& colour);
        
        void AddPoint();
        void DeletePoint();
        
        
        // ------ areas managing : Z-Order ------
        public :
        
        void SendSelectedAreaToBack();
        void SendSelectedAreaBackward();
        void BringSelectedAreaForward();
        void BringSelectedAreaToFront();
        
        
        
        // - - - - - Selection events managing (orders from parent manager) - - - - -
        virtual void OnSelection() override;
        virtual void OnUnselection() override;
        
        
        // - - - - - Canvas (mouse) events managing - - - - -
        
        /// \brief Implements the beginning of the editing of a Miam::EditableArea,
        /// depending on the mouse/touch/pen events given
        ///
        /// Two different mouse (touch) events managing possibilites :
        ///
        /// n°1 : editon/manipulation/play with a unique selected area only (in
        /// the future, there may be several selected areas), with 1 touch input
        /// source (may also be changed)
        ///
        /// n°2 : editon/manipulation/play with any area (they all stay in the
        /// same state), with an arbitrary number of touch inputs
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
