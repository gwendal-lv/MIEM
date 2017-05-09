/*
  ==============================================================================

    This file was auto-generated!
 Then heavily modified.....

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
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
