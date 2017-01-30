/*
  ==============================================================================

    InteractiveScene.h
    Created: 17 Nov 2016 4:53:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef INTERACTIVESCENE_H_INCLUDED
#define INTERACTIVESCENE_H_INCLUDED



#include <memory>
#include <vector>
#include <string>
#include <map>

#include "JuceHeader.h"

#include "AreaEvent.h"

#include "IEditableArea.h"

// Pre-declarations for pointers
class SceneCanvasComponent;

namespace Miam
{
    // Pre-declarations for pointers
    class Exciter;
    class IDrawableArea;
    class IInteractiveArea;
    class MultiSceneCanvasInteractor;
    
    
    
    /// \brief A graphical scene made of areas and exciters, to be drawn on
    /// SceneCanvasComponent
    ///
    /// This kind of scene can be loaded (from a file for example) and destroyed,
    /// but is not fully editable. See Miam::EditableArea for editing features.
    /// However, the exciters (if enabled) can be moved with mouse/touch/pen events
    class InteractiveScene
    {
        
        // ...Enums....
        public :
        /// \brief For Miam Spat research purposes
        enum class ExcitersBehaviorType {
            
            NoExciters,
            
            AppearOnTouch,
            
            ManualAddAndDelete,
            
            ManualAddAndDelete_AutoDisappear,
            
        };
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        protected :
        
        // links back to parent modules
        std::weak_ptr<MultiSceneCanvasInteractor> canvasManager;
        /// \brief The associated Juce (mono-scene) canvas component
        ///
        /// A basic pointer that has to be properly (at some point) transmitted to the Juce API
        SceneCanvasComponent* canvasComponent = 0;
        
        
        // name
        std::string name;
        
        /// \brief The areas to be drawn
        std::vector< std::shared_ptr<IInteractiveArea> > areas;
        
        std::vector< std::shared_ptr<Exciter> > initialExciters;
        std::vector< std::shared_ptr<Exciter> > currentExciters;
        ExcitersBehaviorType excitersBehavior;
        
        
        /// \brief Links (Locks) a multi-touch souce ID to an EditableArea
        std::map<int, std::shared_ptr<IEditableArea>> touchSourceToEditableArea;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        // areas + exciters
        size_t GetDrawableObjectsCount();
        std::shared_ptr<IDrawableArea> GetDrawableObject(size_t i);
        
        // true interactive areas only
        size_t GetInteractiveAreasCount();
        std::shared_ptr<IInteractiveArea> GetInteractiveArea(size_t i);
        
        
        /* useless normally...
        size_t GetExcitersCount() {return currentExciters.size(); }
        std::shared_ptr<Exciter> GetExciter(size_t i) {return currentExciters[i];}
         */
        
        std::string GetName() {return name;}
        virtual void SetName(std::string _name);
        
        
        
    
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction and Destruction (and helpers) - - - - -
        InteractiveScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_, ExcitersBehaviorType excitersBehavior_ = ExcitersBehaviorType::ManualAddAndDelete);
        virtual ~InteractiveScene();
        
        
        
        
        // - - - - - Areas Managing : Add and Delete - - - - -
        
        
        /// \brief Adds an area without creating it before
        virtual std::shared_ptr<AreaEvent> AddArea(std::shared_ptr<IInteractiveArea> newArea);
        
        
        
        // - - - - - Selection events managing (orders from parent manager) - - - - -

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// !!!!!!!!!!!!!!!! renvoyer des évènements sur ce qu'il s'est passé !!!!!!!!!!
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        virtual void OnSelection();
        /// \ brief Behavior on unselection commanded from parent (area transformations are stopped, ...). Must be called by classes that inherit from this.
        virtual void OnUnselection();
        
        
        // - - - - - Canvas (mouse) events managing - - - - -
        // CES FONCTIONS IMPLÉMENTERONT LES DÉPLACEMENTS DES EXCITATEURS
        // Elles pourront être appelées depuis la classe file EditableScene si
        // nécessaire, si les excitateurs sont effectivement utilisés
        //
        // Par défaut elles sont overridées par la classe fille
        
        /// \brief Defines the behavior of the scene when an input occurs. Possibly begins an exciter's movement
        ///
        /// Within the event,
        /// may send an error message, or an explanation if nothing happened
        /// Empty return string means that everything went as expected
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE);
        /// \brief Defines the behavior of the scene when an input occurs. Possibly moves an exciter if enabled
        ///
        /// Does not compute a new spatialization state : a parent will ask the
        /// scene to do so at the right time.
        ///
        /// \returns An event that describes what happened
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE);
        /// \brief Defines the behavior of the scene when an input occurs. Possibly ends an exciter move if enabled
        ///
        /// \returns An event that describes what happened
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE);

    };
    
    
    
}


#endif  // INTERACTIVESCENE_H_INCLUDED
