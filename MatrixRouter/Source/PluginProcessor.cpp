/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <cmath>
#include <string>

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "NetworkModel.h"

#include "Presenter.h"


using namespace Miam;



//==============================================================================
MatrixRouterAudioProcessor::MatrixRouterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::mono(), true)
                     #endif
                       ),
#else
:
#endif
    currentInputBuffer(1, Miam_MaxBufferSize)
{
    // Init of the routing matrix
    for (int i=0 ; i<JucePlugin_MaxNumInputChannels ; i++)
    {
        for (int j=0 ; j<JucePlugin_MaxNumInputChannels ; j++)
        {
            routingMatrix[i][j] = 0.0f;
            oldRoutingMatrix[i][j] = 0.0f;
            newMatrixCoeff[i][j] = false;
        }
    }
    
    // Modules init afterwards self-init
    networkModel = new NetworkModel(*this);
    presenter = new Presenter(*this, *networkModel);
    
    // Creation of automatizable audio parameters for DAW communication
    addParameter(udpPortAudioParam = new AudioParameterInt("udpPort",
                                                           "UDP port for OSC",
                                                           -1,     // min
                                                           32768,  // max
                                                           7654)); // default
}

MatrixRouterAudioProcessor::~MatrixRouterAudioProcessor()
{
    delete presenter;
    delete networkModel;
}

//==============================================================================
const String MatrixRouterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MatrixRouterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MatrixRouterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double MatrixRouterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MatrixRouterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MatrixRouterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MatrixRouterAudioProcessor::setCurrentProgram (int index)
{
}

const String MatrixRouterAudioProcessor::getProgramName (int index)
{
    return String();
}

void MatrixRouterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MatrixRouterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Buffer size check (should be very useless)
    if (samplesPerBlock > Miam_MaxBufferSize)
        throw std::runtime_error("Current buffer size is too big for the MiamMatrixRouter plug-in. Please redure buffer size to " + std::to_string(Miam_MaxBufferSize) + " samples or less.");
}

void MatrixRouterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MatrixRouterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif



//==============================================================================
// =================== Functions executed on the audio thread ===================
//==============================================================================
void MatrixRouterAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int nSamples = buffer.getNumSamples();
    
    // Send of inputs and outputs number, if it has just changed
    if (totalNumInputChannels != lastInputsCount
        || totalNumOutputChannels != lastOutputsCount)
    {
        // transmission of information to the presenter
        AsyncParamChange msgToPresenter;
        msgToPresenter.Type = AsyncParamChange::ParamType::InputsAndOutputsCount;
        msgToPresenter.Id1 = totalNumInputChannels;
        msgToPresenter.Id2 = totalNumOutputChannels;
        SendParamChange(msgToPresenter);
        
        // Internal update
        lastInputsCount = totalNumInputChannels;
        lastOutputsCount = totalNumOutputChannels;
    }
    
    // New info may have arrived
    processorAsyncUpdate();
    
    // Backup of all of the original input audio data, into a local static buffer
    for (int i=0 ; i<totalNumInputChannels ; i++)
    {
        std::memcpy(inputAudioData[i], buffer.getReadPointer(i), nSamples*sizeof(float));
    }
    // Then : clear of all output buffers
    buffer.clear();
    
    // Matrix routing, sum input by input at the level
    for (int i = 0; i < totalNumInputChannels; i++)
    {
        bool currentInputBufferModified = true; // new input iteration

        // Cumulative sum (mixing/routing) for an output
        for (int j=0 ; j<totalNumOutputChannels ; j++)
        {
            // If the input has to be routed to the output
            if (routingMatrix[i][j] > (float)Miam_MinVolume
                || newMatrixCoeff[i][j])
            {
                // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                // - - - - - beginning of the actual routing matrix code - - - - -
                
                // Was the input buffer modifier during the processing of the last output ?
                
                if (currentInputBufferModified)
                {
                    float* inputBufRawPtr = currentInputBuffer.getWritePointer(0);
                    std::memcpy(inputBufRawPtr, inputAudioData[i], nSamples*sizeof(float));
                    currentInputBufferModified = false;
                }
                
                // Ramp to be applied, then actual addition
                if (newMatrixCoeff[i][j] && false)
                {
                    // Gain ramp parameters computing
                    double absVolumeDelta = std::abs((double)(routingMatrix[i][j] - oldRoutingMatrix[i][j]));
                    int rampNumSamples = (int) std::round(getSampleRate()*absVolumeDelta/(2.0*M_PI*Miam_TransitionFrequency));
                    
                    // Gain ramp computing
                    if (rampNumSamples < nSamples) // If ramp is short enough, but not
                    {
                        currentInputBuffer.applyGainRamp(0, 0, rampNumSamples, oldRoutingMatrix[i][j], routingMatrix[i][j]);
                        currentInputBuffer.applyGain(0, rampNumSamples, nSamples-rampNumSamples, routingMatrix[i][j]);
                    }
                    else // Else, ramp on the whole buffer
                        currentInputBuffer.applyGainRamp(0, 0, nSamples, oldRoutingMatrix[i][j], routingMatrix[i][j]);
                    // Temporary buffer necessarily modified at this point
                    currentInputBufferModified = true;
                    
                    // Mixing
                    buffer.addFrom(j, 0, currentInputBuffer, 0, 0, nSamples);
                    
                    // New becomes the old
                    oldRoutingMatrix[i][j] = routingMatrix[i][j];
                    newMatrixCoeff[i][j] = false;
                }
                // Simple gain only : direct Multiply and Add betwen audio buffers
                else
                {
                    
                    // Parameters : int destChannel, int destStartSample, const AudioBuffer &source, int sourceChannel, int sourceStartSample, int numSamples, float gainToApplyToSource = 1.0f
                    buffer.addFrom(j, 0, currentInputBuffer, 0, 0, nSamples,
                                   routingMatrix[i][j]);
                    
                }
                
                
                // - - - - - end of the actual routing matrix code - - - - -
                // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            }
        }
    }
}
void MatrixRouterAudioProcessor::processorAsyncUpdate()
{
    AsyncParamChange paramChange;
    
    // - - - - - At first, info from the Presenter - - - - -
    while (presenter->TryGetAsyncParamChange(paramChange))
        processParamChange(paramChange);
    
    // - - - - - Then, info from the network module of the model - - - - -
    // Such data will erase anything from the presenter, and the presenter will
    // even be lock-freely informed of the freshest data available
    while (networkModel->TryGetAsyncParamChange(paramChange))
        processParamChange(paramChange, true);
}
void MatrixRouterAudioProcessor::processParamChange(AsyncParamChange& paramChange, bool notifyPresenter)
{
    switch (paramChange.Type)
    {
        // Matrix coefficient
        case AsyncParamChange::Volume :
            // Security : any unadapted data makes the whole param change corrupted
            if (0 <= paramChange.Id1
                && paramChange.Id1 < JucePlugin_MaxNumInputChannels
                && 0 <= paramChange.Id2
                && paramChange.Id2 < JucePlugin_MaxNumOutputChannels
                && 0.0 <= paramChange.DoubleValue
                && paramChange.DoubleValue <= 2.0) // +6dB
            {
                routingMatrix[paramChange.Id1][paramChange.Id2] = (float)paramChange.DoubleValue;
                newMatrixCoeff[paramChange.Id1][paramChange.Id2] = true;
            }
            break;
            
        default :
            break;
    }
    
    if (notifyPresenter)
        TrySendParamChange(paramChange);
}
//==============================================================================
//==============================================================================
//==============================================================================




bool MatrixRouterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

// ATTENTION : l'éditeur peut être détruit n'importe quand et ne doit donc pas être stocké
AudioProcessorEditor* MatrixRouterAudioProcessor::createEditor()
{
    AudioProcessorEditor* newEditor = new MatrixRouterAudioProcessorEditor (*this, *presenter);
    return newEditor;
}

//==============================================================================
void MatrixRouterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Data backup towards the DAW
    MemoryOutputStream (destData, true).writeInt(*udpPortAudioParam);
}

void MatrixRouterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Data retrieval from the DAW
    *udpPortAudioParam = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readInt();
    
    // Updates then
    bool isUdpConnected = networkModel->SetUdpPort(*udpPortAudioParam);
    presenter->OnNewUdpPort(*udpPortAudioParam, isUdpConnected);
    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //  + MISE À JOUR MATRICE VIA UNE LOCK FREE QUEUE INTERNE MA GUEULE
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MatrixRouterAudioProcessor();
}
