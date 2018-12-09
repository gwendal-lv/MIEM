/*
  ==============================================================================

    OpenGLTextObject.h
    Created: 5 Jul 2018 11:38:16am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include <string>
#include <thread>
#include "JuceHeader.h"

#include "IFontGLManager.h"

namespace Miam {
    
    class OpenGLTextObject
    {
    public:
        OpenGLTextObject(String path, float _x, float _y, float _characterWidth, float _characterHeight, int _maxSize);
        ~OpenGLTextObject();

        static Image LoadImage(String pathOrName);
        static Image resizeImageToPowerOfTwo(Image m_image);
        
        void initialiseText(OpenGLContext& context);
        void drawOneTexturedRectangle(OpenGLContext &context, juce::Matrix3D<float> &model, juce::Matrix3D<float> &testView, juce::Matrix3D<float> &testPerspective, std::u16string stringToDraw[]);
        void release();
        void waitForOpenGLResourcesRealeased();
        
        void SetFontManager(IFontGLManager* _manager)
        { fontManager = _manager; }
        
        
    private:
        void computeVertices();
        void computeUV(int idx, char32_t character);
        void UTF16ToCodePoint(std::u16string::iterator &it, char32_t &currentCodePoint);
        void initialiseBuffer(OpenGLContext& context);
        void initialiseAttribute();
        void initialiseShaderProgram(OpenGLContext &context);

        float textX, textY, characterWidth, characterHeight;
        int maxSize;

        std::unique_ptr<OpenGLShaderProgram> textShaderProgram;

        GLuint vertexBuffer;
        GLfloat *g_vertex_buffer_data;//[6 * 3];

        GLuint UVBuffer;
        GLfloat *g_UV_buffer_data;//[6 * 2];

        
        // All items concerning textures and images should be initialized only ONCE
        // at construction by the Juce Message thread.
        // Then, only the OpenGL thread will access it
        // (through thread-safe std:: shared pointers)
        std::unique_ptr<OpenGLShaderProgram::Attribute> positionText, colourText, vertexUV;
        std::unique_ptr<OpenGLShaderProgram::Uniform> textProjectionMatrix, textViewMatrix, textModelMatrix, texture;
        std::shared_ptr<OpenGLTexture> textTexture;
        
        // If we copy-construct the item, the underlying data will not be allocated and copied
        // https://docs.juce.com/master/classImage.html#a5333c440b8f5637a3c7878e9b8d1ad21
        //juce::Image* image = nullptr; // image belongs to another manager class
        // IMAGE à demander directement au manager !!
        IFontGLManager* fontManager = nullptr;

        String myTextVertexShader;
        String myTextFragmentShader;


        void releaseResourcesSync();
        

        std::atomic<bool> needToRelease;
        std::atomic<bool> resourcesReleased;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLTextObject)
    };
}// namespace Miam

