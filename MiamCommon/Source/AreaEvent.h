/*
  ==============================================================================

    AreaEvent.h
    Created: 31 Dec 2016 10:55:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef AREAEVENT_H_INCLUDED
#define AREAEVENT_H_INCLUDED


#include <memory>


#include "GraphicEvent.h"

#include "IDrawableArea.h"




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
        AreaEventType GetType() {return eventType;}
        std::shared_ptr<IDrawableArea> GetConcernedArea() {return concernedArea;}
        std::shared_ptr<InteractiveScene> GetConcernedScene() {return concernedScene;}
        int GetAreaIdInScene() {return areaIdInScene;}
        
        /// \brief To call after the construction, in case the event is created by
        /// the scene itself (then, the scene does not know the shared_ptr to itself)
        void SetConcernedScene(std::shared_ptr<InteractiveScene> concernedScene_)
        { concernedScene = concernedScene_; }
        
        
        
        /// \brief Default constructor : indicates that nothing happens
        AreaEvent();
        
        /// \brief Contructs an event about an area that may be linked to a scene
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
