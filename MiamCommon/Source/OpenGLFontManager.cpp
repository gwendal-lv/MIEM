/*
  ==============================================================================

    OpenGLFontManager.cpp
    Created: 9 Dec 2018 10:36:32pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "OpenGLFontManager.h"

using namespace Miam;

OpenGLFontManager::OpenGLFontManager()
{
    fontTextureImage = LoadImage("newFontImg_png");
}

void OpenGLFontManager::InitGLFontResources(OpenGLContext &context)
{
    fontTexture = std::make_unique<OpenGLTexture>();
    fontTexture->loadImage(fontTextureImage);
    
    initialiseShaderProgram(context);
    initialiseBuffers(context);
    initialiseAttributesAndUniforms(context);
}
void OpenGLFontManager::ReleaseGLFontResources()
{
    releaseAttributesAndUniforms();
    textShaderProgram->release();
    textShaderProgram = nullptr;
    
    // aucun release pour les VBOs ?
    
    fontTexture->release();
    fontTexture = nullptr;
}



void OpenGLFontManager::initialiseBuffers(OpenGLContext& context)
{
    context.extensions.glGenBuffers(1, &vertexBuffer);
    context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    context.extensions.glBufferData(GL_ARRAY_BUFFER,
                                    MIEM_GL_LABEL_MAX_CHARS * 6 * 3 * sizeof(GLfloat),
                                    nullptr, // non initialisé cette fois !
                                    GL_DYNAMIC_DRAW);
    
    context.extensions.glGenBuffers(1, &UVBuffer);
    context.extensions.glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    context.extensions.glBufferData(GL_ARRAY_BUFFER,
                                    MIEM_GL_LABEL_MAX_CHARS * 6 * 2 * sizeof(GLfloat),
                                    nullptr, // idem, non initialisé
                                    GL_DYNAMIC_DRAW);
}

void OpenGLFontManager::initialiseShaderProgram(OpenGLContext &context)
{
    /// - - - VERTEX shader - - -
    vertexShaderSourceCode =
    "attribute vec4 position;                                       \n"
    "attribute vec2 uvCoord;                                        \n" // envoyé par CPU (peut être adapté)
    
    "uniform mat4 modelMatrix;                                      \n"
    "uniform mat4 projectionMatrix;                                 \n"
    "uniform mat4 viewMatrix;                                       \n"
    
#if JUCE_OPENGL_ES // lowp seems reserved to embedded platforms
    "varying mediump vec2 UV;                                          \n"
#else
    "varying vec2 UV;                                               \n"
#endif
    
    "void main() "
    "{ "
    "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position; "
    "    UV = uvCoord; "
    "}                                                              \n";
    
    
    /// - - - FRAGMENT shader - - -
    fragmentShaderSourceCode =
#if JUCE_OPENGL_ES
    "precision mediump float;  " // valable pour tous les floats
    "varying mediump vec2 UV;                                          \n"
#else
    "varying vec2 UV;                                               \n"
#endif
    // Texture que l'on suppose en niveau de gris. Blanc = opaque, Noir = transparent
    "uniform sampler2D demoTexture;                                 \n" // nom à changer...
    "uniform float textGlobalAlpha;                                       \n"
    
    "void main() "
    "{ "
    // Déclaration variables ordre vec4 : xyzw correspond à RGBA
#if JUCE_OPENGL_ES
    "    mediump vec4 currentFrag; " // ...impossible d'appliquer une précision
    "    mediump vec4 shadowFrag;  " // par défaut aux vec4....
#else
    "    vec4 currentFrag; "
    "    vec4 shadowFrag;  "
#endif
    
    "    currentFrag = texture2D(demoTexture,UV); "
    // mais en fait : .rgba utilisables directement.... sucre syntaxique "swizzle"
    // traitement sur la texture : point gris => BLANC transparent
    "    if (currentFrag.r < 1.0) "
    "        currentFrag.a = currentFrag.r; " // valeur d'opacité était dans le niveau de gris
    "    else " // par sécurité... pour la suite de l'algo
    "        currentFrag.a = 1.0; " // valeur d'opacité était dans le niveau de gris
    "    currentFrag.rgb = vec3(1.0, 1.0, 1.0); " // blanc dans tous les cas
    
    // Rajout d'une ombre à +0.3% en UV (on se fait plaisir, dans le shader....)
    "    shadowFrag = texture2D(demoTexture, UV + vec2(-0.001, 0.003) ); "
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
    
    // Alpha global puis sortie du shader
    "    currentFrag.a = currentFrag.a * textGlobalAlpha; "
    "    gl_FragColor = currentFrag; " // gl_FragColor seems deprecated...  https://stackoverflow.com/questions/51459596/using-gl-fragcolor-vs-out-vec4-color
    "}                                                              \n";
    
    
    // - - - Compiling and registering shaders - - -
    textShaderProgram = std::make_unique<OpenGLShaderProgram>(context);
    
    // vérifier quand même que ça ne pose pas de soucis....
    textShaderProgram->addVertexShader(vertexShaderSourceCode);
    textShaderProgram->addFragmentShader(fragmentShaderSourceCode);
    
    textShaderProgram->link();
    
    // If we don't use() here, OpenGL crashes after...
    textShaderProgram->use();
}

void OpenGLFontManager::initialiseAttributesAndUniforms(OpenGLContext &context)
{
    OpenGLShaderProgram& shaderProgramRef = *(textShaderProgram.get());
    
    positionText = std::make_unique<OpenGLShaderProgram::Attribute>(shaderProgramRef, "position");
    vertexUV = std::make_unique<OpenGLShaderProgram::Attribute>(shaderProgramRef, "uvCoord");
    
    
    auto uniLocation = context.extensions.glGetUniformLocation (shaderProgramRef.getProgramID(), "textGlobalAlpha");
    std::cout << "Uniform Location = " << uniLocation << std::endl;
    
    
    textProjectionMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(shaderProgramRef, "projectionMatrix");
    uniLocation = context.extensions.glGetUniformLocation (shaderProgramRef.getProgramID(), "projectionMatrix");
    std::cout << "Uniform Location = " << uniLocation << std::endl;
    textViewMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(shaderProgramRef, "viewMatrix");
    uniLocation = context.extensions.glGetUniformLocation (shaderProgramRef.getProgramID(), "viewMatrix");
    std::cout << "Uniform Location = " << uniLocation << std::endl;
    textModelMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(shaderProgramRef, "modelMatrix");
    globalAlphaUniform = std::make_unique<OpenGLShaderProgram::Uniform>(shaderProgramRef, "textGlobalAlpha");

    texture = std::make_unique<OpenGLShaderProgram::Uniform>(shaderProgramRef, "demoTexture");
    
    if (texture != nullptr)
        texture->set(0);
    else
        assert(false); // cannot draw anything....
}
void OpenGLFontManager::releaseAttributesAndUniforms()
{
    globalAlphaUniform = nullptr;
    texture = nullptr;
    textModelMatrix = nullptr;
    textViewMatrix = nullptr;
    textProjectionMatrix = nullptr;
    vertexUV = nullptr;
    positionText = nullptr;
}



// - - - STATIC functions - - -

Image OpenGLFontManager::LoadImage(String path)
{
    Image returnImage;
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
        const void * srcData = BinaryData::getNamedResource(BinaryData::namedResourceList[resourceId],
                                                            dataSize);
        returnImage = resizeImageToPowerOfTwo(ImageCache::getFromMemory(srcData,dataSize));
    }
    else
        returnImage = resizeImageToPowerOfTwo(ImageFileFormat::loadFrom(File(path)));
    return returnImage;
}

Image OpenGLFontManager::resizeImageToPowerOfTwo(Image m_image)
{
    if (!(isPowerOfTwo(m_image.getWidth()) && isPowerOfTwo(m_image.getHeight())))
        return m_image.rescaled(jmin(1024, nextPowerOfTwo(m_image.getWidth())),
                                jmin(1024, nextPowerOfTwo(m_image.getHeight())));
    return m_image;
}
