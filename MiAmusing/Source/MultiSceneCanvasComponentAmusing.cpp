/*
  ==============================================================================

    MultiSceneCanvasComponentAmusing.cpp
    Created: 10 Jul 2017 11:45:38am
    Author:  ayup1

  ==============================================================================
*/

#include "MultiSceneCanvasComponentAmusing.h"
#include "AmusingSceneComponent.h"
#include "AreaOptionsComponent.h"
#include "MultiSceneCanvasManager.h"

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

	areaOptionsComponent = new AreaOptionsComponent();
	areaOptionsComponent->CompleteInitialisation(this);
	addChildComponent(areaOptionsComponent);
	areaOptionsComponent->setVisible(false);

	currentOptionClicked = Closed;
}

MultiSceneCanvasComponentAmusing::~MultiSceneCanvasComponentAmusing()
{
	//delete childrenCanvas;
	//MultiSceneCanvasComponent::~MultiSceneCanvasComponent();
}

void MultiSceneCanvasComponentAmusing::resized()
{
	//MultiSceneCanvasComponent::resized();
	
	//areaOptions.setBounds(0, 0, getWidth(), getHeight());
	if (areaOptionsComponent && areaOptionsComponent->isVisible())
	{
		Rectangle<int> r(getBounds());
		r.removeFromTop(24 + space);
		childrenCanvas->setBounds(r.removeFromLeft(getWidth()-150));
		areaOptionsComponent->setBounds(r);
	}
	else
	{
		childrenCanvas->setSize(getWidth(), getHeight() - 24 - space); // appelera l'update du canvasinteractor (le manager)
		childrenCanvas->setTopLeftPosition(0, 24 + space);
	}
	// Buttons positionning
	updateSceneButtonsBounds();
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

void MultiSceneCanvasComponentAmusing::showAreaOptions(bool shouldBeVisible)
{
	areaOptionsComponent->setVisible(shouldBeVisible);
	currentOptionClicked = Rhythm;
	resized();
}

void MultiSceneCanvasComponentAmusing::optionButtonClicked(OptionButtonClicked optionClicked)
{
	resized();
	if (optionClicked != currentOptionClicked)
	{
		currentOptionClicked = optionClicked;
		if (auto canvasManagerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(canvasManager))
		{
			if (auto amusingChildren = (AmusingSceneComponent*)(childrenCanvas))
			{
				switch (optionClicked)
				{
				case Octave:
				case Volume:
					amusingChildren->ShowSideBar(SideBarType::GrayScale);
					canvasManagerAsManager->OnDeleteExciter();
					canvasManagerAsManager->OnAddExciter();
					canvasManagerAsManager->SetEditingMode(optionClicked);
					break;
				case Speed:
					amusingChildren->ShowSideBar(SideBarType::ColourScale);
					canvasManagerAsManager->OnDeleteExciter();
					canvasManagerAsManager->OnAddExciter();
					canvasManagerAsManager->SetEditingMode(optionClicked);
					break;
				case Rhythm:
					amusingChildren->ShowSideBar(SideBarType::None);
					canvasManagerAsManager->SetEditingMode(optionClicked);
					canvasManagerAsManager->OnDeleteExciter();
					canvasManagerAsManager->SetMode(CanvasManagerMode::EditingArea);
					break;
				case Sample:
					amusingChildren->ShowSideBar(SideBarType::ColourButtons);
					canvasManagerAsManager->OnDeleteExciter();
					canvasManagerAsManager->OnAddExciter();
					canvasManagerAsManager->SetEditingMode(optionClicked);
					break;
				case Closed:
					amusingChildren->ShowSideBar(SideBarType::None);
					canvasManagerAsManager->resetAreaPosition();
					break;
				default:
					break;
				}
			}


		}
	}
}
