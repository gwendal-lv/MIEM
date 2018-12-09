/*
  ==============================================================================

    OpenGLFontManager.h
    Created: 9 Dec 2018 8:40:36pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace Miam {
    
    class OpenGLFontManager {
        
        
        // ======== ATTRIBUTES  (might not finally be a real abtract interface....) ========
        protected :
        
        private :
        Image fontTextureImage;
        std::unique_ptr<OpenGLTexture> fontTexture;
        
        
        
        // ======== METHODS  (might not finally be a real abtract interface....) ========
        public :
        OpenGLFontManager();
        
        
        // - - - - - GL Init/Release management - - - - -
        void InitGLFontResources();
        void ReleaseGLFontResources();
        
        // - - - - - setters and getters - - - - -
        Image GetFontImage() { return fontTextureImage; }
        OpenGLTexture* GetFontTexture() { return fontTexture.get(); }
        
        
        // - - - - - Static helper functions - - - - -
        static Image LoadImage(String pathOrName);
        static Image resizeImageToPowerOfTwo(Image m_image);
        
        
    };
    
}
