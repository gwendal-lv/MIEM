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
    rampDuration_ms = MiamRouter_DefaultAttackTime_ms;
    
    dawRampDuration_ms = new AudioParameterInt("rampDuration_ms",
                                              "Attack time (ms)",
                                              1,     // min (defined in projucer)
                                              100,   // max (defined in projucer)
                                              MiamRouter_DefaultAttackTime_ms);
    addParameter(dawRampDuration_ms); // will auto-delete
    *dawRampDuration_ms = rampDuration_ms;
    dawRampDurationBackup_ms = -1; // initialized after construction
    sendRampDuration(); // update enqueued -> read when view is displayed !!!!
    
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
                                                              0.0f, // not Miam_MinVolume
                                                              Miam_MaxVolume, // 6dB
                                                              MiamRouter_DefaultVolume);
            addParameter(dawRoutingMatrix[idx(i,j)]);
            // They will be initialized within the setStateInformation(...) function
            
            remainingRampSamples[i][j] = 0;
            // Following matrix data to be initialized later too
            routingMatrix[i][j] = MiamRouter_DefaultVolume;
            matrixOrigin[i][j] = DataOrigin::InitialValue;
            oldRoutingMatrix[i][j] = MiamRouter_DefaultVolume;
            
            // Data that is Not initialized by force (but naturally within the processing)
            dawMatrixBackup[i][j] = MiamRouter_DefaultVolume;
        }
    }
    
    // Modules construction after self-init
    networkModel = std::make_shared<NetworkModel>(*this);
    presenter = new Presenter(*this, networkModel); // à l'ancienne
    
#ifdef __MIAM_DEBUG
    if (! oscLocalhostDebugger.connect ("127.0.0.1", 9001))
        throw std::runtime_error ("Error: could not connect to UDP port 9001.");
#endif
    
    // Now, the Presenter can start sending slider's values
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Activate;
    paramChange.Id1 = ActivateId::PresenterToModelParametersTransmission;
    SendParamChange(paramChange); // exception thrown if full (can't notify presenter to start....)
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
    initialRampSamples = (int) std::round((double)(rampDuration_ms)*sampleRate/1000.0);
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
    
    // - - - - - Plug-in automatizable parameters - - - - -
    if (*dawRampDuration_ms != dawRampDurationBackup_ms)
    {
////////////////////
        oscLocalhostDebugger.send("/nouvelle_rampe", (int32_t)*dawRampDuration_ms, (int32_t)dawRampDurationBackup_ms);
////////////////////
        // Update (simple pure erase of any other data)
        rampDuration_ms = *dawRampDuration_ms;
        dawRampDurationBackup_ms = rampDuration_ms;
        
        recomputeParameters(getSampleRate());
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
            dawMatrixBackup[i][j] = *dawRoutingMatrix[idx(i,j)];
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

                
                // Change notifications
                // To Presenter (if it didn't send it itself)
                if (origin != DataOrigin::Presenter)
                    TrySendParamChange(paramChange);
                // To DAW (if it didn't send it itself, AND if it's not the init phase)
                if (origin != DataOrigin::Daw)
                {
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
            //*udpPortAudioParam = paramChange.IntegerValue; // and not Id1 or Id2
            break;
        
        // Duration to be identified by Id
        case AsyncParamChange::Duration :
            if (paramChange.Id1 == DurationId::AttackTime)
            {
                rampDuration_ms = paramChange.IntegerValue;
                *dawRampDuration_ms = rampDuration_ms;
                dawRampDurationBackup_ms = rampDuration_ms; // to prevent update loops
                recomputeParameters(getSampleRate());
                ////////////////////
                oscLocalhostDebugger.send("/rampe_Presenter", (int32_t)rampDuration_ms);
                ////////////////////
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
    attackTimeChange.IntegerValue = rampDuration_ms;
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
    MemoryOutputStream memoryOutputStream(destData, true);
    
    // Data backup towards the DAW
    int udpPort = networkModel->GetUdpPort();
    memoryOutputStream.writeInt(udpPort);
    memoryOutputStream.writeInt(rampDuration_ms);
    
////////////////////
    oscLocalhostDebugger.send("/udp_sauvegarde", (int32_t)udpPort);
////////////////////
////////////////////
oscLocalhostDebugger.send("/rampe_sauvegardee", (int32_t)rampDuration_ms, (int32_t)*dawRampDuration_ms, (int32_t)dawRampDurationBackup_ms);
////////////////////
    
    // We write the actual most recent routing data
    for (int i=0 ; i<JucePlugin_MaxNumInputChannels ; i++)
        for (int j=0 ; j<JucePlugin_MaxNumOutputChannels ; j++)
            memoryOutputStream.writeFloat(routingMatrix[i][j]);
}

// Data retrieval from the DAW
void MatrixRouterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    MemoryInputStream memoryInputStream(data, static_cast<size_t> (sizeInBytes), false);
    AsyncParamChange paramChange;
    
    // filter Parameters at first
    // Updates directly after
    int udpPort = memoryInputStream.readInt();
    bool isUdpConnected = networkModel->SetUdpPort(udpPort);
    presenter->OnNewUdpPort(udpPort, isUdpConnected);
    paramChange.Type = AsyncParamChange::UdpPort;
    paramChange.IntegerValue = udpPort;
    SendParamChange(paramChange); // throws exception if full
    
////////////////////
    oscLocalhostDebugger.send("/udp_charge", (int32_t)udpPort);
////////////////////
    
////////////////////
    oscLocalhostDebugger.send("/rampe_avant_charge", (int32_t)rampDuration_ms, (int32_t)*dawRampDuration_ms, (int32_t)dawRampDurationBackup_ms);
////////////////////
    rampDuration_ms = memoryInputStream.readInt();
    if (std::isnan(rampDuration_ms) ) // necessary check !!
        rampDuration_ms = MiamRouter_DefaultAttackTime_ms;
    if (*dawRampDuration_ms != rampDuration_ms) // depends on the DAW
        *dawRampDuration_ms = rampDuration_ms;// automation forced update ENABLED
    // dawRampDurationBackup_ms = rampDuration_ms; // to force updates
    sendRampDuration(); // throws exception if full
    recomputeParameters(getSampleRate());

////////////////////
oscLocalhostDebugger.send("/rampe_chargee", (int32_t)rampDuration_ms, (int32_t)*dawRampDuration_ms, (int32_t)dawRampDurationBackup_ms);
////////////////////
    
    // Mise à jour des coefficients directement (après pour ne pas risquer de bourrer la
    // lock-free queue
    AsyncParamChange coeffUpdate; // pour info presenter
    coeffUpdate.Type = AsyncParamChange::Volume;
    for (int i=0 ; i<JucePlugin_MaxNumInputChannels ; i++)
    {
        coeffUpdate.Id1 = i;
        for (int j=0 ; j<JucePlugin_MaxNumOutputChannels ; j++)
        {
            routingMatrix[i][j] = memoryInputStream.readFloat(); // = automation data at this point ??
             // happened maybe because of the issue of reading int value as if they
            // were float values (only reading buffer start)
            if (std::isnan(routingMatrix[i][j]))
                routingMatrix[i][j] = MiamRouter_DefaultVolume;
            // Perfect equality testing
            if (*dawRoutingMatrix[idx(i, j)] != routingMatrix[i][j])
                *dawRoutingMatrix[idx(i, j)] = routingMatrix[i][j];
            
            matrixOrigin[i][j] = DataOrigin::Daw;
            remainingRampSamples[i][j] = 0;
            
            oldRoutingMatrix[i][j] = routingMatrix[i][j];
            dawMatrixBackup[i][j] = routingMatrix[i][j];
            
            // However, we do update the Presenter from the new data
            coeffUpdate.Id2 = j;
            coeffUpdate.FloatValue = routingMatrix[i][j];
            TrySendParamChange(coeffUpdate); // no exception thrown
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MatrixRouterAudioProcessor();
}
