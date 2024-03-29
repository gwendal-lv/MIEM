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

#ifndef AREAEVENT_H_INCLUDED
#define AREAEVENT_H_INCLUDED


#include <memory>


#include "GraphicEvent.h"

#include "DrawableArea.h"




namespace Miam
{
    // Pre-declarations for pointer members
    class InteractiveScene;
    
    
    enum class AreaEventType {
    
        /// None,
        
        NothingHappened,
        
        
        // - - - - - Events on areas addition / deletion - - - - -
        
        Added, ///< Area has just been added to the scene. The "index in scene" must be provided for such events !!!
        Deleted, ///< Area has just been deleted to the scene. The "index in scene" must NOT be provided.
        
        

		// - - - - - Events that happen at the beginning of a material input - - - - -
        
		PointDragBegins, ///< The last input was close enough to a point to begin a 2D transformation
		AnotherMonoTouchPointDragAlreadyBegun, ///< voilà

        
        
		//  - - - - - Events (flows of events) happening during
        //                              a geometrical modification - - - - -
        
        ShapeChanged, ///< editing contour points
        Translation, ///< Pure translation
        //Rotation,
        //Scaling, ///< "surface changed" case
        RotScale, ///< the whole area is being manipulated (but not translated)
        //TraRotScale, ///< 2D modification from 2 touch point inputs
        
		ParentChanged,

		//  - - - - - Events that happen at the end of a material input - - - - -
        
		PointDragStops, ///< The input was release and is not linked to the area anymore



		//  - - - - - Event related to selection state - - - - -
        
		Selected, ///< Area has just been selected
		Unselected, ///< Area has just been selected

        
        
        
        //  - - - - - Event related to the appearance - - - - -
        ColorChanged, ///< The contour and/or background color of the area has just changed
        
        
        
        //  - - - - - Event related to interactions quantification - - - - -
        ExcitementAmountChanged, ///< The total amount of excitation on this area has changed
        
        
    };
    
    
    
    class AreaEvent : public GraphicEvent
    {
        protected :
        AreaEventType eventType = AreaEventType::NothingHappened;
        
        std::shared_ptr<IDrawableArea> concernedArea;
        
        /// \brief If exists, the scene to which the area belongs.
        ///
        /// nullptr if not related to a scene in particular
        std::shared_ptr<InteractiveScene> concernedScene;
        
        /// \brief If applicable, the id of the concerned drawable object, within
        /// the scene it belongs to. It is the "drawable id", an id that is valid for
        /// any exciter or "classic" area, be careful !
        ///
        /// Allows faster "back-search" of the area within the scene
        int areaIdInScene;
        
        
        
        public :
        AreaEventType GetType() const {return eventType;}
        std::shared_ptr<IDrawableArea> GetConcernedArea() const {return concernedArea;}
        std::shared_ptr<InteractiveScene> GetConcernedScene() const {return concernedScene;}
        int GetAreaIdInScene() const {return areaIdInScene;}
        
        /// \brief To call after the construction, in case the event is created by
        /// the scene itself (then, the scene does not know the shared_ptr to itself)
        void SetConcernedScene(std::shared_ptr<InteractiveScene> concernedScene_)
        { concernedScene = concernedScene_; }
        
        
        
        /// \brief Default constructor : indicates that nothing happens
        AreaEvent();
        
        /// \brief Contructs an event about an area that may be linked to a scene
        ///
        /// Optimisation "passage des arguments par référence" difficile : problèmes
        /// avec les make_shared et le polymorphisme des références dans le constructeur
        /// (plusieurs manières de caster implicitement vers IDrawable à cause de
        /// l'héritage en diamant.... résoudre ça engendrerait beaucoup d'autres soucis...)
        ///
        /// If the "concerned scene" is nullptr, the area is linked to... something
        /// else ?
        AreaEvent(std::shared_ptr<IDrawableArea> concernedArea_,
                  AreaEventType eventType_,
                  int areaIdInScene_ = -1,
                  std::shared_ptr<InteractiveScene> concernedScene_ = nullptr);
        
        /// \brief Constructs an Area Event that contains all necessary info but the index
        /// of the area within its scene.
        ///
        /// Index missing, possibly because : the index often needs a reverse search to be found.
        /// This reverse search is avoided as often as possible.
        AreaEvent(std::shared_ptr<IDrawableArea> concernedArea_,
                  AreaEventType eventType_,
                  std::shared_ptr<InteractiveScene> concernedScene_);
        
        // Copy-constructor
        AreaEvent( AreaEvent *e );
        
        virtual ~AreaEvent() {}
        
    };
}


#endif  // AREAEVENT_H_INCLUDED
