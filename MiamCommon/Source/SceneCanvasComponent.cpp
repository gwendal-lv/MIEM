/*
  ==============================================================================

    SceneCanvasComponent.cpp
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SceneCanvasComponent.h"

#include "MultiSceneCanvasInteractor.h"

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

void SceneCanvasComponent::CompleteInitialization(MultiSceneCanvasInteractor* _canvasManager)
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
    
    // Areas painting (including exciters if existing)
    for (size_t i=0;i<canvasManager->GetDrawableObjectsCount();i++)
        canvasManager->GetDrawableObject(i)->Paint(g);
    
    
}

void SceneCanvasComponent::resized()
{
    // Actualization of all areas graphical objets, if Presenter is accessible (not at first time)
    if (canvasManager != 0)
    {
        // We update ALL areas NOW, to avoid a consequent amount of calculus on
        // scene change (which should happen as fast as possible)
        for (size_t i=0;i<canvasManager->GetScenesCount();i++)
        {
            for (size_t j=0 ; j<canvasManager->GetScene(i)->GetDrawableObjectsCount() ; j++)
                canvasManager->GetScene(i)->GetDrawableObject(j)->CanvasResized(this);
        }
    }
}



void SceneCanvasComponent::mouseDown(const juce::MouseEvent& event)
{
    canvasManager->OnCanvasMouseDown(event);
}

void SceneCanvasComponent::mouseDrag(const juce::MouseEvent& event)
{
    canvasManager->OnCanvasMouseDrag(event);
}

void SceneCanvasComponent::mouseUp(const juce::MouseEvent& event)
{
    canvasManager->OnCanvasMouseUp(event);
}


