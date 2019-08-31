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
