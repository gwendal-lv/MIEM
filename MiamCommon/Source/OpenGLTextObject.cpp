/*
  ==============================================================================

    OpenGLTextObject.cpp
    Created: 5 Jul 2018 11:38:16am
    Author:  ayup1

  ==============================================================================
*/

#include "OpenGLTextObject.h"
//
//#include <ft2build.h>
//#include FT_FREETYPE_H 

using namespace std;

OpenGLTextObject::OpenGLTextObject(String path, float _x, float _y, float _characterWidth, float _characterHeight, int _maxSize) : 
	textX(_x), textY(_y), characterWidth(_characterWidth), characterHeight(_characterHeight), maxSize(_maxSize)
{
	//FT_Library ft;
	//if (FT_Init_FreeType(&ft))
	//	DBG("ERROR::FREETYPE: Could not init FreeType Library");

	//FT_Face face;
	//if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	//	DBG("ERROR::FREETYPE: Failed to load font");



	textTexture =  std::make_unique<OpenGLTexture>();

	bool isBinary = false;
	int resourceId = 0;
	for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
	{
		if (path == BinaryData::namedResourceList[i])
		{
			isBinary = true;
			resourceId = i;
		}
	}

	if (isBinary)
	{
		int dataSize = 0;
		const void * srcData = BinaryData::getNamedResource(BinaryData::namedResourceList[resourceId], dataSize);
		image = resizeImageToPowerOfTwo(ImageCache::getFromMemory(srcData,dataSize));
	}
	else
		image = resizeImageToPowerOfTwo(ImageFileFormat::loadFrom(File(path)));

	g_vertex_buffer_data = new GLfloat[maxSize * 6 * 3];
	g_UV_buffer_data = new GLfloat[maxSize * 6 * 2];
	computeVertices();
	//std::string testString = "                      ";
	for(int i = 0; i < maxSize; ++i)
		computeUV(i,' ');
}

OpenGLTextObject::~OpenGLTextObject()
{
	delete[] g_vertex_buffer_data;
	delete[] g_UV_buffer_data;
}

void OpenGLTextObject::initialiseText(OpenGLContext& context)
{
	needToRelease = false;
	waitForOpenGLResourcesRealeased();
	initialiseShaderProgram(context);
	initialiseBuffer(context);
	initialiseAttribute();
}

void OpenGLTextObject::initialiseBuffer(OpenGLContext& context)
{
	context.extensions.glGenBuffers(1, &vertexBuffer);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	context.extensions.glBufferData(GL_ARRAY_BUFFER, maxSize * 6 * 3 * sizeof(GLfloat), g_vertex_buffer_data, GL_STATIC_DRAW);

	context.extensions.glGenBuffers(1, &UVBuffer);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	context.extensions.glBufferData(GL_ARRAY_BUFFER, maxSize * 6 * 2 * sizeof(GLfloat), g_UV_buffer_data, GL_STATIC_DRAW);
}

void OpenGLTextObject::initialiseAttribute()
{
	positionText = std::make_unique<OpenGLShaderProgram::Attribute>(*textShaderProgram.get(), "position");
	vertexUV = std::make_unique<OpenGLShaderProgram::Attribute>(*textShaderProgram.get(), "uvCoord");

	textProjectionMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*textShaderProgram.get(), "projectionMatrix");
	textViewMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*textShaderProgram.get(), "viewMatrix");
	textModelMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*textShaderProgram.get(), "modelMatrix");
	texture = std::make_unique<OpenGLShaderProgram::Uniform>(*textShaderProgram.get(), "demoTexture");

	textTexture->loadImage(image);

	if (texture != nullptr)
		texture->set(0);
}

void OpenGLTextObject::initialiseShaderProgram(OpenGLContext &context)
{
	textShaderProgram = std::make_unique<OpenGLShaderProgram>(context);
	textShaderProgram->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(myTextVertexShader));
	textShaderProgram->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(myTextFragmentShader));
	textShaderProgram->link();

	textShaderProgram->use();
}

void OpenGLTextObject::destructionThreadFunc()
{
	textShaderProgram->release();
	textShaderProgram = nullptr;
	positionText = nullptr;
	vertexUV = nullptr;
	textProjectionMatrix = nullptr;
	textViewMatrix = nullptr;
	textModelMatrix = nullptr;
	texture = nullptr;
}

void OpenGLTextObject::waitForOpenGLResourcesRealeased()
{
	if (needToRelease)
	{
		while (!destructionThread.joinable()) {}
		destructionThread.join();
		needToRelease = false;
	}
}

void OpenGLTextObject::drawOneTexturedRectangle(OpenGLContext &context, juce::Matrix3D<float> &model, juce::Matrix3D<float> &testView, juce::Matrix3D<float> &testPerspective, std::u16string stringToDraw[])
{
	computeVertices();

	if (needToRelease)
	{
		textTexture->release();
		DBG("delete texture");
		DBG("texture deleted");
		textTexture = nullptr;


		
		destructionThread = std::thread(&OpenGLTextObject::destructionThreadFunc, this);
		
	}

	int numChar = 0;
	std::u16string::iterator it = stringToDraw[0].begin();
	while (it != stringToDraw[0].end())
	{
		char32_t currentCodePoint = 0x00000000;
		UTF16ToCodePoint(it, currentCodePoint);
		computeUV(numChar, currentCodePoint);
		++numChar;
	}
	for(int i = numChar; i < maxSize;++i)
		computeUV(i, (char32_t)' ');

	
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
	context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (!needToRelease)
	{
		if (textShaderProgram != nullptr)
			textShaderProgram->use();


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
			//context.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, maxSize * 6 * 3 * sizeof(GLfloat), g_oneVertex_buffer_data);
			context.extensions.glVertexAttribPointer(positionText->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);

			context.extensions.glEnableVertexAttribArray(vertexUV->attributeID);
			context.extensions.glBindBuffer(GL_ARRAY_BUFFER, UVBuffer); // changer ça!
			context.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, maxSize * 6 * 2 * sizeof(GLfloat), g_UV_buffer_data);
			context.extensions.glVertexAttribPointer(vertexUV->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(float[2]), 0);

			glDrawArrays(GL_TRIANGLES, 0, maxSize * 2 * 3);

			context.extensions.glDisableVertexAttribArray(positionText->attributeID);
			context.extensions.glDisableVertexAttribArray(vertexUV->attributeID);
		}
	}
}

void OpenGLTextObject::computeVertices()
{
	for (int i = 0; i < maxSize; ++i)
	{
		g_vertex_buffer_data[i * 18 + 0] = textX + i * characterWidth;
		g_vertex_buffer_data[i * 18 + 1] = textY + 0.0f;
		g_vertex_buffer_data[i * 18 + 2] = 0.0f;

		g_vertex_buffer_data[i * 18 + 3] = textX + (i + 1) * characterWidth;
		g_vertex_buffer_data[i * 18 + 4] = textY + 0.0f;
		g_vertex_buffer_data[i * 18 + 5] = 0.0f;

		g_vertex_buffer_data[i * 18 + 6] = textX + (i + 1) * characterWidth;
		g_vertex_buffer_data[i * 18 + 7] = textY + characterHeight;
		g_vertex_buffer_data[i * 18 + 8] = 0.0f;

		g_vertex_buffer_data[i * 18 + 9] = textX + i * characterWidth;
		g_vertex_buffer_data[i * 18 + 10] = textY + characterHeight;
		g_vertex_buffer_data[i * 18 + 11] = 0.0f;

		g_vertex_buffer_data[i * 18 + 12] = textX + (i + 1) * characterWidth;
		g_vertex_buffer_data[i * 18 + 13] = textY + characterHeight;
		g_vertex_buffer_data[i * 18 + 14] = 0.0f;

		g_vertex_buffer_data[i * 18 + 15] = textX + i * characterWidth;
		g_vertex_buffer_data[i * 18 + 16] = textY + 0.0f;
		g_vertex_buffer_data[i * 18 + 17] = 0.0f;
	}
}

void OpenGLTextObject::computeUV(int idx, char32_t character)
{
	float wRatio = (39.0f / 1024.0f); // width of character in % of texture width
	float hRatio = 1.0f / 16.0f; // height of character in % of texture height

	float xNew = wRatio * float(character % 16);//float(character % 16) * wRatio;//float(character % 16) / 8.0f;
	float yNew = 1.0f - float(int(character / 16) - 2) * hRatio;//1 - float(int(character / 16)) * hRatio;//(7 - (float(character / 8) - 4)) / 8.0f;//(float(character / 16)-1.0f) / 8.0f;

	// UV :30------>1
	//     | \      |
	//     |  \     |
	//     |   \    |
	//     |    \   |
	//     |     \  |
	//     |      V V
	//     5------>42


	// coin inferieur droit

	g_UV_buffer_data[idx * 12 + 0] = xNew;//xNew + 0.0f;
	g_UV_buffer_data[idx * 12 + 1] = yNew - hRatio;//0.0f;//yNew - hRatio;

	g_UV_buffer_data[idx * 12 + 2] = xNew + wRatio;//1.0f;//xNew + wRatio;
	g_UV_buffer_data[idx * 12 + 3] = yNew - hRatio; //0.0f;//yNew - hRatio;

	g_UV_buffer_data[idx * 12 + 4] = xNew + wRatio; //1.0f;//xNew + wRatio;
	g_UV_buffer_data[idx * 12 + 5] = yNew; //1.0f;//yNew - 2 * hRatio;


	//coin superieur gauche

	g_UV_buffer_data[idx * 12 + 6] = xNew;//xNew + 0.0f;
	g_UV_buffer_data[idx * 12 + 7] = yNew;//1.0f;//yNew - hRatio;

	g_UV_buffer_data[idx * 12 + 8] = xNew + wRatio; //1.0f;//xNew + wRatio;
	g_UV_buffer_data[idx * 12 + 9] = yNew; //1.0f;//yNew - 2 * hRatio;

	g_UV_buffer_data[idx * 12 + 10] = xNew;//xNew + 0.0f;
	g_UV_buffer_data[idx * 12 + 11] = yNew - hRatio;;//0.0f;//yNew - 2 * hRatio;
}

void OpenGLTextObject::UTF16ToCodePoint(std::u16string::iterator &it, char32_t &currentCodePoint)
{
	if (*it > (char16_t)(0x9999)) // est codé sur 2 bytes
	{
		currentCodePoint = (*it - 0xD800) << 10;
		++it;
		currentCodePoint += (*it - 0xDC00);
		currentCodePoint += (0x00010000);
		++it;
	}
	else
	{
		currentCodePoint = (*it);
		++it;
	}
}

void OpenGLTextObject::release()
{
	// rendre inaccessible par le thread JUCE les differents objets
	//textTexture = nullptr;




	needToRelease = true;


}