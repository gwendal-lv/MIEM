/*
  ==============================================================================

    ControlView.h
    Created: 13 Nov 2017 2:40:41pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


namespace Miam
{
    class ControlView
    {
        
        // ========== ATTRIBUTES ==========
        protected :
        
        // The first created window
        DocumentWindow* mainWindow;

        
        
        // ========== METHODS ==========
        
        public :
        
        ControlView(DocumentWindow* mainWindow_) :
        mainWindow(mainWindow_)
        {}
        
        virtual ~ControlView() {}
        
        virtual void SetTitle(std::string title);
        
    };
}
