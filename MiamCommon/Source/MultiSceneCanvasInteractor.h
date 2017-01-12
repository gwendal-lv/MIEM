/*
  ==============================================================================

    MultiSceneCanvasInteractor.h
    Created: 8 Oct 2016 1:02:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MULTISCENECANVASINTERACTOR_H_INCLUDED
#define MULTISCENECANVASINTERACTOR_H_INCLUDED

#include <memory>
#include <vector>

#include <deque>


#include "SceneCanvasComponent.h"

#include "EditableScene.h"

#include "CanvasManagerMode.h"



#include "EditableArea.h"
#include "EditablePolygon.h"


class MultiSceneCanvasComponent;


namespace Miam {
    // pre-declaration for pointers
    class IGraphicSessionManager;
    class EditableScene;
    
    
    /// \brief Manages the interaction with some Miam::EditableScene
    ///
    ///
    class MultiSceneCanvasInteractor
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        
        // Current internal running mode
        CanvasManagerMode mode;
        
        
        /// \brief The associated Juce canvas component
        ///
        /// The pointer was actually extracted from a juce::ScopedPointer (do not
        /// delete it from inside this class)
        MultiSceneCanvasComponent* canvasComponent;
        
        
        
        // The scenes
        std::vector< std::shared_ptr<EditableScene> > scenes;
        
        /// \brief The currently selected scene
        ///
        /// There should always be at least one scene alive, to prevent undefined
        /// behavior
        std::shared_ptr<EditableScene> selectedScene = nullptr;
        
        
        /// \brief Pointer back to the unique edition manager (parent of this manager)
        IGraphicSessionManager* graphicSessionManager = 0;
        
        
        
        protected :
        SceneCanvasComponent::Id selfId; ///< To be transformed to a name...
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
    public:
        MultiSceneCanvasInteractor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId);
        
        virtual ~MultiSceneCanvasInteractor();
        
        
        
        public :
        void CallRepaint();
        
        
        
        
        // ----- Running Mode -----
        
        /// \brief Default acceptation of mode change (to be overriden)
        ///
        /// Tries to set the next running mode, then tells the
        /// Miam::graphicSessionManager what happened
        /// The Miam::IGraphicSessionManager might then itself change its mode
        virtual void SetMode(CanvasManagerMode _mode);
        /// \brief Gets the current running mode
        virtual CanvasManagerMode GetMode() {return mode;}
        
        
        
        // ------ Setters and Getters ------
        public :
        /// \brief Gets a specific DrawableArea by index
        ///
        /// The first (index 0) is supposed to be drawn first (deepest layer)
        /// Called by the associated canvas component (View module)
        std::shared_ptr<IDrawableArea> GetDrawableObject(int index_);
        /// \brief Max argument for Miam::MultiSceneCanvasInteractor::GetDrawableArea
        size_t GetDrawableObjectsCount();
        
        SceneCanvasComponent::Id GetId() {return selfId;}
        
        
        // Scenes
        virtual size_t GetScenesCount() {return scenes.size();}
        virtual std::shared_ptr<EditableScene> GetScene(size_t i) {return scenes[i];}
        
        /// \brief Mostly performs multiple downcasts from Miam::EditableScene
        /// to Miam::InteractiveScene
        virtual std::vector<std::shared_ptr<InteractiveScene>> GetInteractiveScenes();
        
        virtual std::shared_ptr<EditableScene> GetSelectedScene() {return selectedScene;}
        virtual int GetSelectedSceneId();
        
        
        
        
        
        // - - - - - Selection management (scenes only) - - - - -
        
        virtual void SelectScene(int id);
        
  
        
        // ------ Scenes managing : Add and Delete ------
        virtual void AddScene(std::string name);
        virtual void AddScene(std::shared_ptr<EditableScene> newScene);
        /// Returns wether the selected scene has been deleted or not (if it
        /// was the last one)
        virtual bool DeleteScene();
        
        
        
        // ------ areas managing : Add and Delete ------
        
        // For testing purposes only
        // ADDS AREAS ON ALL SCENES
        void __AddTestAreas();
    
       

        
        
        
        // ---------- Events from CanvasComponent (from View) ----------
        public :
        void OnCanvasMouseDown(const MouseEvent& mouseE);
        void OnCanvasMouseDrag(const MouseEvent& mouseE);
        void OnCanvasMouseUp(const MouseEvent& mouseE);
        
		//public :
			//virtual void OnSurfaceChanged(double newSurface) = 0;
        
        
        
    };
    
}



#endif  // MULTISCENECANVASINTERACTOR_H_INCLUDED
