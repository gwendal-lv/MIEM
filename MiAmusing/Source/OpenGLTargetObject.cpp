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

#include "OpenGLTargetObject.h"

#include "MiamMath.h"

OpenGLTargetObject::OpenGLTargetObject(int _x, int _y, int _w, int _h, int _numAngle, int _Ncircles, float _r0)
{
	textTexture = std::make_unique<OpenGLTexture>();
	targetImage = Image(juce::Image::PixelFormat::ARGB, 1024, 1024, true);//Image(PixelARGB, 1024, 1024, true);
	Graphics g(targetImage);
	g.fillAll(Colours::black);//Colour((uint8)0, (uint8)0, (uint8)0, (uint8)0));
	g.setColour(Colours::lightgrey);
	g.setOpacity(0.5f);
	float thickness = 0.5f;

	int r0 = _r0 * 1024;
	int sideLength = 1024;
	int width = 1024;
	int height = 1024;
	int deltaX = (sideLength - 2.0f * r0) / (_Ncircles - 1);
	int ellipseVerticesCount = 32;
	int centerX = 512;
	int centerY = 512;
	for (int i = _Ncircles - 1; i > -1; --i)
	{
		/*g.drawEllipse((width - (sideLength - i * deltaX)) / 2.0f, (height - (sideLength - i * deltaX)) / 2.0f,
			float(sideLength - i * deltaX), float(sideLength - i * deltaX), thickness);*/
		
		juce::Path contour;
		float r = r0 + i * (1024.0f/2.0f - float(r0)) / (_Ncircles - 1);
		contour.startNewSubPath(centerX + r * (float)std::cos(0.0),
			centerY + r * (float)std::sin(0.0));
		for (int i = 1; i<ellipseVerticesCount; i++)
		{
			double normalizedAngle = (double)(i) / (double)(ellipseVerticesCount);

			contour.lineTo(centerX + r * (float)std::cos(2.0 * M_PI * normalizedAngle),
				centerY + r * (float)std::sin(2.0 * M_PI * normalizedAngle));
		}
		contour.closeSubPath();
		g.strokePath(contour, PathStrokeType(thickness));
	}

	int minDimension = sideLength;
	double currentAngle = 0.0;
	for (int i = 0; i < _numAngle; ++i)
	{
		currentAngle += 2 * M_PI / (double)_numAngle;
		g.drawLine((float)width / 2.0f, (float)height / 2.0f, (float)width / 2.0f + (float)minDimension / 2.0f * (float)std::cos(currentAngle), (float)height / 2.0f + (float)minDimension / 2.0f * (float)std::sin(currentAngle), thickness);
	}

	setBounds(_x, _y, _w, _h);

	// top left
	g_UV_buffer_data[0] = 0.0f;
	g_UV_buffer_data[1] = 1.0f;

	// top right
	g_UV_buffer_data[2] = 1.0f;
	g_UV_buffer_data[3] = 1.0f;

	//bottom right
	g_UV_buffer_data[4] = 1.0f;
	g_UV_buffer_data[5] = 0.0f;

	// top left
	g_UV_buffer_data[6] = 0.0f;
	g_UV_buffer_data[7] = 1.0f;

	// bottom right
	g_UV_buffer_data[8] = 1.0f;
	g_UV_buffer_data[9] = 0.0f;

	// bottom left
	g_UV_buffer_data[10] = 0.0f;
	g_UV_buffer_data[11] = 0.0f;

	needToUpdatePosition = true;
	needToUpdateUV = true;
}

OpenGLTargetObject::~OpenGLTargetObject()
{

}

void OpenGLTargetObject::initialise(OpenGLContext & context)
{
	needToRelease = false;
	waitForOpenGLResourcesRealeased();
	initialiseShaderProgram(context);
	initialiseBuffer(context);
	initialiseAttribute();
}

void OpenGLTargetObject::drawTarget(OpenGLContext & context, juce::Matrix3D<float>& model, juce::Matrix3D<float>& testView, juce::Matrix3D<float>& testPerspective)
{
	if (needToRelease)
	{
		textTexture->release();
		DBG("delete texture");
		DBG("texture deleted");
		textTexture = nullptr;
		destructionThread = std::thread(&OpenGLTargetObject::destructionThreadFunc, this);
	}
	else
	{
		if (shaderProgram != nullptr)
			shaderProgram->use();

		if (textModelMatrix != nullptr)
			textModelMatrix->setMatrix4(model.mat, 1, false);

		if (textProjectionMatrix != nullptr)
			textProjectionMatrix->setMatrix4(testPerspective.mat, 1, false);

		if (textViewMatrix != nullptr)
			textViewMatrix->setMatrix4(testView.mat, 1, false);

		if (textTexture != nullptr)
			textTexture->bind();

		if (positionText != nullptr && vertexUV != nullptr)
		{
			context.extensions.glEnableVertexAttribArray(positionText->attributeID);
			context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			if(needToUpdatePosition)
				context.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * 3 * sizeof(GLfloat), g_vertex_buffer_data);
			context.extensions.glVertexAttribPointer(positionText->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);

			context.extensions.glEnableVertexAttribArray(vertexUV->attributeID);
			context.extensions.glBindBuffer(GL_ARRAY_BUFFER, UVBuffer); // changer �a!
			if(needToUpdateUV)
				context.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * 2 * sizeof(GLfloat), g_UV_buffer_data);
			context.extensions.glVertexAttribPointer(vertexUV->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(float[2]), 0);

			glDrawArrays(GL_TRIANGLES, 0, 2 * 3);

			context.extensions.glDisableVertexAttribArray(positionText->attributeID);
			context.extensions.glDisableVertexAttribArray(vertexUV->attributeID);
		}
	}
}

void OpenGLTargetObject::release()
{
	needToRelease = true;
}

void OpenGLTargetObject::waitForOpenGLResourcesRealeased()
{
	if (needToRelease)
	{
		while (!destructionThread.joinable()) {}
		destructionThread.join();
		needToRelease = false;
	}
}

void OpenGLTargetObject::setBounds(int _x, int _y, int _w, int _h)
{
	needToUpdatePosition = true;

	// Top left
	g_vertex_buffer_data[0] = _x;
	g_vertex_buffer_data[1] = _y;
	g_vertex_buffer_data[2] = 0.01f;

	// Top right
	g_vertex_buffer_data[3] = _x + _w;
	g_vertex_buffer_data[4] = _y;
	g_vertex_buffer_data[5] = 0.01f;

	// bottom right
	g_vertex_buffer_data[6] = _x + _w;
	g_vertex_buffer_data[7] = _y + _h;
	g_vertex_buffer_data[8] = 0.01f;

	// Top left
	g_vertex_buffer_data[9] = _x;
	g_vertex_buffer_data[10] = _y;
	g_vertex_buffer_data[11] = 0.01f;

	// bottom right
	g_vertex_buffer_data[12] = _x + _w;
	g_vertex_buffer_data[13] = _y + _h;
	g_vertex_buffer_data[14] = 0.01f;

	// bottom left
	g_vertex_buffer_data[15] = _x;
	g_vertex_buffer_data[16] = _y + _h;
	g_vertex_buffer_data[17] = 0.01f;


}

void OpenGLTargetObject::initialiseBuffer(OpenGLContext & context)
{
	context.extensions.glGenBuffers(1, &vertexBuffer);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	context.extensions.glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GLfloat), g_vertex_buffer_data, GL_STATIC_DRAW);

	context.extensions.glGenBuffers(1, &UVBuffer);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	context.extensions.glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(GLfloat), g_UV_buffer_data, GL_STATIC_DRAW);
}

void OpenGLTargetObject::initialiseAttribute()
{
	positionText = std::make_unique<OpenGLShaderProgram::Attribute>(*shaderProgram.get(), "position");
	vertexUV = std::make_unique<OpenGLShaderProgram::Attribute>(*shaderProgram.get(), "uvCoord");

	textProjectionMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "projectionMatrix");
	textViewMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "viewMatrix");
	textModelMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "modelMatrix");
	texture = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "demoTexture");

	textTexture->loadImage(targetImage);

	if (texture != nullptr)
		texture->set(0);
}

void OpenGLTargetObject::initialiseShaderProgram(OpenGLContext & context)
{
	shaderProgram = std::make_unique<OpenGLShaderProgram>(context);
	shaderProgram->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(myTextVertexShader));
	shaderProgram->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(myTextFragmentShader));
	shaderProgram->link();

	shaderProgram->use();
}

void OpenGLTargetObject::destructionThreadFunc()
{
	shaderProgram->release();
	shaderProgram = nullptr;
	positionText = nullptr;
	vertexUV = nullptr;
	textProjectionMatrix = nullptr;
	textViewMatrix = nullptr;
	textModelMatrix = nullptr;
	texture = nullptr;
}
