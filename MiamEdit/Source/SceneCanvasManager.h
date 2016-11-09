/*
  ==============================================================================

    SceneCanvas.h
    Created: 8 Oct 2016 1:02:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENECANVAS_H_INCLUDED
#define SCENECANVAS_H_INCLUDED

#include <memory>

#include <deque>

#include "SceneCanvasMode.h"

#include "SceneEditionComponent.h"
#include "SceneCanvasComponent.h"

#include "EditableArea.h"
#include "EditablePolygon.h"

namespace Miam {
    class View;
    
    
    /// \brief The "Presenter-side" methods and attributes of a SceneCanvasComponent
    ///
    ///
    class SceneCanvasManager
    {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        protected :
        /// \brief Link to the unique Miam::View module (common displays, ...)
        View* view;
        
        /// \brief Pointer back to the unique edition manager (parent of this manager)
        SceneEditionManager* sceneEditionManager;
        
        /// \brief Pointer for this sub-manager to be able to send orders to the view
        SceneEditionComponent* sceneEditionComponent;
        
        /// \brief The associated Juce canvasComponent
        ///
        /// A normal pointer that has to be properly (at some point) transmitted to the Juce API
        SceneCanvasComponent* canvasComponent;
        
        
        
        /// \brief Areas belonging to the canvas
        /// WHY DO WE NEED THIS $$ AND $$ THE ONES ORDERED FOR DRAWING ?
        std::vector< std::shared_ptr<EditableArea> > areas;
        /// \brief Indicates the drawing order of areas (contents of each std::deque
        /// element), for a given canvas (canvas id is an position of the std::vector)
        ///
        /// Area at index zero is the first drawn (lowest layer). Size of a second-level
        /// deque = size of drawable areas of the corresponding canvas = size of
        /// editable areas of the corresponding canvas
        std::deque< std::shared_ptr<EditableArea> > areasOrderedForDrawing;
        /// \brief The currently selected area (may be a null pointer)
        std::shared_ptr<EditableArea> selectedArea = nullptr;
        
        
        protected :
        SceneCanvasComponent::Id selfId; // USELESS ?
        
        // Current internal running mode
        SceneCanvasMode mode;
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
    public:
        SceneCanvasManager(View* _view, SceneEditionManager* _sceneEditionManager, SceneEditionComponent* _sceneEditionComponent, SceneCanvasComponent::Id _selfId);
        
        
        
        public :
        void CallRepaint() {canvasComponent->repaint();}
        
        
        // ----- Running Mode -----
        
        /// \brief Tries to set the next running mode, then tells the
        /// Miam::SceneEditionManager what happened
        ///
        /// The Miam::SceneEditionManager might then itself change its mode
        void SetMode(SceneCanvasMode);
        /// \brief Gets the current running mode
        SceneCanvasMode GetMode() {return mode;}
        
        
        
        // ------ Setters and Getters ------
        public :
        /// \brief Gets a specific DrawableArea by index
        ///
        /// The first (index 0) is supposed to be drawn first (deepest layer)
        /// Called by the associated canvas component (View module)
        std::shared_ptr<DrawableArea> GetDrawableArea(int _index);
        size_t GetDrawableAreasCount() {return areas.size();}
        
        SceneCanvasComponent::Id GetId() {return selfId;}
        
 
        
        protected :
        void setSelectedArea(std::shared_ptr<EditableArea> _selectedArea);
        
        
        public :
        /// \brief Gets the currently selected area (nullptr if nothing selected)
        std::shared_ptr<EditableArea> GetSelectedArea() {return selectedArea;}
        
        
        // ------ areas managing : Add and Delete ------
        
        // For testing purposes only
        void __AddTestAreas();
    
        
        /// \brief Adds a polygon without creating it before
        void AddEditableArea(std::shared_ptr<EditableArea> newArea, bool selectArea = false);
        /// \brief Deletes the currently selected Miam::EditableArea
        ///
        /// Returns whether something was actually deleted or not
        bool DeleteSelectedArea();
        
        /// \brief Add a default centered grey hexagon
        void AddDefaultArea(uint64_t _nextAreaId);
        
        protected :
        // Deleted all shared_pointers to the area that has this ID
        // So : this should entirely delete the area...
        void deleteAreaByUniqueId(uint64_t uid);
        
        
        // ------ areas managing : Z-Order ------
        public :
        
        void SendSelectedAreaToBack();
        void SendSelectedAreaBackward();
        void SendSelectedAreaForward();
        void SendSelectedAreaToFront();
        
        
        
        
        // ---------- Events from CanvasComponent (from View) ----------
        public :
        void OnCanvasMouseDown(Point<int> clicLocation);
        void OnCanvasMouseDrag(Point<int> mouseLocation);
        void OnCanvasMouseUp();
        
        
        
        
        
    };
    
}



#endif  // SCENECANVAS_H_INCLUDED
