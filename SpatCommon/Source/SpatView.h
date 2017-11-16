/*
  ==============================================================================

    SpatView.h
    Created: 13 Nov 2017 2:40:41pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


namespace Miam
{
    class SpatView
    {
        
        // ========== ATTRIBUTES ==========
        protected :
        
        // The first created window
        DocumentWindow* mainWindow;

        
        
        // ========== METHODS ==========
        
        public :
        
        SpatView(DocumentWindow* mainWindow_) :
        mainWindow(mainWindow_)
        {}
        
        void SetTitle(std::string title);
        
    };
}
