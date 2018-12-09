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
using namespace Miam;

OpenGLTextObject::OpenGLTextObject(float _x, float _y, float _characterWidth, float _characterHeight, int _maxSize) : 
	textX(_x), textY(_y), characterWidth(_characterWidth), characterHeight(_characterHeight), maxSize(_maxSize)
{
    needToRelease = false;
    resourcesReleased = true; // default state
    
    
    // - - - chargement de l'image de la police de caractères - - -
    //image = LoadImage(path);
    // maintenant : géré par une classe mère
    
    // - - - calculd des VBO et UV buffers - - -
	g_vertex_buffer_data = new GLfloat[maxSize * 6 * 3];
	g_UV_buffer_data = new GLfloat[maxSize * 6 * 2];
	computeVertices();
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
    resourcesReleased = false;

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

	if (texture != nullptr)
		texture->set(0);
    else
        assert(false); // cannot draw anything....
}

void OpenGLTextObject::initialiseShaderProgram(OpenGLContext &context)
{
    /// - - - VERTEX shader - - -
    myTextVertexShader =
    "attribute vec4 position;                                       \n"
    "attribute vec2 uvCoord;                                        \n" // envoyé par CPU (peut être adapté)
    
    "uniform mat4 modelMatrix;                                      \n"
    "uniform mat4 projectionMatrix;                                 \n"
    "uniform mat4 viewMatrix;                                       \n"
    
#if JUCE_OPENGL_ES // lowp seems reserved to embedded platforms
    "varying lowp vec2 UV;                                          \n"
#else
    "varying vec2 UV;                                               \n"
#endif
    
    "void main() "
    "{ "
    "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position; "
    "    UV = uvCoord; "
    "}                                                              \n";
    
    
    /// - - - FRAGMENT shader - - -
    myTextFragmentShader =
#if JUCE_OPENGL_ES
    "varying lowp vec2 UV;                                          \n"
#else
    "varying vec2 UV;                                               \n"
#endif
    // Texture que l'on suppose en niveau de gris. Blanc = opaque, Noir = transparent
    "uniform sampler2D demoTexture;                                 \n" // nom à changer...
    
    "void main() "
    "{ "
    "    vec4 currentFrag = texture2D(demoTexture,UV); " // ordre vec4 : xyzw correspond à RGBA
    // mais en fait : .rgba utilisables directement.... sucre syntaxique "swizzle"
    // traitement sur la texture : point gris => BLANC transparent
    "    if (currentFrag.r < 1.0) "
    "        currentFrag.a = currentFrag.r; " // valeur d'opacité était dans le niveau de gris
    "    else " // par sécurité... pour la suite de l'algo
    "        currentFrag.a = 1.0; " // valeur d'opacité était dans le niveau de gris
    "    currentFrag.rgb = vec3(1.0, 1.0, 1.0); " // blanc dans tous les cas
    
    // Rajout d'une ombre à +0.3% en UV (on se fait plaisir, dans le shader....)
    "    vec4 shadowFrag = texture2D(demoTexture, UV + vec2(-0.001, 0.003) ); "
    // traitement différent du précédent : point gris => NOIR transparent
    "    if (shadowFrag.r < 1.0) "
    "        shadowFrag.a = shadowFrag.r; "
    "    else " // sécurité
    "        shadowFrag.a = 1.0; "
    "    shadowFrag.rgb = vec3(0.0, 0.0, 0.0); " // noir dans tous les cas
    
    // Opérateur de mélange "A over B" https://en.wikipedia.org/wiki/Alpha_compositing
    // pour mélange de l'ombre dans le current frag (si nécessaire)
    "    if (currentFrag.a < 1.0) "
    "    { "
    "        float blendedAlpha = currentFrag.a + shadowFrag.a * (1.0 - currentFrag.a); "
    "        currentFrag.rgb = ( currentFrag.a * currentFrag.rgb + shadowFrag.rgb * shadowFrag.a * (1.0-currentFrag.a) ) / blendedAlpha ; "
    "        currentFrag.a = blendedAlpha; "
    "    } "
    
    // Sinon, sortie du shader
    "    gl_FragColor = currentFrag; " // gl_FragColor seems deprecated...  https://stackoverflow.com/questions/51459596/using-gl-fragcolor-vs-out-vec4-color
    "}                                                              \n";
    
    
    // - - - Compiling and registering shaders - - -
	textShaderProgram = std::make_unique<OpenGLShaderProgram>(context);

    // vérifier quand même que ça ne pose pas de soucis....
    textShaderProgram->addVertexShader(myTextVertexShader);
    textShaderProgram->addFragmentShader(myTextFragmentShader);
    
	textShaderProgram->link();

    // If we don't use() here, OpenGL crashes after...
	textShaderProgram->use();
}

void OpenGLTextObject::releaseResourcesSync()
{
	textShaderProgram->release();
    
	textShaderProgram = nullptr;
	positionText = nullptr;
	vertexUV = nullptr;
	textProjectionMatrix = nullptr;
	textViewMatrix = nullptr;
	textModelMatrix = nullptr;
	texture = nullptr;
    
    
    resourcesReleased = true;
    needToRelease = false;
}


void OpenGLTextObject::drawOneTexturedRectangle(OpenGLContext &context, juce::Matrix3D<float> &model, juce::Matrix3D<float> &testView, juce::Matrix3D<float> &testPerspective, std::u16string stringToDraw[])
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
        // PAS NÉCESSAIRE À CHAQUE FRAME ????
        // PAS NÉCESSAIRE À CHAQUE FRAME ????
        computeVertices();
        // PAS NÉCESSAIRE À CHAQUE FRAME ????
        // PAS NÉCESSAIRE À CHAQUE FRAME ????
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
        
		textShaderProgram->use();
        
        // Uniforms qui sont liés au shader activé précédemment
		textModelMatrix->setMatrix4(model.mat, 1, false);
        textProjectionMatrix->setMatrix4(testPerspective.mat, 1, false);
        textViewMatrix->setMatrix4(testView.mat, 1, false);
        
        fontManager->GetFontTexture()->bind();
    
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

