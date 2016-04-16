/*
  ==============================================================================

    SceneCanvasComponent.cpp
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SceneCanvasComponent.h"

#include "SceneEditionManager.h"

//==============================================================================
SceneCanvasComponent::SceneCanvasComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SceneCanvasComponent::~SceneCanvasComponent()
{
}

void SceneCanvasComponent::CompleteInitialization(SceneEditionManager* _sceneEditionManager)
{
    sceneEditionManager = _sceneEditionManager;
    
    // to refresh all polygons
    resized();
}








void SceneCanvasComponent::paint (Graphics& g)
{
    // Pure black background
    g.fillAll (Colours::black);
    
    // Areas painting
    for (int i=0;i<sceneEditionManager->GetDrawableAreasSize();i++)
        sceneEditionManager->GetDrawableArea(i).Paint(g);
}

void SceneCanvasComponent::resized()
{
    // Actualization of all areas graphical objets, if Presenter is accessible (not at first time)
    if (sceneEditionManager != 0)
    {
        for (int i=0;i<sceneEditionManager->GetDrawableAreasSize();i++)
            sceneEditionManager->GetDrawableArea(i).CanvasResized(getWidth(), getHeight());
    }
}



//

void SceneCanvasComponent::mouseDown(const juce::MouseEvent& event)
{
    sceneEditionManager->OnCanvasMouseDown(event.getMouseDownPosition());
}

void SceneCanvasComponent::mouseDrag(const juce::MouseEvent& event)
{
    sceneEditionManager->OnCanvasMouseDrag(event.getPosition());
}

void SceneCanvasComponent::mouseUp(const juce::MouseEvent& /*event*/)
{
    sceneEditionManager->OnCanvasMouseUp();
}


