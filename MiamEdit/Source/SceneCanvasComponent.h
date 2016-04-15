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
}
using namespace Miam;


//==============================================================================
/*
*/
class SceneCanvasComponent    : public Component
{
public:
    SceneCanvasComponent();
    ~SceneCanvasComponent();
    
    void CompleteInitialization(SceneEditionManager* _sceneEditionManager);

    void paint (Graphics&);
    void resized();
  
    // Juce Events
    void mouseDown(const juce::MouseEvent &event);
    void mouseDrag(const juce::MouseEvent &event);
    void mouseUp(const juce::MouseEvent &event);
    
    // Getters and Setters
    float GetRatio() {return ((float)getWidth())/((float)getHeight());}
    
    
    // Attributes
    private :
    SceneEditionManager* sceneEditionManager = 0; // default c++ null pointer
    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneCanvasComponent)
};


#endif  // SCENECANVASCOMPONENT_H_INCLUDED
