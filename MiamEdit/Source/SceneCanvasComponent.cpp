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
SceneCanvasComponent::SceneCanvasComponent(SceneCanvasComponent::Id _id) :
    selfId(_id),
    selectedForEditing(false)
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
    
    // White interior contour 2px line to show when the canvas is active
    if (selectedForEditing)
    {
        g.setColour(Colours::white);
        g.drawRect(1, 1, getWidth()-2, getHeight()-2, 2.0);
    }
    
    // Areas painting
    for (int i=0;i<sceneEditionManager->GetDrawableAreasCount(selfId);i++)
        sceneEditionManager->GetDrawableArea(selfId, i).Paint(g);
}

void SceneCanvasComponent::resized()
{
    // Actualization of all areas graphical objets, if Presenter is accessible (not at first time)
    if (sceneEditionManager != 0)
    {
        for (int i=0;i<sceneEditionManager->GetDrawableAreasCount(selfId);i++)
            sceneEditionManager->GetDrawableArea(selfId, i).CanvasResized(this);
    }
}



//

void SceneCanvasComponent::mouseDown(const juce::MouseEvent& event)
{
    sceneEditionManager->OnCanvasMouseDown(selfId, event.getMouseDownPosition());
}

void SceneCanvasComponent::mouseDrag(const juce::MouseEvent& event)
{
    sceneEditionManager->OnCanvasMouseDrag(selfId, event.getPosition());
}

void SceneCanvasComponent::mouseUp(const juce::MouseEvent& /*event*/)
{
    sceneEditionManager->OnCanvasMouseUp(selfId);
}


