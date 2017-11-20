/*
  ==============================================================================

    SceneCanvasComponent.h
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENECANVASCOMPONENT_H_INCLUDED
#define SCENECANVASCOMPONENT_H_INCLUDED

#include <memory>

#include "JuceHeader.h"

#include "DrawableArea.h"

#include "FrequencyMeasurer.h"



using namespace Miam;

namespace Miam {
    
    class MultiSceneCanvasInteractor;
}

//==============================================================================
/// \brief Initially empty component dedicated to the drawing of several Miam::DrawableArea
///
/// This component does not have any children UI controls built within the Projucer.
class SceneCanvasComponent    : public OpenGLRenderer,
                                public Component

{
public:
    
    
    // = = = = = = = = = = ENUMS = = = = = = = = = =
    
    /// \brief IDs for describing a particular canvas, or canvases in general
    ///
    enum Id : int { // enum *class* brings cast issues... (wait for C++14 ?)
        None = -1, ///< No canvas currently selected (useless now with smart ptrs)
        
        Canvas1 = 0, ///< First canvas...
        Canvas2, ///< Second canvas...
        CanvasesCount ///< Automatically contains the amount of accessible canvases
    };
    
    
    
    
    
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    
    protected :
    /// \brief Link back to parent module. Thread-safe access to member functions
    /// (guaranteed by std::shared_ptr)
    std::weak_ptr<MultiSceneCanvasInteractor> canvasManager;
    
    bool selectedForEditing;
    
    
    // - - - - - OpenGL - - - - -
    OpenGLContext openGlContext;
    const int swapInterval = 1; // synced on vertical frequency
    bool isSwapSynced;
    
    // Les pointeurs sur les 2è copies des objets du canvas interactor
    // (pour savoir lesquels avaient changé) -> ATTENTION tout l'algorithme thread-safe
    // ici fonctionne tant que le CanvasInteractor construit bien un nouvel objet à chaque fois
    // que l'objet de base est modifié.
    // On stocke ça dans un simple vecteur (alors que les objets de la seconde
    // copie sont dans une liste)
    std::vector<std::shared_ptr<IDrawableArea>> canvasAreasPointersCopies;
    // La 3è copie des objets (celle pour faire le rendu à tout moment),
    // qui sera éventuellement actualisée lorsque nécessaire
    std::vector<std::shared_ptr<IDrawableArea>> duplicatedAreas;
    
    // - - - - - Time measures - - - - -
    const double desiredFrequency_Hz = 60.0; // actual freq will actually be greater
    const double desiredPeriod_ms = 1000.0/desiredFrequency_Hz;
    FrequencyMeasurer displayFrequencyMeasurer;
    
    protected :
    
    
    
    
    // = = = = = = = = = = METHODS = = = = = = = = = =
    public :
    
    SceneCanvasComponent();
    ~SceneCanvasComponent();
    
	/// \brief Also called from Miam::View::CompleteInitialization
    void CompleteInitialization(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager);

    // - - - - - - - - Juce usual paint/resized component methods - - - - - - - - -
    void resized() override;
    
    // - - - - - - - - OpenGL specific - - - - - - - - -
    virtual void newOpenGLContextCreated() override;
    virtual void renderOpenGL() override; // ! in background-thread !
    virtual void openGLContextClosing() override;
    
    // - - - - - - - - Actual painting codes - - - - - - - - -
    protected :
    
    // - - - - - - - - Juce events - - - - - - - - -
    public :
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    
    // Getters and Setters
    float GetRatio() {return ((float)getWidth()) / ((float)getHeight()) ; }
    void SetIsSelectedForEditing(bool isSelected);
    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneCanvasComponent)
};


#endif  // SCENECANVASCOMPONENT_H_INCLUDED
