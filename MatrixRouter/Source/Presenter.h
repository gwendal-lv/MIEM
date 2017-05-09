/*
  ==============================================================================

    Presenter.h
    Created: 3 May 2017 3:16:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "IPresenter.h"

#include "PeriodicUpdateThread.h"

#include <cmath>

#include "JuceHeader.h"


namespace Miam {
    
    
    // Forward declarations
    class MatrixRouterAudioProcessor;
    class MatrixRouterAudioProcessorEditor;
    class OscMatrixComponent;
    
    
    /// \brief Base class of the PRESENTER module (see MVP scheme)
    class Presenter : public IPresenter
    {
        
        
        
        // ================== ATTRIBUTES ===================
        
        // - - - - - links to other modules - - - - -
        private :
        MatrixRouterAudioProcessor& model;
        
        // - - - - - graphical objects - - - - -
        /// \brief Kept within the presenter, sent to the ProcessorEditor (=View)
        /// when necessary (when recreated by the user) for the View to become its parent
        ScopedPointer<OscMatrixComponent> oscMatrixComponent;
        
        // - - - - - time management - - - - -
        const double updateFrequency_Hz = 15.0;
        const long updatePeriod_us = 66667; // rien de bien diabolique, correspond à 15Hz
        
        // ================== METHODS ===================
        
        // - - - - - Construction & destruction - - - - -
        public :
        Presenter(MatrixRouterAudioProcessor& _model);
        ~Presenter();
        
        // - - - - - Setters and Getters - - - - -
        OscMatrixComponent* GetOscMatrixComponent() {return oscMatrixComponent;}
        
        
        // - - - - - Periodic Update - - - - -
        const int GetUpdateFrequency_Hz() const {return (int)round(updateFrequency_Hz);}
        // Called on the Juce UI thread from View's Timer
        void UpdateFromView(MatrixRouterAudioProcessorEditor* view);
        // Override method that should not be called here (particular situation : fresh
        // view pointer necessary
        void Update() override;
        
    
        // - - - - - Callbacks from View - - - - -
        /// \brief Translates the data into a Miam::AsyncParamChange, then sends the data
        /// to the Model.
        void OnSliderValueChanged(int row, int col, double value);
        
        
    };
    
}
