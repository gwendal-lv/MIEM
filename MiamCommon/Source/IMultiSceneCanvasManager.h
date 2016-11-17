/*
  ==============================================================================

    IMultiSceneCanvasManager.h
    Created: 17 Nov 2016 3:34:23pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IMULTISCENECANVASMANAGER_H_INCLUDED
#define IMULTISCENECANVASMANAGER_H_INCLUDED

#include <memory>

#include "JuceHeader.h"
#include "DrawableArea.h"



namespace Miam {

    /// \brief Generic interface for a manager that can handle MultiSceneCanvasComponent
    ///
    ///
    class IMultiSceneCanvasManager
    {
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        virtual ~IMultiSceneCanvasManager() {}
        
        
        
        
        // ------ Setters and Getters ------
        public :
        /// \brief Gets a specific DrawableArea by index
        ///
        /// The first (index 0) is supposed to be drawn first (deepest layer)
        /// Called by the associated canvas component (View module)
        virtual std::shared_ptr<DrawableArea> GetDrawableArea(int _index) = 0;
        virtual size_t GetDrawableAreasCount() = 0;
        
        
        
        // ---------- Events from a canvas component (from View) ----------
        public :
        virtual void OnCanvasMouseDown(Point<int> clicLocation) = 0;
        virtual void OnCanvasMouseDrag(Point<int> mouseLocation) = 0;
        virtual void OnCanvasMouseUp() = 0;
        
        
        
        
        
    };

}



#endif  // IMULTISCENECANVASMANAGER_H_INCLUDED
