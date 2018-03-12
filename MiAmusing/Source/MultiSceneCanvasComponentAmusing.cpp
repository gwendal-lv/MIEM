/*
  ==============================================================================

    MultiSceneCanvasComponentAmusing.cpp
    Created: 10 Jul 2017 11:45:38am
    Author:  ayup1

  ==============================================================================
*/

#include "MultiSceneCanvasComponentAmusing.h"
#include "AmusingSceneComponent.h"

MultiSceneCanvasComponentAmusing::MultiSceneCanvasComponentAmusing()
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	// Children canvas at first
	if (childrenCanvas)
		delete childrenCanvas;
	childrenCanvas = new AmusingSceneComponent();//AmusingSceneComponent();
	addAndMakeVisible(childrenCanvas);

	// Buttons then
	// A CHANGER
	sceneChoiceTextButtons.push_back(new TextButton("Default unique scene"));
	addAndMakeVisible(sceneChoiceTextButtons.back());
	//addAndMakeVisible(areaOptions);
}

MultiSceneCanvasComponentAmusing::~MultiSceneCanvasComponentAmusing()
{
	//delete childrenCanvas;
	//MultiSceneCanvasComponent::~MultiSceneCanvasComponent();
}

void MultiSceneCanvasComponentAmusing::resized()
{
	MultiSceneCanvasComponent::resized();
	//areaOptions.setBounds(0, 0, getWidth(), getHeight());
}

void MultiSceneCanvasComponentAmusing::setSamplesColor(int Nsamples, Colour colorCode[])
{
	if (AmusingSceneComponent* childrenCanvasAmusing = (AmusingSceneComponent*)childrenCanvas)
	{
		childrenCanvasAmusing->setSamplesColor(Nsamples, colorCode);
	}
}

void MultiSceneCanvasComponentAmusing::addColourSample(int index, Colour colour)
{
	if (AmusingSceneComponent* childrenCanvasAmusing = (AmusingSceneComponent*)childrenCanvas)
	{
		childrenCanvasAmusing->addColourSample(index, colour);
	}
}