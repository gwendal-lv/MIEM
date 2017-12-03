/*
  ==============================================================================

    AmusingSceneComponent.h
    Created: 7 Jul 2017 3:39:08pm
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SceneCanvasComponent.h"
#include "AreaOptions.h"



//==============================================================================
/*
*/
class AmusingSceneComponent    : public SceneCanvasComponent,
	public std::enable_shared_from_this<AmusingSceneComponent>
{
public:
    AmusingSceneComponent();
    ~AmusingSceneComponent();

	void setSamplesColor(int Nsamples, Colour colorCode[]);
	
	void renderOpenGL() override; // ! in background-thread !
	AreaOptions areaOptions;

	void OnSpeedChanged(double newVelocity);
	void OnBaseNoteChanged(int newBaseNote);
	void OnVelocityChanged(double newVelocity);

	void OnColourChanged(Colour newColour);

	void SetAreaOptionsCenter(bpt center);
	void SetAreaOptionsVisible(bool show);
	void SetAreaOptionsVisible(bool show, double speed, double velocity, int octave);

	/*void mouseDown(const juce::MouseEvent &event) override;
	void mouseDrag(const juce::MouseEvent &event) override;
	void mouseUp(const juce::MouseEvent &event) override;*/

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmusingSceneComponent)
};
