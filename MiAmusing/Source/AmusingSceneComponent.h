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

#include <vector>

#include "AreaOptions.h"

enum SideBarType
{
	None,
	GrayScale,
	TextScale,
	ColourButtons,
	ColourScale,
	ScaleMarking,
};

//==============================================================================
/*
*/
class AmusingSceneComponent    : public SceneCanvasComponent,
	public std::enable_shared_from_this<AmusingSceneComponent>,
	public Button::Listener
{

public:
    AmusingSceneComponent();
    ~AmusingSceneComponent();

	void setSamplesColor(int Nsamples, Colour colorCode[]);

	void addColourSample(int index, Colour color);
	
	void renderOpenGL() override; // ! in background-thread !
	void newOpenGLContextCreated() override;

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
	void mouseDrag(const juce::MouseEvent &event) override;

	void mouseUp(const juce::MouseEvent & event) override;

	void ShowSideBar(SideBarType sideBarType);
	void SetNumScaleMarking(int _numScaleMarking) { numScaleMarking = _numScaleMarking; }

	/*void mouseDown(const juce::MouseEvent &event) override;
	void mouseDrag(const juce::MouseEvent &event) override;
	void mouseUp(const juce::MouseEvent &event) override;*/

private:
	static const int Npolygons = 20;

	bool isOptionShowed;
	SideBarType currentSideBarType;
	std::vector<Colour> buttonsColor;
	int numScaleMarking;

	GLuint testSideBarVertexBuffer;
	GLfloat g_testSideBarVertex_buffer_data[4 * 3];

	GLuint testSideBarCoulourBuffer;
	GLfloat g_testSideBarCoulour_buffer_data[4 * 4];

	GLuint testSideBarIndexBuffer;
	unsigned int g_testSideBarIndex_buffer_data[6];


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmusingSceneComponent)
};
