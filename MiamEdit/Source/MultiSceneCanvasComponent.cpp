/*
  ==============================================================================

    MultiSceneCanvasComponent.cpp
    Created: 16 Nov 2016 5:35:10pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MultiSceneCanvasComponent.h"

//==============================================================================
MultiSceneCanvasComponent::MultiSceneCanvasComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Children canvas at first
    childrenCanvas = new SceneCanvasComponent();
    addAndMakeVisible(childrenCanvas);
    
    // Buttons then
    // A CHANGER
    sceneChoiceTextButtons.push_back(new TextButton("Scene 1 trololo"));
    addAndMakeVisible(sceneChoiceTextButtons.back());
    sceneChoiceTextButtons.push_back(new TextButton("Scene 2 tralalala"));
    addAndMakeVisible(sceneChoiceTextButtons.back());
}

MultiSceneCanvasComponent::~MultiSceneCanvasComponent()
{
    delete childrenCanvas;
}

void MultiSceneCanvasComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (Colours::mediumpurple);   // PURPLE background
    /*

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ("MultiSceneCanvasComponent", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
    */
}

void MultiSceneCanvasComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    // Buttons on the top (to avoid bottom false manipulations.... good idea ?)
    // Same size as in the menus, but spaces are 4 PIXELS
    // height 24px + 4px under
    // BUTTONS SIZE SHOULD BE DIFFERENT ON COMPUTERS AND ON TABLETS/SMARTPHONES
    const int space = 4;
    
    // Children display canvas on the bottom
    childrenCanvas->setSize(getWidth(), getHeight() -24 -space);
    childrenCanvas->setTopLeftPosition(0, 24 +space);
    
    // Buttons positionning
    int buttonWidth = roundFloatToInt(((float)(getWidth())-(float)(space))/(float)(sceneChoiceTextButtons.size()))-space;
    for (size_t i=0 ; i<sceneChoiceTextButtons.size() ; i++)
    {
        sceneChoiceTextButtons[i]->setBounds(space+i*(buttonWidth+space), 0, buttonWidth, 24);
    }
}

