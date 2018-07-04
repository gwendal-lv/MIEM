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


	GLuint coulourBoutonsVertex;
	GLfloat g_coulourBoutonsVertex_buffer_data[3 * 4 * 10];

	GLuint coulourBoutonsCoulour;
	GLfloat g_coulourBoutonsCoulour_buffer_data[4 * 4 * 10];

	GLuint coulourBoutonsIndex;
	GLuint g_coulourBoutonsIndex_buffer_data[6 * 10];

	GLuint scaleMarkingVertex;
	GLfloat g_scaleMarkingVertex_buffer_data[(128 + 1) * 4 * 3];

	GLuint scaleMarkingCoulour;
	GLfloat g_scaleMarkingCoulour_buffer_data[(128 + 1) * 4 * 4];

	GLuint scaleMarkingIndex;
	GLuint g_scaleMarkingIndex_buffer_data[(128 + 1) * 6];

	void computeScaleMarking(Point<float> beginPoint, float height, float width, int numMarks);

	GLuint textVertexBuffer;
	GLfloat g_textVertex_buffer_data[4 * 3];

	GLuint textCoulourBuffer;
	GLfloat g_textCoulour_buffer_data[4 * 4];

	GLuint textUVBuffer;
	GLfloat g_textUV_buffer_data[4 * 2];

	GLuint textIndexBuffer;
	unsigned int g_textIndex_buffer_data[6];

	void computeTextBuffers();

	OpenGLTexture textTexture;

	ScopedPointer<OpenGLShaderProgram::Attribute> positionText, vertexUV;
	std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, modelMatrix, texture;

	Image resizeImageToPowerOfTwo(Image image);

	ScopedPointer<OpenGLShaderProgram> textShaderProgram;

	String textVertexShader =
		"attribute vec4 positionText;\n"
#if JUCE_OPENGL_ES
		"attribute lowp vec2 vertexUV;\n"
#else
		"attribute vec2 vertexUV;\n"
#endif
		"\n"

		"uniform mat4 modelMatrix;\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"\n"
#if JUCE_OPENGL_ES
		"varying lowp vec2 UV;\n"
#else
		"varying vec2 UV;\n"
#endif
		"\n"
		"void main()\n"
		"{\n"
		"\n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionText;\n"
		"    UV = vertexUV;"
		"}\n";

	String textFragmentShader =
#if JUCE_OPENGL_ES
		"varying lowp vec4 destinationColour;\n"
#else
		"varying vec4 destinationColour;\n"
#endif
		"\n"
#if JUCE_OPENGL_ES
		"varying lowp vec2 UV;\n"
#else
		"varying vec2 UV;\n"
#endif
		"uniform sampler2D demoTexture;"
		"\n"
		"void main()\n"
		"{\n"
#if JUCE_OPENGL_ES
		"   highp float l = 0.3;\n"

#else
		"   float l = 0.3;\n"

#endif
		"    gl_FragColor = texture2D(demoTexture,UV);\n"
		"    gl_FragColor.w = 1.0;\n"
		"    gl_FragColor.x = 1.0;\n"
		"    gl_FragColor.y = 1.0;\n"
		"}\n";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmusingSceneComponent)
};
