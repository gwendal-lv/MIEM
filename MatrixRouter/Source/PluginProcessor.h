/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include <memory>

#include "../JuceLibraryCode/JuceHeader.h"


#define MiamRouter_MaxBufferSize          16384
#define MiamRouter_TransitionFrequency    1000   // Hz

#include "AudioDefines.h"

#include "IModel.h"


namespace Miam {
    
    // Forward declarations
    class NetworkModel;
    class Presenter;
    
    
    
    //==============================================================================
    /**
     */
    class MatrixRouterAudioProcessor  : public AudioProcessor, public IModel
    {
        
        // ================== ATTRIBUTES ===================
    private:
        // - - - - - Links to other modules - - - - -
        /// Called from both the UI thread for save/load phases and UPD changes
        /// And the audio thread (lock-free message requested by the audio thread)
        std::shared_ptr<NetworkModel> networkModel;
        Presenter* presenter;
        
        // - - - - - Various buffers - - - - -
        /// /brief Buffer employed to backup audio data on a new block arrival
        ///
        /// 16384 is one of the biggest size ever found on the web (mozilla dev network)
        ///
        /// Optimization possible within the prepareToPlay function
        /// -> 1Mo buffer = we don't care on a decent system playing 64 channels
        float inputAudioData[JucePlugin_MaxNumInputChannels][MiamRouter_MaxBufferSize];
        /// \brief Buffer of an input only, intended to be modified
        AudioBuffer<float> currentInputBuffer;
        
        // - - - - - Processing parameters - - - - -
        /// \brief The main routing matrix
        ///
        /// The data type is "float" because Juce decided this for the buffer....
        float routingMatrix[JucePlugin_MaxNumInputChannels][JucePlugin_MaxNumInputChannels];
        /// \brief Contains the matrix coefficients before the update (i.e.
        /// the values to begin with when computing the volume ramps)
        float oldRoutingMatrix[JucePlugin_MaxNumInputChannels][JucePlugin_MaxNumInputChannels];
        /// \brief Indicates wether the concerned matrix coeff has just been updated
        bool newMatrixCoeff[JucePlugin_MaxNumInputChannels][JucePlugin_MaxNumInputChannels];
        
        
        // - - - - - Audio parameters for Automation - - - - -
        AudioParameterInt* udpPortAudioParam;
        
        // - - - - - Auxiliary attributes - - - - -
        // To detect and send changes to the Presenter
        int lastInputsCount = -1;
        int lastOutputsCount = -1;
        
        
        // ================== METHODS ===================
    public:
        
        //==============================================================================
        MatrixRouterAudioProcessor();
        ~MatrixRouterAudioProcessor();
        
        //==============================================================================
        void prepareToPlay (double sampleRate, int samplesPerBlock) override;
        void releaseResources() override;
        
#ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
        
        // - - - - - Functions executed on the audio thread - - - - -
    public :
        void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    private :
        /// \brief Gets all available Miam::AsyncParamChange data structures and
        /// interprets in terms of audio processor coefficients
        void processorAsyncUpdate();
        void processParamChange(AsyncParamChange& paramChange, bool notifyPresenter=false);
        /// \brief Auxiliary function
        void sendInputsOutputsCount();
    public :
        
        // - - - - - MIAM setters and getters - - - - -
    public :
        std::shared_ptr<NetworkModel> GetNetworkModel() {return networkModel;}
        
        
        //==============================================================================
        AudioProcessorEditor* createEditor() override;
        bool hasEditor() const override;
        
        //==============================================================================
        const String getName() const override;
        
        bool acceptsMidi() const override;
        bool producesMidi() const override;
        double getTailLengthSeconds() const override;
        
        //==============================================================================
        int getNumPrograms() override;
        int getCurrentProgram() override;
        void setCurrentProgram (int index) override;
        const String getProgramName (int index) override;
        void changeProgramName (int index, const String& newName) override;
        
        //==============================================================================
        void getStateInformation (MemoryBlock& destData) override;
        void setStateInformation (const void* data, int sizeInBytes) override;
        
        
    private:
        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixRouterAudioProcessor)
    };
    
} // namespace Miam


#endif  // PLUGINPROCESSOR_H_INCLUDED
