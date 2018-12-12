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

#include "OpenGLFontManager.h"

#include "MiemVector.hpp"

namespace Miam {
    
    class OpenGLTextObject
    {
    public:
        OpenGLTextObject(float _x, float _y, float _characterWidth, float _characterHeight, int _maxSize);
        ~OpenGLTextObject();

        void Initialise(OpenGLContext& context, OpenGLFontManager* _manager);
        /// \brief Releases critical GL resources. Must
        /// be called from the GL thread, is directly executed.
        void releaseResourcesSync();
        
        void SetText(std::u16string& stringToDraw);
        void drawOneTexturedRectangle(OpenGLContext &context, juce::Matrix3D<float> &model, juce::Matrix3D<float> &testView, juce::Matrix3D<float> &testPerspective, GLfloat globalAlpha = 1.0f);
        
        void SetFontManager(OpenGLFontManager* _manager)
        { fontManager = _manager; }
        
        
    private:
        void computeVertices();
        void recomputeUV(int idx, char32_t character);
        
        void UTF16ToCodePoint(std::u16string::iterator &it, char32_t &currentCodePoint);
        float getCharWidthRatio(char32_t u32character);

        
        OpenGLFontManager* fontManager = nullptr;
        
        std::u16string currentText;
        
        // Parameters assigned at construction only
        const float textX, textY, characterWidth, characterHeight;
        const int maxSize;

        Vector<GLfloat> g_vertex_buffer;
        Vector<GLfloat> g_UV_buffer;

        
        bool resourcesReleased;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLTextObject)
    };
}// namespace Miam

