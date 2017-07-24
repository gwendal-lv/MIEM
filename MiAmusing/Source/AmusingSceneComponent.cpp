/*
  ==============================================================================

    AmusingSceneComponent.cpp
    Created: 7 Jul 2017 3:39:08pm
    Author:  ayup1

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AmusingSceneComponent.h"
#include "MultiSceneCanvasManager.h"

#include <memory>

//==============================================================================
AmusingSceneComponent::AmusingSceneComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.


	openGlContext.attachTo(areaOptions);
	addAndMakeVisible(areaOptions); // test pour voir si on le voit bien
	areaOptions.setBounds(0 , 0, 300, 200);
	areaOptions.CompleteInitialization(this);
	areaOptions.setVisible(false);
}

AmusingSceneComponent::~AmusingSceneComponent()
{
	//openGlContext.detach(areaOptions);
	//openGlContext.detach();
}

void AmusingSceneComponent::renderOpenGL()
{
	SceneCanvasComponent::renderOpenGL();
	//areaOptions.setBounds(0, 0, getWidth(), getHeight());
}

//void AmusingSceneComponent::mouseDown(const juce::MouseEvent &event)
//{
//	DBG("AmusingSceneComponent::mouseDown");
//}
//
//void AmusingSceneComponent::mouseDrag(const juce::MouseEvent &event)
//{
//	DBG("AmusingSceneComponent::mouseDrag");
//}
//
//void AmusingSceneComponent::mouseUp(const juce::MouseEvent &event)
//{
//	DBG("AmusingSceneComponent::mouseUp");
//}

void AmusingSceneComponent::OnSpeedChanged(double newSpeed)
{
	DBG("newSpeed = " + (String)newSpeed);
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeSpeed(newSpeed);
		}
	}
}

void AmusingSceneComponent::OnBaseNoteChanged(double newBaseNote)
{
	DBG("newBaseNote = " + (String)newBaseNote);
	//canvasManager.lock()->
	//if(auto canvasManagerAsManager = std::dynamic_pointer_cast)
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeBaseNote(newBaseNote);
		}
	}
}

void AmusingSceneComponent::SetAreaOptionsCenter(bpt center)
{
	areaOptions.setCentrePosition(center.get<0>(), center.get<1>());
	
}

void AmusingSceneComponent::SetAreaOptionsVisible(bool show)
{
	if (show)
	{

		areaOptions.setVisible(true);
	}
	else
		areaOptions.setVisible(false);
}

void AmusingSceneComponent::SetAreaOptionsVisible(bool show,double speed)
{
	if (show)
	{
		areaOptions.setSpeedSliderValue(speed);
		areaOptions.setVisible(true);
	}
	else
		areaOptions.setVisible(false);
}
