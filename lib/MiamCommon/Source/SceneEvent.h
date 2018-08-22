/*
  ==============================================================================

    SceneEvent.h
    Created: 10 Jan 2017 4:11:07pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENEEVENT_H_INCLUDED
#define SCENEEVENT_H_INCLUDED



#include <memory>


#include "GraphicEvent.h"

#include "EditableScene.h"


namespace Miam
{
    
    
    enum class SceneEventType {
        
        /// None,
        
        NothingHappened, // not used ?
        
        
        Added, ///< Scene has just been added to the canvas
        Deleted, ///< Scene has just been deleted to the canvas
        
        
        SceneChanged,
        
    };
    
    
    
    class SceneEvent : public GraphicEvent
    {
        protected :
		SceneEventType eventType;// = SceneEventType::SceneChanged;
        std::shared_ptr<MultiSceneCanvasInteractor> canvasManager = 0;
        std::shared_ptr<EditableScene> oldScene = nullptr;
        std::shared_ptr<EditableScene> newScene = nullptr;
        
        public :
        SceneEventType GetType() {return eventType;}
        
        std::shared_ptr<MultiSceneCanvasInteractor> GetParentCanvas() {return canvasManager;}
        std::shared_ptr<EditableScene> GetOldScene() {return oldScene;}
        std::shared_ptr<EditableScene> GetNewScene() {return newScene;}
        
        // Construction and Destruction
		SceneEvent(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, std::shared_ptr<EditableScene> Scene_,SceneEventType eventType_);
        SceneEvent(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, std::shared_ptr<EditableScene> oldScene_, std::shared_ptr<EditableScene> newScene_, SceneEventType eventType_);
        virtual ~SceneEvent() {}
        
    };
}


#endif  // SCENEEVENT_H_INCLUDED
