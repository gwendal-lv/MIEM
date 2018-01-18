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
	public std::enable_shared_from_this<AmusingSceneComponent>,
	public ButtonListener
{
public:
    AmusingSceneComponent();
    ~AmusingSceneComponent();

	void setSamplesColor(int Nsamples, Colour colorCode[]);
	
	void renderOpenGL() override; // ! in background-thread !
	AreaOptions areaOptions;
	TextButton *showOptionsButton;

	void OnSpeedChanged(double newVelocity);
	void OnBaseNoteChanged(int newBaseNote);
	void OnVelocityChanged(double newVelocity);

	void OnColourChanged(Colour newColour, int colourIdx);

	void SetAreaOptionsCenter(bpt center);
	void SetAreaOptionsVisible(bool show);
	void SetAreaOptionsVisible(bool show, double speed, double velocity, int octave, int colorIdx);

	void buttonClicked(Button* buttonClicked) override;

	void mouseDown(const juce::MouseEvent &event) override;
	void mouseDoubleClick(const juce::MouseEvent &event) override;

	/*void mouseDown(const juce::MouseEvent &event) override;
	void mouseDrag(const juce::MouseEvent &event) override;
	void mouseUp(const juce::MouseEvent &event) override;*/

private:
	bool isOptionShowed;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmusingSceneComponent)
};
