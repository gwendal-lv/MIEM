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

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "OscMatrixComponent.h"


namespace Miam {
    
    
    // Forward declarations
    class Presenter;
    
    
    /// \brief Base class of the VIEW module of the plugin
    class MatrixRouterAudioProcessorEditor  : public AudioProcessorEditor, private Timer
    {
        
        
        
        // ======================= ATTRIBUTES ===========================
    private:
        
        // This reference is provided as a quick way for your editor to
        // access the processor object that created it.
        // (dégagé pour respecter le MVP)
        // MatrixRouterAudioProcessor& processor;
        
        
        // - - - - - Links to other modules - - - - -
        Presenter& presenter;
        
        
        
        // - - - - - Children components - - - - -
        OscMatrixComponent* oscMatrixComponent; // owned by the Presenter
        
        
        
        
        // ======================= METHODS ===========================
        // - - - - - Constructor and destructor - - - - -
    public:
        MatrixRouterAudioProcessorEditor (MatrixRouterAudioProcessor&, Presenter&);
        ~MatrixRouterAudioProcessorEditor();
        
        
        // - - - - - Juce Graphics - - - - -
        void paint (Graphics&) override;
        void resized() override;
        
        
        // - - - - - Callbacks - - - - -
        // No editor -> no update forced on the Presenter module
        void timerCallback() override;
        
        
        // truc
    private :
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixRouterAudioProcessorEditor)
    };
    
} // namespace Miam


#endif  // PLUGINEDITOR_H_INCLUDED
