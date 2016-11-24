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

#include "JuceHeader.h"


// Pre-declarations for pointers
class SceneCanvasComponent;

namespace Miam
{
    // Pre-declarations for pointers
    class Exciter;
    class IDrawableArea;
    class IInteractiveArea;
    class MultiSceneCanvasInteractor;
    
    
    
    /// \brief A graphical scene made of areas and exciters, to be drawn on a SceneCanvasComponent
    ///
    /// This kind of scene can be loaded (from a file for example) and destroyed,
    /// but is not modifiable.
    class InteractiveScene
    {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        protected :
        
        // links back to parent modules
        MultiSceneCanvasInteractor* canvasManager = 0;
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
        
        
        
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        size_t GetAreasCount();
        std::shared_ptr<IDrawableArea> GetDrawableArea(size_t i);
        std::shared_ptr<IInteractiveArea> GetInteractiveArea(size_t i);
        std::string GetName() {return name;}
        virtual void SetName(std::string _name);
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction and Destruction (and helpers) - - - - -
        InteractiveScene(MultiSceneCanvasInteractor* _canvasManager, SceneCanvasComponent* _canvasComponent);
        virtual ~InteractiveScene();
        
        
        
        
        // - - - - - Areas Managing : Add and Delete - - - - -
        
        
        /// \brief Adds an area without creating it before
        virtual void AddArea(std::shared_ptr<IInteractiveArea> newArea);
        
        
        
        
        // - - - - - Canvas (mouse) events managing - - - - -
        
        /// \brief Defines the behavior of the scene when an input occurs
        ///
        /// May send an error message, or an explanation if nothing happened
        /// Empty return string means that everything went as expected
        virtual std::string OnCanvasMouseDown(Point<int>& clicLocation) = 0;
        /// \brief Defines the behavior of the scene when an input occurs
        ///
        /// Returns wether something is being dragged or not
        virtual bool OnCanvasMouseDrag(Point<int>& mouseLocation) = 0;
        /// \brief Defines the behavior of the scene when an input occurs
        ///
        /// Return wether something has stopped being dragged or not
        virtual bool OnCanvasMouseUp() = 0;
        
        
    };
    
    
    
}


#endif  // INTERACTIVESCENE_H_INCLUDED
