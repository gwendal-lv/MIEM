/*
  ==============================================================================

    SceneCanvasComponent.cpp
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SceneCanvasComponent.h"

#include "IMultiSceneCanvasManager.h"

//==============================================================================
SceneCanvasComponent::SceneCanvasComponent() :
    selectedForEditing(false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SceneCanvasComponent::~SceneCanvasComponent()
{
}

void SceneCanvasComponent::CompleteInitialization(IMultiSceneCanvasManager* _canvasManager)
{
    canvasManager = _canvasManager;
}








void SceneCanvasComponent::paint (Graphics& g)
{
    // Pure black background
    g.fillAll (Colours::black);
    
    // White interior contour 2px line to show when the canvas is active
    if (selectedForEditing)
    {
        g.setColour(Colours::white);
        g.drawRect(1, 1, getWidth()-2, getHeight()-2, 2);
    }
    
    // Areas painting
    for (int i=0;i<canvasManager->GetDrawableAreasCount();i++)
        canvasManager->GetDrawableArea(i)->Paint(g);
}

void SceneCanvasComponent::resized()
{
    // Actualization of all areas graphical objets, if Presenter is accessible (not at first time)
    if (canvasManager != 0)
    {
        for (int i=0;i<canvasManager->GetDrawableAreasCount();i++)
            canvasManager->GetDrawableArea(i)->CanvasResized(this);
    }
}



// FOR NOW : transmission of mouse/touch events for only ONE input source
// Because the scene edition manager is built to react to the mouse only...

void SceneCanvasComponent::mouseDown(const juce::MouseEvent& event)
{
	if (event.source.getIndex() == 0)
		canvasManager->OnCanvasMouseDown(event.getMouseDownPosition());
}

void SceneCanvasComponent::mouseDrag(const juce::MouseEvent& event)
{
	if (event.source.getIndex() == 0)
		canvasManager->OnCanvasMouseDrag(event.getPosition());
}

void SceneCanvasComponent::mouseUp(const juce::MouseEvent& event)
{
	if (event.source.getIndex() == 0)
		canvasManager->OnCanvasMouseUp();
}


