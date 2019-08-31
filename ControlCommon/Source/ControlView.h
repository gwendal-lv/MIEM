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
        
        /// \brief Returns whether the fullscreen state could be changed
        /// or not
        void SetFullscreenState(bool shouldBeFullscreen);
        bool GetFullscreenState();
        
        virtual void TriggerGLResourcesRelease() = 0;
        
    };
}
