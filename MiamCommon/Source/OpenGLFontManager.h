/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
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
        std::unique_ptr<OpenGLShaderProgram::Attribute> textPositionAttribute, textUVAttribute;
        std::unique_ptr<OpenGLShaderProgram::Uniform> textProjectionMatrixUniform, textViewMatrixUniform, textModelMatrixUniform, textureUniform, globalAlphaUniform;
        
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
        void initialiseAttributesAndUniforms(OpenGLContext &context);
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
