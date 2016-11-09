/*
  ==============================================================================

    SceneCanvasComponent.h
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENECANVASCOMPONENT_H_INCLUDED
#define SCENECANVASCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "DrawableArea.h"


namespace Miam
{
    class SceneEditionManager;
    class SceneCanvasManager;

    
    
}
using namespace Miam;


//==============================================================================
/// \brief Initially empty component dedicated to the drawing of several Miam::DrawableArea
///
/// This component does not have any children UI controls built within the Projucer.
class SceneCanvasComponent    : public Component
{
public:
    
    
    
    // = = = = = = = = = = ENUMS = = = = = = = = = =
    
    /// \brief IDs for describing a particular canvas, or canvases in general
    ///
    enum Id : int { // enum *class* brings cast issues... (wait for C++14 ?)
        None = -1, ///< No canvas currently selected
        
        MainScene = 0, ///< The main canvas, that can display several different scenes a spatialization performance
        FixedScene, ///< Displays a fixed scene, that never changes during a spat performance
        SceneCanvasesCount ///< Automatically contains the amount of accessible canvases
    };
    
    
    
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    
    protected :
    // links back to parent modules
    SceneEditionManager* sceneEditionManager = 0; // default c++ null pointer
    SceneCanvasManager* canvasManager = 0;
    
    // Self ID
    SceneCanvasComponent::Id selfId;
    
    bool selectedForEditing;
    
    std::vector< std::weak_ptr<DrawableArea> > areas;
    
    
    
    // = = = = = = = = = = METHODS = = = = = = = = = =
    public :
    
    SceneCanvasComponent(SceneCanvasComponent::Id _id);
    ~SceneCanvasComponent();
    
	/// \brief Also called from Miam::View::CompleteInitialization
    void CompleteInitialization(SceneEditionManager* _sceneEditionManager);
    void CompleteInitialization(SceneCanvasManager* _canvasManager);

    void paint (Graphics&);
    void resized();
  
    // Juce Events
    void mouseDown(const juce::MouseEvent &event);
    void mouseDrag(const juce::MouseEvent &event);
    void mouseUp(const juce::MouseEvent &event);
    
    // Getters and Setters
    float GetRatio() {return ((float)getWidth()) / ((float)getHeight()) ; }
    void SetIsSelectedForEditing(bool isSelected) {selectedForEditing = isSelected;}
    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneCanvasComponent)
};


#endif  // SCENECANVASCOMPONENT_H_INCLUDED
