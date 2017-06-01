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
    currentInputBuffer(1, MiamRouter_MaxBufferSize)
{
    // Creation of automatizable audio parameters for DAW communication
    udpPortAudioParam = new AudioParameterInt("udpPort",
                                              "UDP port for OSC",
                                              -1,     // min
                                              32768,  // max
                                              7654);  // default
    addParameter(udpPortAudioParam); // will auto-delete
    rampDuration_ms = new AudioParameterFloat("rampDuration_ms",
                                              "Attack time (ms)",
                                              0.1,     // min (defined in projucer)
                                              100.0,   // max (defined in projucer)
                                              40.0);   // default
    addParameter(rampDuration_ms); // will auto-delete
    rampDurationBackup_ms = 0.0f; // initialized after construction
    
    // Init of the routing matrix + associated DAW parameters
    for (int i=0 ; i<JucePlugin_MaxNumInputChannels ; i++)
    {
        for (int j=0 ; j<JucePlugin_MaxNumInputChannels ; j++)
        {
            // Audio DAW parameter (nommage marche pour des coeffs de moins de 100)
            // name formatting
            std::string shortName, longName;
            if (i<10) // 1 digit : a zero will be added first
                shortName += "0";
            shortName += std::to_string(i);
            shortName += "_";
            if (j<10) // 1 digit : a zero will be added first
                shortName += "0";
            shortName += std::to_string(j);
            longName = "Matrix coeff. " + shortName;
            
            dawRoutingMatrix[idx(i,j)] = new AudioParameterFloat(shortName.c_str(),
                                                              longName,
                                                              0.0, // not Miam_MinVolume
                                                              Miam_MaxVolume, // 6dB
                                                              0.0);
            addParameter(dawRoutingMatrix[idx(i,j)]);
            // They will be initialized within the setStateInformation(...) function
            
            remainingRampSamples[i][j] = 0;
            // Following matrix data to be initialized later too
            routingMatrix[i][j] = 0.0f;
            matrixOrigin[i][j] = DataOrigin::InitialValue;
            oldRoutingMatrix[i][j] = 0.0f;
            
            // Data that is Not initialized by force (but naturally within the processing)
            dawMatrixBackup[i][j] = 0.0f;
        }
    }
    
    // Modules init after self-init
    networkModel = std::make_shared<NetworkModel>(*this);
    presenter = new Presenter(*this, *networkModel); // à l'ancienne
    
#ifdef __MIAM_DEBUG
    if (! oscLocalhostDebugger.connect ("127.0.0.1", 9001))
        throw std::runtime_error ("Error: could not connect to UDP port 9001.");
#endif
}

MatrixRouterAudioProcessor::~MatrixRouterAudioProcessor()
{
    // Modules deletion
    delete presenter;
    // network is a shared pointer...
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
    if (samplesPerBlock > MiamRouter_MaxBufferSize)
        throw std::runtime_error("Current buffer size is too big for the MiamMatrixRouter plug-in. Please redure buffer size to " + std::to_string(MiamRouter_MaxBufferSize) + " samples or less.");
    
    // ! samplePerBlocks is the expected maximum block size
    // BUT sample rate will remain constant
    recomputeParameters(sampleRate);
}

void MatrixRouterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void MatrixRouterAudioProcessor::recomputeParameters(double sampleRate)
{
    // must be >= 1
    initialRampSamples = (int) std::round((double)(*rampDuration_ms)*sampleRate/1000.0);
    if (initialRampSamples == 0)
        initialRampSamples = 1;
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
        //oscLocalhostDebugger.send("/miam/debug/processBlock0", (Float32)0.0);
    
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int nSamples = buffer.getNumSamples();
    
    
    // - - - - - Audio parameters updates - - - - -

    // Send of inputs and outputs number, if it has just changed
    if (totalNumInputChannels != lastInputsCount
        || totalNumOutputChannels != lastOutputsCount)
    {
        // Internal update
        lastInputsCount = totalNumInputChannels;
        lastOutputsCount = totalNumOutputChannels;
        
        // transmission of information to the presenter
        sendInputsOutputsCount();
    }
    
    // Plug-in automatizable parameters
    if (*rampDuration_ms != rampDurationBackup_ms)
    {
        // Update
        recomputeParameters(getSampleRate());
        rampDurationBackup_ms = *rampDuration_ms;
        // Notif to presenter
        sendRampDuration();
    }
    
    // - - - - - Routing data updates - - - - -
    
    AsyncParamChange paramChange;
    
    // 1st matrix update : info from the Presenter
    while (presenter->TryGetAsyncParamChange(paramChange))
        processParamChange(paramChange, DataOrigin::Presenter); // without notification
    
    // 2nd matrix update : info from the network module of the model
    // Such data will erase anything from others on this execution of the audio
    // thread, and the presenter will
    // be lock-freely informed of the freshest data available.
    // BUT during next frame, the Presenter and the DAW will be able to erase the data
    while (networkModel->TryGetAsyncParamChange(paramChange))
        processParamChange(paramChange, DataOrigin::NetworkModel);
    
    // 3rd matrix update: detection of changes on the DAW side (automation...) with
    // lowest priority
    for (int i = 0; i < totalNumInputChannels; i++)
    {
        for (int j=0 ; j < totalNumOutputChannels ; j++)
        {
            bool changeJustHappened = false;
            // At first : Scrutation for a change on the DAW side (since last buffer)
            // ONLY if the plug-in did not already received a command (OSC or GUI)
            // (else, the check wouldn't be necessary because the data will be
            // overriden at next "if" statement)
            if (remainingRampSamples[i][j] != initialRampSamples) // no command just before
            {
                // exact comparisons !
                // Sinon, grosse propagation des erreurs -> bug de détection de chgmt
                if ( *dawRoutingMatrix[idx(i,j)] != dawMatrixBackup[i][j] )
                {
                    oscLocalhostDebugger.send("/cgmtParamDaw", (int32_t)i, (int32_t)j, (Float32)*dawRoutingMatrix[idx(i,j)], (Float32)dawMatrixBackup[i][j]);
                    
                    // The DAW then takes control of the internal matrix :
                    // Creation of the async param change to be processed
                    paramChange.Type = AsyncParamChange::Volume;
                    paramChange.Id1 = i;
                    paramChange.Id2 = j;
                    paramChange.FloatValue = *dawRoutingMatrix[idx(i,j)];
                    // Actual Synchronous processing with Presenter notification
                    // Updates the internal routing matrix
                    processParamChange(paramChange, DataOrigin::Daw);
                    changeJustHappened = true;
                }
            }
            
            // Finally, backup of the whole matrix
            if (i == 0 && j == 0 && changeJustHappened)
                oscLocalhostDebugger.send("/AvantBackupDaw", (int32_t)i, (int32_t)j, (Float32)*dawRoutingMatrix[idx(i,j)], (Float32)dawMatrixBackup[i][j]);
            dawMatrixBackup[i][j] = *dawRoutingMatrix[idx(i,j)];
            if (i == 0 && j == 0 && changeJustHappened)
            oscLocalhostDebugger.send("/ApresBackupDaw", (int32_t)i, (int32_t)j, (Float32)*dawRoutingMatrix[idx(i,j)], (Float32)dawMatrixBackup[i][j]);
        }
    }
    
    
    
    // - - - - - Matrix routing, sum input by input at the level - - - - -
    
    // Backup of all of the original input audio data, into a local static buffer
    for (int i=0 ; i<totalNumInputChannels ; i++)
    {
        std::memcpy(inputAudioData[i], buffer.getReadPointer(i), nSamples*sizeof(float));
    }
    // Then : clear of all output buffers
    buffer.clear();
    
    for (int i = 0; i < totalNumInputChannels; i++)
    {
        bool currentInputBufferModified = true; // new input iteration

        // Cumulative sum (mixing/routing) for an output
        for (int j=0 ; j<totalNumOutputChannels ; j++)
        {
            // If the input has to be routed to the output :
            if (routingMatrix[i][j] > (float)Miam_MinVolume // non-negligible volume
                || remainingRampSamples[i][j] > 0) // or volume ramp currently applied
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
                
                // Ramp being applied, then actual addition
                if (remainingRampSamples[i][j] > 0)
                {
                    // Gain ramp computing
                    if (remainingRampSamples[i][j] < nSamples) // if ramp ends here
                    {
                        // RAMP (input buf modifier)
                        currentInputBuffer.applyGainRamp(0, 0, remainingRampSamples[i][j], oldRoutingMatrix[i][j], routingMatrix[i][j]);
                        currentInputBuffer.applyGain(0, remainingRampSamples[i][j], nSamples-remainingRampSamples[i][j], routingMatrix[i][j]);
                        currentInputBufferModified = true;
                        // MIXING then
                        buffer.addFrom(j, 0, currentInputBuffer, 0, 0, nSamples);
                        // This signifies the end of the coeff change
                        remainingRampSamples[i][j] = 0;
                        oldRoutingMatrix[i][j] = routingMatrix[i][j];
                    }
                    else // Else, ramp on the whole buffer
                    {
                        float tempRampVolume = oldRoutingMatrix[i][j]
                        + ((float)(nSamples-1.0)/(float)(remainingRampSamples[i][j]))
                                 * (routingMatrix[i][j] - oldRoutingMatrix[i][j]);
                        // Direct RAMP + MIXING
                        buffer.addFromWithRamp(j, 0, inputAudioData[i], nSamples, oldRoutingMatrix[i][j], tempRampVolume);
                        // For next buffer's first sample (so: it's not tempRampVolume)
                        oldRoutingMatrix[i][j] = oldRoutingMatrix[i][j]
                        + ((float)(nSamples)/(float)(remainingRampSamples[i][j]))
                        * (routingMatrix[i][j] - oldRoutingMatrix[i][j]);;
                        // Ramp params for next buffer
                        remainingRampSamples[i][j] -= nSamples;
                    }
                    
                }
                // Classical MIXING :
                // Simple gain only, direct Multiply and Add betwen audio buffers
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
int32_t updatesPasDuDawCompteur = 0;
void MatrixRouterAudioProcessor::processParamChange(AsyncParamChange& paramChange, DataOrigin origin)
{
    // For response to requests
    AsyncParamChange responseParam;
    
    // Main switch
    switch (paramChange.Type)
    {
        // Matrix coefficient
        case AsyncParamChange::Volume :
            // Security : any unadapted data makes the whole param change corrupted
            if (0 <= paramChange.Id1
                && paramChange.Id1 < JucePlugin_MaxNumInputChannels
                && 0 <= paramChange.Id2
                && paramChange.Id2 < JucePlugin_MaxNumOutputChannels
                && 0.0f <= paramChange.FloatValue
                && paramChange.FloatValue <= Miam_MaxVolume) // +6dB
            {
                routingMatrix[paramChange.Id1][paramChange.Id2] = paramChange.FloatValue;
                // no ramp on init values
                if (origin != DataOrigin::InitialValue)
                    remainingRampSamples[paramChange.Id1][paramChange.Id2] = initialRampSamples;
                else
                    remainingRampSamples[paramChange.Id1][paramChange.Id2] = 0;
                // We suppose here that the priorities by ORIGIN were respected
                // BEFORE calling this function
                matrixOrigin[paramChange.Id1][paramChange.Id2] = origin;

                /*
                if (origin == DataOrigin::Daw)
                {
                    oscLocalhostDebugger.send("/cgmtParamDaw", (int32_t)paramChange.Id1, (int32_t)paramChange.Id2, (Float32)*dawRoutingMatrix[idx(paramChange.Id1, paramChange.Id2)], (Float32)dawMatrixBackup[paramChange.Id1][paramChange.Id2]);
                }
                */

                
                // Change notifications
                // To Presenter (if it didn't send it itself)
                if (origin != DataOrigin::Presenter)
                    TrySendParamChange(paramChange);
                // To DAW (if it didn't send it itself, AND if it's not the init phase)
                if (origin != DataOrigin::Daw)
                {
                    if (origin == DataOrigin::Presenter)
                    {
                    updatesPasDuDawCompteur++;
                    oscLocalhostDebugger.send("/updatePresenter", updatesPasDuDawCompteur);
                    }
                    
                    *dawRoutingMatrix[idx(paramChange.Id1,paramChange.Id2)]
                        = routingMatrix[paramChange.Id1][paramChange.Id2];
                }
            }
            break;
            
        // Update request from the Presenter
        case AsyncParamChange::UpdateDisplay :
            // If it's a request for general parameters
            if (paramChange.Id1 == UpdateDisplayId::GeneralParameters)
            {
                sendInputsOutputsCount();
                sendRampDuration();
            }
            // Else, it can be a request for a specific row
            else if (0 <= paramChange.Id1 && paramChange.Id1 < JucePlugin_MaxNumInputChannels)
            {
                responseParam.Type = AsyncParamChange::Volume;
                responseParam.Id1 = paramChange.Id1;
                // We send the entire row
                for (int j=0 ; j<JucePlugin_MaxNumOutputChannels ; j++)
                {
                    responseParam.Id2 = j;
                    responseParam.FloatValue = routingMatrix[paramChange.Id1][j];
                    SendParamChange(responseParam); // and not Try, because the update was just asked for -> is it OK ?
                }
            }
            break;
            
        // Information (and not a request) of a new port
        case AsyncParamChange::UdpPort :
            *udpPortAudioParam = paramChange.IntegerValue; // and not Id1 or Id2
            break;
        
        // Duration to be identified by Id
        case AsyncParamChange::Duration :
            if (paramChange.Id1 == DurationId::AttackTime)
            {
                *rampDuration_ms = (float)paramChange.DoubleValue;
                rampDurationBackup_ms = *rampDuration_ms; // to prevent update loops
                recomputeParameters(getSampleRate());
            }
            break;
            
        default :
            break;
    }
}
void MatrixRouterAudioProcessor::sendInputsOutputsCount()
{
    AsyncParamChange msgToPresenter;
    msgToPresenter.Type = AsyncParamChange::ParamType::InputsAndOutputsCount;
    msgToPresenter.Id1 = lastInputsCount;
    msgToPresenter.Id2 = lastOutputsCount;
    TrySendParamChange(msgToPresenter); // View may not exist (Presenter not updated)
}
void MatrixRouterAudioProcessor::sendRampDuration()
{
    AsyncParamChange attackTimeChange;
    attackTimeChange.Type = AsyncParamChange::Duration;
    attackTimeChange.Id1 = DurationId::AttackTime;
    attackTimeChange.DoubleValue = (double) *rampDuration_ms;
    TrySendParamChange(attackTimeChange);
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
    MatrixRouterAudioProcessorEditor* newEditor = new MatrixRouterAudioProcessorEditor (*this, *presenter);
    
    // !!!!! blocking function !!!!!!
    presenter->OnPluginEditorCreated(newEditor);
    // !!!!! blocking function !!!!!!
    
    return newEditor;
}

//==============================================================================
void MatrixRouterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Data backup towards the DAW
    MemoryOutputStream (destData, true).writeInt(*udpPortAudioParam);
    MemoryOutputStream (destData, true).writeFloat(*rampDuration_ms);
    
    // We write the actual most recent routing data
    for (int i=0 ; i<JucePlugin_MaxNumInputChannels ; i++)
        for (int j=0 ; j<JucePlugin_MaxNumOutputChannels ; j++)
            MemoryOutputStream (destData, true).writeFloat(routingMatrix[i][j]);
     
}

// Data retrieval from the DAW
void MatrixRouterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
    // filter Parameters at first
    // Updates directly after
    AsyncParamChange paramChange;
    *udpPortAudioParam = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readInt();
    bool isUdpConnected = networkModel->SetUdpPort(*udpPortAudioParam);
    presenter->OnNewUdpPort(*udpPortAudioParam, isUdpConnected);
    
    *rampDuration_ms = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    paramChange.Type = AsyncParamChange::Duration;
    paramChange.Id1 = 0; // means attack time
    paramChange.DoubleValue = (double) *rampDuration_ms;
    
    recomputeParameters(getSampleRate());
    
    // Mise à jour des coefficients directement
    
    AsyncParamChange coeffUpdate; // pour info presenter
    coeffUpdate.Type = AsyncParamChange::Volume;
    for (int i=0 ; i<JucePlugin_MaxNumInputChannels ; i++)
    {
        coeffUpdate.Id1 = i;
        for (int j=0 ; j<JucePlugin_MaxNumOutputChannels ; j++)
        {
            routingMatrix[i][j] = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat(); // = automation data at this point
            //*dawRoutingMatrix[idx(i, j)] = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat(); // != automation data at this point !!
            matrixOrigin[i][j] = DataOrigin::Daw;
            remainingRampSamples[i][j] = 0;
            
            oldRoutingMatrix[i][j] = routingMatrix[i][j];
            dawMatrixBackup[i][j] = routingMatrix[i][j];
            
            // However, we do update the Presenter from the new data
            coeffUpdate.Id2 = j;
            coeffUpdate.FloatValue = routingMatrix[i][j];
            TrySendParamChange(coeffUpdate);
        }
    }
    
    oscLocalhostDebugger.send("/miam/debug/setStateInfoFinished", (Float32)0.0);
    // Now, the Presenter can start sending slider's values
    paramChange.Type = AsyncParamChange::Activate;
    paramChange.Id1 = ActivateId::PresenterToModelParametersTransmission;
    SendParamChange(paramChange); // exception thrown if full (can't notify presenter to start....)
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MatrixRouterAudioProcessor();
}
