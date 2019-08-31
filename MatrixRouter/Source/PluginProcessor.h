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

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED


#include <memory>

#include "../JuceLibraryCode/JuceHeader.h"

#include "ModelDefines.h"

#include "IModel.h"

#include "OscDebugger.h"


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
#ifdef __MIAM_DEBUG
    private :
        OscDebugger oscLocalhostDebugger;
    public :
        static OscDebugger* OscLocalhostDebugger;
    private:
#endif
        
        
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
        /// \brief Keeps track of the origin of all current coefficients of the
        /// routingMatrix
        DataOrigin matrixOrigin[JucePlugin_MaxNumInputChannels][JucePlugin_MaxNumInputChannels];
        /// \brief Indicates the remaining duration (in samples) of the volume gain ramp
        /// applied to a coefficient of the matrix
        ///
        /// 0 means that : routingMatrix[i][j] == oldRoutingMatrix[i][j]
        int remainingRampSamples[JucePlugin_MaxNumInputChannels][JucePlugin_MaxNumInputChannels];
        /// \brief Number of samples that corresponds to the following double variable
        ///
        /// Must be >= 1 to avoid bugs :
        /// if == 0, unable to detect if a volume ramp is happening or not
        int initialRampSamples;
        /// \brief Attack/Release time for any volume transition
        int rampDuration_ms;
        
        // - - - - - Audio parameters for Automation - - - - -
        
        /// \brief Automatizable data shared with the data
        ///
        /// Also the data that can be saved and loaded to and from the DAW.
        /// Can be updated at any time by the DAW without notification...
        AudioParameterFloat* dawRoutingMatrix[JucePlugin_MaxNumInputChannels*JucePlugin_MaxNumInputChannels];
        /// \brief Backup of the entire matrix from the DAW side....
        ///
        /// Copie entière nécessaire à chaque nouveau buffer audio traité : le DAW
        /// ne notifie pas le plug-in des changements
        /// Peut-être qu'en développant en VST / AU natif par OS on pourrait avoir des
        /// notifications...
        float dawMatrixBackup[JucePlugin_MaxNumInputChannels][JucePlugin_MaxNumInputChannels];
        
        /// \brief Corresponds to the "attack time" within the View module
        AudioParameterInt* dawRampDuration_ms;
        /// \brief Same as rampDuration_ms but from the DAW
        /// \brief To detect changes on the DAW side... (méthode archaïque)
        int dawRampDurationBackup_ms;
        
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
        // - - - - - Auxiliary function on "prepare to play" - - - - -
    private :
        void recomputeParameters(double sampleRate);
    public :
        
#ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
        
        // - - - - - Functions executed on the audio thread - - - - -
    public :
        void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    private :
        
        /// \brief Processes (or not, depending on the origin and priority) a param
        /// change coming from the specified source, and computes and launches a volume
        /// ramp is the param change concerns the matrix.
        void processParamChange(AsyncParamChange& paramChange, DataOrigin origin);
        
        /// \brief Auxiliary functions
        void sendInputsOutputsCount();
        void sendRampDuration();
        
        size_t idx(size_t i, size_t j) {return i*JucePlugin_MaxNumInputChannels+j;}
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
        /// \Brief Called from the DAW, it forces the state of the plugin.
        ///
        /// The DAW (Reaper/AU) sometimes loads nan values -> check for this, if nan
        /// values are given by the DAW -> default values loaded.
        void setStateInformation (const void* data, int sizeInBytes) override;
        
        
    private:
        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixRouterAudioProcessor)
    };
    
} // namespace Miam


#endif  // PLUGINPROCESSOR_H_INCLUDED
