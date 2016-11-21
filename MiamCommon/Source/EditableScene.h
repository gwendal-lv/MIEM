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
        private :
        
        /// \brief The currently selected area (may be a null pointer)
        std::shared_ptr<IEditableArea> selectedArea = nullptr;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        std::shared_ptr<IEditableArea> GetEditableArea(size_t i);
        
        void SetSelectedArea(std::shared_ptr<IEditableArea> _selectedArea, bool changeMode = true);
        std::shared_ptr<IEditableArea> GetSelectedArea() {return selectedArea;}
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction and Destruction (and helpers) - - - - -
        EditableScene(MultiSceneCanvasInteractor* _canvasManager, SceneCanvasComponent* _canvasComponent);
        
        
        // - - - - - Areas Managing : Add and Delete - - - - -
        
        //void AddArea(std::shared_ptr<IEditableArea> newArea, bool selectArea = false);
        
        void AddDefaultArea(uint64_t _nextAreaId);
      
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
        
        
        // - - - - - Canvas (mouse) events managing - - - - -
        
        
        virtual std::string OnCanvasMouseDown(Point<int>& clicLocation) override;
        virtual bool OnCanvasMouseDrag(Point<int>& mouseLocation) override;
        virtual bool OnCanvasMouseUp() override;
        
        
    };
    
    
    
}




#endif  // EDITABLESCENE_H_INCLUDED
