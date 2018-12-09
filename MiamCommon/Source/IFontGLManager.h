/*
  ==============================================================================

    IFontGLManager.h
    Created: 9 Dec 2018 8:40:36pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace Miam {
    
    class IFontGLManager {
        
        
        // ======== ATTRIBUTES  (might not finally be a real abtract interface....) ========
        protected :
        
        Image fontTextureImage;
        
        
        
        // ======== METHODS  (might not finally be a real abtract interface....) ========
        public :
        
        virtual ~IFontGLManager()
        {}
        
        virtual Image* GetFontImagePtr() { return &fontTextureImage; }
        
        
    };
    
}
