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

#include <mutex>


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
        
        
        /// \brief Pointer back to the unique edition manager (parent of this manager)
        IGraphicSessionManager* graphicSessionManager = 0;

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
        
        
        
        // - - - - - Thread-safe attributes (OpenGL async drawing) - - - - -
        
        std::mutex asyncDrawableObjectsMutex;
        // Synchronized with the UNIQUE selected scene's drawable objects
        std::vector<std::shared_ptr<IDrawableArea>> asyncDrawableObjects;
        
        /// \brief Maps an original area to its asynchronously drawn copy. Accessed
        /// from the main "presenter (juce) message thread" only.
        ///
        /// Not always protected by mutex locks ! As its elements are shared pointers,
        /// which allows MT-safe access
        std::map<std::shared_ptr<IDrawableArea>, std::vector<std::shared_ptr<IDrawableArea>>::iterator> originalToAsyncObject;
        
        
        
        protected :
        SceneCanvasComponent::Id selfId; ///< To be transformed to a name...
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
    public:
        MultiSceneCanvasInteractor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId);
        
        virtual ~MultiSceneCanvasInteractor();
        
        
        
        public :
        /// \biref Updates data, refreshes then actually paints what's necessary
        /// (and asked to repaint).
        ///
        /// May only repaint the canvas (and not the Miam::MultiSceneCanvasComponent
        /// with its additionnal UI on side : buttons, etc...)
        void CallRepaint(bool repaintSideUiElements = true);
        
        
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
        
        /*
        /// \brief Gets a specific DrawableArea by index
        ///
        /// The first (index 0) is supposed to be drawn first (deepest layer)
        /// Called by the associated canvas component (View module)
        std::shared_ptr<IDrawableArea> GetDrawableObject(size_t index_);
        /// \brief Max argument for Miam::MultiSceneCanvasInteractor::GetDrawableArea
        size_t GetDrawableObjectsCount();
        */
        
        SceneCanvasComponent::Id GetId() {return selfId;}
        
        
        // Scenes
        virtual size_t GetScenesCount() {return scenes.size();}
        virtual std::shared_ptr<EditableScene> GetScene(size_t i) {return scenes[i];}
        
        /// \brief Mostly performs multiple downcasts from Miam::EditableScene
        /// to Miam::InteractiveScene
        virtual std::vector<std::shared_ptr<InteractiveScene>> GetInteractiveScenes();
        
        // for this remain master of the modifications applied to it
        //virtual std::shared_ptr<EditableScene> GetSelectedScene() {return selectedScene;}
        
        // à DÉGAGER SI LA MAP DE EDITABLEAREA VERS SON INDICE MARCHE BIEN
        virtual int GetSelectedSceneId();
        
        
        
        
        // - - - - - Internal events management - - - - -
        
        protected :
        /// \brief
        ///
        /// Copy-constructor of the event called : else, there would be cast
        /// issues
        void handleAndSendEventSync(std::shared_ptr<GraphicEvent> graphicE);
        // for optimization purposes
        void handleAndSendAreaEventSync(std::shared_ptr<AreaEvent> areaE);
        
        
        
        // - - - - - Thread-safe methods (for OpenGL async drawing) - - - - -
        
        protected : // internal routines
        void recreateAllAsyncDrawableObjects(bool recreateMap = true);
        void addAsyncDrawableObject(int insertionIdInScene, std::shared_ptr<IDrawableArea> originalAreaToAdd);
        void updateAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToUpdate);
        void deleteAsyncDrawableObject(int idInScene, std::shared_ptr<IDrawableArea> originalAreaToDelete);

        public : // external interfacing
        // 3 following functions, to be used very carefully from the OpenGL renderer !
        void LockAsyncDrawableObjects() { asyncDrawableObjectsMutex.lock(); }
        std::vector<std::shared_ptr<IDrawableArea>>& GetAsyncDrawableObjects()
        { return asyncDrawableObjects; }
        void UnlockAsyncDrawableObjects() { asyncDrawableObjectsMutex.unlock(); }
        
        
        
        
        // - - - - - Selection management (scenes only) - - - - -
        public :
        virtual void SelectScene(int id);
        
  
        
        // ------ Scenes managing : Add and Delete ------
        virtual void AddScene(std::string name);
        virtual void AddScene(std::shared_ptr<EditableScene> newScene);
        /// Returns wether the selected scene has been deleted or not (if it
        /// was the last one).
        virtual bool DeleteScene();
        
        
        
        // ------ areas managing : Add and Delete ------
        public :
        // For testing purposes only
        // ADDS AREAS ON ALL SCENES
        void __AddTestAreas();
        
        protected :
        // does what it says, and sends an event also
        void addAreaToScene(std::shared_ptr<EditableScene> scene_, std::shared_ptr<IInteractiveArea> area_);

        // ---------- Events from CanvasComponent (from View) ----------
        public :
        void OnCanvasMouseDown(const MouseEvent& mouseE);
        void OnCanvasMouseDrag(const MouseEvent& mouseE);
        void OnCanvasMouseUp(const MouseEvent& mouseE);
        void OnResized();
        
    };
    
}



#endif  // MULTISCENECANVASINTERACTOR_H_INCLUDED
