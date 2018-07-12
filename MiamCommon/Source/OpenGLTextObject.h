/*
  ==============================================================================

    OpenGLTextObject.h
    Created: 5 Jul 2018 11:38:16am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include <string>
#include "JuceHeader.h"

class OpenGLTextObject
{
public:
	OpenGLTextObject(String path, float _x, float _y, float _characterWidth, float _characterHeight, int _maxSize);
	~OpenGLTextObject();

	void initialiseText(OpenGLContext& context);
	void drawOneTexturedRectangle(OpenGLContext &context, juce::Matrix3D<float> &model, juce::Matrix3D<float> &testView, juce::Matrix3D<float> &testPerspective, std::string stringToDraw);
	void release();
		
private:
	void computeVertices();
	void computeUV(int idx, char character);
	void initialiseBuffer(OpenGLContext& context);
	void initialiseAttribute();
	void initialiseShaderProgram(OpenGLContext &context);

	float textX, textY, characterWidth, characterHeight;
	int maxSize;

	ScopedPointer<OpenGLShaderProgram> textShaderProgram;

	GLuint vertexBuffer;
	GLfloat *g_vertex_buffer_data;//[6 * 3];

	GLuint UVBuffer;
	GLfloat *g_UV_buffer_data;//[6 * 2];

	ScopedPointer<OpenGLShaderProgram::Attribute> positionText, colourText, vertexUV;
	ScopedPointer<OpenGLShaderProgram::Uniform> textProjectionMatrix, textViewMatrix, textModelMatrix, texture;
	std::unique_ptr<OpenGLTexture> textTexture;
	juce::Image image;

	String myTextVertexShader = "attribute vec4 position;\n"
		"attribute vec2 uvCoord;\n"
		"\n"
		"uniform mat4 modelMatrix;\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"\n"
		"out vec2 UV;\n"
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
#else
		"varying vec4 destinationColour;\n"
#endif
		"\n"
		"in vec2 UV;\n"
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

	Image resizeImageToPowerOfTwo(Image image)
	{
		if (!(isPowerOfTwo(image.getWidth()) && isPowerOfTwo(image.getHeight())))
			return image.rescaled(jmin(1024, nextPowerOfTwo(image.getWidth())),
				jmin(1024, nextPowerOfTwo(image.getHeight())));

		return image;
	}
};
