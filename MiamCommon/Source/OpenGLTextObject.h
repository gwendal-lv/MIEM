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

namespace Miam {
    
    class OpenGLTextObject
    {
    public:
        OpenGLTextObject(float _x, float _y, float _characterWidth, float _characterHeight, int _maxSize);
        ~OpenGLTextObject();

        void initialiseText(OpenGLContext& context);
        /// \brief Releases critical GL resources. Must
        /// be called from the GL thread, is directly executed.
        void releaseResourcesSync();
        
        void drawOneTexturedRectangle(OpenGLContext &context, juce::Matrix3D<float> &model, juce::Matrix3D<float> &testView, juce::Matrix3D<float> &testPerspective, std::u16string stringToDraw[]);
        
        void SetFontManager(OpenGLFontManager* _manager)
        { fontManager = _manager; }
        
        
    private:
        void computeVertices();
        void computeUV(int idx, char32_t character);
        void UTF16ToCodePoint(std::u16string::iterator &it, char32_t &currentCodePoint);

        
        OpenGLFontManager* fontManager = nullptr;
        
        
        float textX, textY, characterWidth, characterHeight;
        int maxSize;


        GLfloat *g_vertex_buffer_data;//[6 * 3];

        GLfloat *g_UV_buffer_data;//[6 * 2];

        

        std::atomic<bool> needToRelease;
        std::atomic<bool> resourcesReleased;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLTextObject)
    };
}// namespace Miam

