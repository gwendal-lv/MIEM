/*
  ==============================================================================

    OpenGLFontManager.h
    Created: 9 Dec 2018 8:40:36pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

// To be used also in
#define MIEM_GL_LABEL_MAX_CHARS     (1024)

namespace Miam {
    
    class OpenGLTextObject;
    
    class OpenGLFontManager {
        
        // Text objects will have access-rights to the following attributes
        friend class Miam::OpenGLTextObject;
        
        // ======== ATTRIBUTES ========
        protected :
        
        private :
        Image fontTextureImage;
        std::unique_ptr<OpenGLTexture> fontTexture;
        
        // Shaders
        String vertexShaderSourceCode;
        String fragmentShaderSourceCode;
        std::unique_ptr<OpenGLShaderProgram> textShaderProgram;
        
        // All items concerning textures and images should be initialized only ONCE
        // at construction by the Juce Message thread.
        // Then, only the OpenGL thread will access it
        // (through thread-safe std:: shared pointers)
        public : // pour l'instant...
        std::unique_ptr<OpenGLShaderProgram::Attribute> positionText, colourText, vertexUV;
        std::unique_ptr<OpenGLShaderProgram::Uniform> textProjectionMatrix, textViewMatrix, textModelMatrix, texture;
        
        // VBOs
        GLuint vertexBuffer, UVBuffer;
        // const int maxCharacters = MIEM_GL_LABEL_MAX_CHARS; // useless now
        
        
        // ======== METHODS ========
        public :
        OpenGLFontManager();
        
        
        // - - - - - GL Init/Release management - - - - -
        void InitGLFontResources(OpenGLContext &context);
        void ReleaseGLFontResources();
        // - helpers -
        private :
        void initialiseShaderProgram(OpenGLContext &context);
        void initialiseAttributesAndUniforms();
        void releaseAttributesAndUniforms();
        void initialiseBuffers(OpenGLContext& context); 
        
        // - - - - - setters and getters - - - - -
        // disabled at the moment... friend class for better encapsulation
        /*
        Image GetFontImage() { return fontTextureImage; }
        OpenGLTexture* GetFontTexture() { return fontTexture.get(); }
        OpenGLShaderProgram* GetShaderProgram() { return textShaderProgram.get(); }
         */
        
        
        // - - - - - Static helper functions - - - - -
        static Image LoadImage(String pathOrName);
        static Image resizeImageToPowerOfTwo(Image m_image);
        
        
    };
    
}
