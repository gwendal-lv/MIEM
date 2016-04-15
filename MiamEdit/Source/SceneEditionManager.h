/*
  ==============================================================================

    SceneEditionManager.h
    Created: 28 Mar 2016 5:27:18pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENEEDITIONMANAGER_H_INCLUDED
#define SCENEEDITIONMANAGER_H_INCLUDED


#include <vector>
#include <deque>
#include <map>

#include "EditablePolygon.h"

#include "Juceheader.h" // for : Point<float>, uint64

#include "SceneEditionComponent.h"
#include "SceneEditionMode.h"


namespace Miam {
    
    // Simple declarations
    class View;
    
    
    
    class SceneEditionManager {
        
        
        // Graphical objects belong to the Presenter module, not to the View
        private :
        
        // internal states
        SceneEditionMode mode;
        
        // links back to the View module
        View* view;
        SceneEditionComponent* sceneEditionComponent;
        
        // Graphic objects instances
        std::vector<EditablePolygon> polygons;
        
        // Graphic objects management
        int64_t nextAreaId; // unique Id of the created (or pasted, or loaded...) area
        // maps a unique DrawableArea Id to its plygon vector index (for links with Spkrs Configs)
        std::map< int64_t, int > polygonsVectorIndexes;
        // TODO : a map for each kind of area !! if we can't find the Id, it's wasn't a polygon (maybe an ellipse, or something else...)
        
        // Display & editing attributes
        std::deque<int64_t> areasOrder; // area at index zero is the first drawn (lowest layer). Size = size of drawable areas = size of editable areas
        int64_t selectedAreaUniqueId = -1; // -1 means no area selected
        int64_t areaToCopy = -1; // vector index of the polygon to copy (idem considered value -1)
        
        
        public :
        // Construction
        SceneEditionManager(View* _view);
        
        // Getters and Setters
        protected :
        EditablePolygon& GetEditablePolygon(int64_t uniqueId);
        // Generic objects
        //EditableArea& GetEditableArea(int64_t uniqueId);
        public :
        int GetDrawableAreasSize();
        DrawableArea& GetDrawableArea(int position);
        
        
        // Utilities
        protected :
        void addEditableArea(EditablePolygon& newPolygon, bool selectArea = false);
        void deleteEditableArea(int64_t uniqueId);
        void setSelectedAreaUniqueId(int64_t newIndex);
        void setMode(SceneEditionMode newMode);
        
        
        // Events from View
        public :
        
        void OnCanvasMouseDown(Point<int> clicLocation);
        void OnCanvasMouseDrag(Point<int> mouseLocation);
        void OnCanvasMouseUp();
        
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
        
    };
    
    
}


#endif  // SCENEEDITIONMANAGER_H_INCLUDED
