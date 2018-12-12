/*
  ==============================================================================

    OpenGLTextObject.cpp
    Created: 5 Jul 2018 11:38:16am
    Author:  ayup1

  ==============================================================================
*/

#include <assert.h> // not included when compiling in VS2017

#include "OpenGLTextObject.h"


using namespace std;
using namespace Miam;

OpenGLTextObject::OpenGLTextObject(float _x, float _y, float _characterWidth, float _characterHeight, int _maxSize) : 
	textX(_x), textY(_y), characterWidth(_characterWidth), characterHeight(_characterHeight), maxSize(_maxSize)
{
    resourcesReleased = true; // default state
    
    
    // - - - chargement de l'image de la police de caractères - - -
    //image = LoadImage(path);
    // maintenant : géré par une classe mère
    
    // - - - calculd des VBO et UV buffers - - -
	g_vertex_buffer.resize(maxSize * 6 * 3);
	g_UV_buffer.resize(maxSize * 6 * 2);
    
    
    // To be done only once for a given size
    computeVertices();
    
    // init to empty string
    std::u16string emptyString = u"";
    SetText(emptyString);
}

OpenGLTextObject::~OpenGLTextObject()
{
}

void OpenGLTextObject::Initialise(OpenGLContext& context, OpenGLFontManager* _manager)
{
    SetFontManager(_manager);
    
    resourcesReleased = false;
}


void OpenGLTextObject::releaseResourcesSync()
{
    resourcesReleased = true;
}


void OpenGLTextObject::SetText(std::u16string& stringToDraw)
{
    currentText = stringToDraw;
    
    computeVertices();
    
    int numChar = 0;
    std::u16string::iterator it = stringToDraw.begin();
    while (it != stringToDraw.end())
    {
        // Are there more characters in the string, compared to the parameter given at construction ?
        if (numChar >= maxSize)
        {
            assert(false); // this should not happen... too many letters are asked for drawing
            it = stringToDraw.end(); // better than a dirty 'break' !
        }
        else
        {
            char32_t currentCodePoint = 0x00000000;
            UTF16ToCodePoint(it, currentCodePoint);
            recomputeUV(numChar, currentCodePoint);
            ++numChar;
        }
    }
    for(int i = numChar; i < maxSize;++i)
        recomputeUV(i, (char32_t)' ');
}


void OpenGLTextObject::drawOneTexturedRectangle(OpenGLContext &context, juce::Matrix3D<float> &model, juce::Matrix3D<float> &testView, juce::Matrix3D<float> &testPerspective, GLfloat globalAlpha)
{
    context.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
    context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	if (resourcesReleased)
	{
        DBG("[OpenGLTextObject] error : OpenGL render request but resources were released.");
        assert(false); // alerte au gogole !
	}
    else
    {
        // string to display must have been set before calling this drawing function
        
		fontManager->textShaderProgram->use();
        
        // Uniforms qui sont liés au shader activé précédemment
		fontManager->textModelMatrixUniform->setMatrix4(model.mat, 1, false);
        fontManager->textProjectionMatrixUniform->setMatrix4(testPerspective.mat, 1, false);
        fontManager->textViewMatrixUniform->setMatrix4(testView.mat, 1, false);
        fontManager->globalAlphaUniform->set(globalAlpha);
        
        fontManager->fontTexture->bind();
    
        // The VBOs are stored within the manager are shared with all areas
        // => glBufferSubData must update all necessary data
        context.extensions.glEnableVertexAttribArray(fontManager->textPositionAttribute->attributeID);
        context.extensions.glBindBuffer(GL_ARRAY_BUFFER, fontManager->vertexBuffer);
        context.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, maxSize * 6 * 3 * sizeof(GLfloat), g_vertex_buffer.data());
        context.extensions.glVertexAttribPointer(fontManager->textPositionAttribute->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);

        context.extensions.glEnableVertexAttribArray(fontManager->textUVAttribute->attributeID);
        context.extensions.glBindBuffer(GL_ARRAY_BUFFER, fontManager->UVBuffer); // changer ça!
        context.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, maxSize * 6 * 2 * sizeof(GLfloat), g_UV_buffer.data());
        context.extensions.glVertexAttribPointer(fontManager->textUVAttribute->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(float[2]), 0);

        glDrawArrays(GL_TRIANGLES, 0, maxSize * 2 * 3);

        context.extensions.glDisableVertexAttribArray(fontManager->textPositionAttribute->attributeID);
        context.extensions.glDisableVertexAttribArray(fontManager->textUVAttribute->attributeID);
    
        
        // Unbind VBOs par sécurité...
        context.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
        context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void OpenGLTextObject::computeVertices()
{
    float currentXPos = 0.0f;
	for (int i = 0; i < maxSize; ++i)
	{
        float currentCharWidth = characterWidth; // default value
        if ( i < currentText.size() )
        {
            currentCharWidth *= getCharWidthRatio(currentText[i]);
        }
        
        // Points definition
		g_vertex_buffer[i * 18 + 0] = textX + currentXPos;
		g_vertex_buffer[i * 18 + 1] = textY + 0.0f;
		g_vertex_buffer[i * 18 + 2] = 0.0f;

		g_vertex_buffer[i * 18 + 3] = textX + currentXPos + currentCharWidth;
		g_vertex_buffer[i * 18 + 4] = textY + 0.0f;
		g_vertex_buffer[i * 18 + 5] = 0.0f;

		g_vertex_buffer[i * 18 + 6] = textX + currentXPos + currentCharWidth;
		g_vertex_buffer[i * 18 + 7] = textY - characterHeight;
		g_vertex_buffer[i * 18 + 8] = 0.0f;

		g_vertex_buffer[i * 18 + 9] = textX + currentXPos;
		g_vertex_buffer[i * 18 + 10] = textY - characterHeight;
		g_vertex_buffer[i * 18 + 11] = 0.0f;

		g_vertex_buffer[i * 18 + 12] = textX + currentXPos + currentCharWidth;
		g_vertex_buffer[i * 18 + 13] = textY - characterHeight;
		g_vertex_buffer[i * 18 + 14] = 0.0f;

		g_vertex_buffer[i * 18 + 15] = textX  + currentXPos;
		g_vertex_buffer[i * 18 + 16] = textY + 0.0f;
		g_vertex_buffer[i * 18 + 17] = 0.0f;
        
        // Current X pos update
        currentXPos += currentCharWidth;
	}
}

void OpenGLTextObject::recomputeUV(int idx, char32_t character)
{
    // width of character in % of texture width
    float wRatio = (39.0f / 1024.0f);
     // ratio specific to the current character (i, j, l, etc...)
    float charWRatio = wRatio * getCharWidthRatio(character);
    
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

	g_UV_buffer[idx * 12 + 0] = xNew;
	g_UV_buffer[idx * 12 + 1] = yNew - hRatio;

	g_UV_buffer[idx * 12 + 2] = xNew + charWRatio;
	g_UV_buffer[idx * 12 + 3] = yNew - hRatio;

	g_UV_buffer[idx * 12 + 4] = xNew + charWRatio;
	g_UV_buffer[idx * 12 + 5] = yNew;


	//coin superieur gauche

	g_UV_buffer[idx * 12 + 6] = xNew;
	g_UV_buffer[idx * 12 + 7] = yNew;

	g_UV_buffer[idx * 12 + 8] = xNew + charWRatio;
	g_UV_buffer[idx * 12 + 9] = yNew;

	g_UV_buffer[idx * 12 + 10] = xNew;
	g_UV_buffer[idx * 12 + 11] = yNew - hRatio;;
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

float OpenGLTextObject::getCharWidthRatio(char32_t u32character)
{
    switch(u32character)
    {
        case 'i' :
        case 'I' :
            return 0.4;
            
        case 'j' :
        case 'l' :
        case 'J' :
            return 0.45f;
            
        case 'f' :
        case 't' :
        case 'r' :
            return 0.5f;
            
        case ' ' :
            return 0.7;
            
        case 'm' :
        case 'M' :
        case 'w' :
        case 'W' :
            return 0.95f;
            
        default :
            return 0.7f; // une lettre fait en moyenne 90% de son espace dans la texture !
    }
}

