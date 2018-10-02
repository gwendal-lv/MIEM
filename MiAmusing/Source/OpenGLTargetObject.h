/*
  ==============================================================================

    OpenGLTargetObject.h
    Created: 1 Oct 2018 11:51:10am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include <thread>
#include "JuceHeader.h"

class OpenGLTargetObject
{
public:
	OpenGLTargetObject(int _x, int _y, int _w, int _h, int _angleInc, int _Ncicrles, float _r0);
	~OpenGLTargetObject();

	void initialise(OpenGLContext& context);
	void drawTarget(OpenGLContext &context, juce::Matrix3D<float> &model, juce::Matrix3D<float> &testView, juce::Matrix3D<float> &testPerspective);
	void release();
	void waitForOpenGLResourcesRealeased();
	void setBounds(int _x, int _y, int _w, int _h);

private:
	bool needToUpdatePosition;
	bool needToUpdateUV;

	GLuint vertexBuffer;
	GLfloat g_vertex_buffer_data[6 * 3];

	GLuint UVBuffer;
	GLfloat g_UV_buffer_data[6 * 2];

	// image sur laquelle on dessinera la cible avec les fonction JUCE
	// elle sera utilisée comme texture pour l'affichage OpenGL
	Image targetImage;

	// fonction d'initialisation OpenGL
	void initialiseBuffer(OpenGLContext& context);
	void initialiseAttribute();
	void initialiseShaderProgram(OpenGLContext &context);

	// attributs openGL
	std::unique_ptr<OpenGLShaderProgram::Attribute> positionText, vertexUV;
	std::unique_ptr<OpenGLShaderProgram::Uniform> textProjectionMatrix, textViewMatrix, textModelMatrix, texture;
	std::unique_ptr<OpenGLTexture> textTexture;
	std::unique_ptr<OpenGLShaderProgram> shaderProgram;

	String myTextVertexShader = "attribute vec4 position;\n"
		"attribute vec2 uvCoord;\n"
		"\n"
		"uniform mat4 modelMatrix;\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"\n"
#if JUCE_OPENGL_ES
		"varying vec2 UV;\n"
#else
		"varying lowp vec2 UV;\n"
#endif
		"\n"
		"void main()\n"
		"{\n"
		"\n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;\n"
		"    UV = uvCoord;"
		"}\n";

	String myTextFragmentShader =
#if JUCE_OPENGL_ES
		"varying lowp vec4 destinationColour;\n"
		"varying lowp vec2 UV;\n"
#else
		"varying vec4 destinationColour;\n"
		"varying vec2 UV;\n"
#endif
		"\n"
		"uniform sampler2D demoTexture;\n"
		"\n"
		"void main()\n"
		"{\n"
#if JUCE_OPENGL_ES
		"   highp float l = 0.3;\n"

#else
		"   float l = 0.3;\n"

#endif
		"    gl_FragColor = texture2D(demoTexture,UV);\n"
		"}\n";


	// destruction des attributs openGL via un thread
	std::thread destructionThread;
	void destructionThreadFunc();


	std::atomic<bool> needToRelease;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLTargetObject)
};

