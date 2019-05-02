/*
  ==============================================================================

    MatrixViewport.h
    Created: 5 May 2017 1:55:12pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
