/*
  ==============================================================================

    MultiSceneCanvasEditor.h
    Created: 20 Nov 2016 11:06:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
