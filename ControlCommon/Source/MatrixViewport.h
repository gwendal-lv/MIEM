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

// for external testing
//#include "../../MatrixRouter/JuceLibraryCode/JuceHeader.h"

#include "MatrixComponent.h" // includes LabelledMatrixComponent.h

//==============================================================================
/*
*/

namespace Miam
{
    
    class MatrixViewport    : public Viewport
    {
        
        // ======================= ATTRIBUTES ===========================
        private :
        
        // Parent and child
        LabelledMatrixComponent* const parent;
        MatrixComponent scrollableMatrixComponent;
        
        
        // ======================= METHODS ===========================
    public:
        
        // - - - - - Construction and Destruction - - - - -
        
        MatrixViewport(LabelledMatrixComponent* _parent,
                       int _maxRowsCount, int _maxColsCount)
        :
        parent(_parent),
        scrollableMatrixComponent(_parent, _maxRowsCount, _maxColsCount)
        {
            addAndMakeVisible(scrollableMatrixComponent);
            setViewedComponent(&scrollableMatrixComponent);
            setViewPosition(0, 0);
            
            // Shouldn't be necessary.... But macOS still brings scroll from
            // touchpad
            setScrollBarsShown(true, true, false, false);
        }
        
        ~MatrixViewport()
        {
        }
        
        
        // - - - - - Getters - - - - -
        /// \brief Direct access to the component, its sliders and their values
        MatrixComponent* GetMatrixComponent() {return &scrollableMatrixComponent;}
        
        
        // - - - - - Juce graphics - - - - -
        
        void paint (Graphics& /*g*/) override
        {
            // remplit le fond des barres de scroll
            //g.fillAll(Colours::black);
        }
        
        void resized() override
        {
            scrollableMatrixComponent.setBounds(getBounds());
        }
        
        // - - - - - Callbacks - - - - -

        void visibleAreaChanged (const Rectangle< int > & /*newVisibleArea*/) override
        {
            parent->OnViewportVisibleAreaChanged();
        }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixViewport)
    };
    
    
} // namespace Miam
