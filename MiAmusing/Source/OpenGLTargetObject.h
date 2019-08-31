/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
	// elle sera utilis�e comme texture pour l'affichage OpenGL
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

