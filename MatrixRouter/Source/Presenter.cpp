/*
  ==============================================================================

    Presenter.cpp
    Created: 3 May 2017 3:16:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "Presenter.h"

#include "PluginProcessor.h" // class MatrixRouterAudioProcessor
#include "NetworkModel.h"

#include "OscMatrixComponent.h"

#include "MatrixSlider.h" // min and max volumes

#include "ModelDefines.h" // Enums for lock-free communication

using namespace Miam;




// =================== Construction & destruction ===================
Presenter::Presenter(MatrixRouterAudioProcessor& _model, NetworkModel& _networkModel)
    :
    model(_model), networkModel(_networkModel),
oscAddressCopy(networkModel.GetOscAddress()) // thread-safe at this point
{
    oscMatrixComponent = new OscMatrixComponent(this);
}

Presenter::~Presenter()
{
}


void Presenter::OnPluginEditorCreated(MatrixRouterAudioProcessorEditor* view)
{
    // Update request, row by row, every 5ms
    AsyncParamChange updateRequest;
    updateRequest.Type = AsyncParamChange::UpdateDisplay;
    for (int i=-1 ; i<JucePlugin_MaxNumInputChannels ; i++)
    {
        // CONVENTION : Id1 represents the row to update
        // EXCEPTION : -1 represents any other info about the matrix (in/out count, ...)
        updateRequest.Id1 = i;
        SendParamChange(updateRequest);
        // Not to charge too much the audio processor
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}



// =================== Periodic Update ===================

void Presenter::UpdateFromView(MatrixRouterAudioProcessorEditor* view)
{
    // Have new messages arrived ?
    AsyncParamChange newParamChange;
    double sliderValue_dB;
    while( model.TryGetAsyncParamChange(newParamChange) )
    {
        switch (newParamChange.Type)
        {
            case AsyncParamChange::Activate :
                if (newParamChange.Id1 == ActivateId::PresenterToModelParametersTransmission)
                    isModelReadyToReceive = true;
                break;
                
            case AsyncParamChange::InputsAndOutputsCount :
                oscMatrixComponent->SetActiveSliders(newParamChange.Id1, newParamChange.Id2);
                break;
                
            case AsyncParamChange::Duration :
                if (newParamChange.Id1 == DurationId::AttackTime) // Attack time
                    oscMatrixComponent->SetAttackSliderValue(newParamChange.DoubleValue);
                break;
                
            case AsyncParamChange::Volume :
                sliderValue_dB = Decibels::gainToDecibels(newParamChange.FloatValue);
                oscMatrixComponent->SetSliderValue(newParamChange.Id1, newParamChange.Id2, sliderValue_dB);
                break;
                
            default :
                break;
        }
    }
}


void Presenter::Update()
{
    throw std::runtime_error("Classic Update() from Presenter should not be called without specifying a View module (that may not exist when working with audio plug-ins");
}


// =================== Callbacks from View ===================

void Presenter::OnSliderValueChanged(int row, int col, double value)
{
    if (isModelReadyToReceive)
    {
        AsyncParamChange paramChange;
        paramChange.Type = AsyncParamChange::Volume;
        paramChange.Id1 = row;
        paramChange.Id2 = col;
        
        // We keep values over min+0.5dB only
        if (value < (MatrixSlider::GetMinVolume_dB() + 0.5))
            paramChange.FloatValue = 0.0f;
        else
            paramChange.FloatValue = (float)Decibels::decibelsToGain(value);
        
        // Enqueuing
        SendParamChange(paramChange);
    }
}
void Presenter::OnUdpPortChanged(int udpPort)
{
    // At this point : called from the Juce UI thread (so : OK), notifyModel=true
    bool isUdpConnected = networkModel.SetUdpPort(udpPort, true);
    // Self-update
    this->OnNewUdpPort(udpPort, isUdpConnected);
}
void Presenter::OnAttackDurationChanged(double attackDuration)
{
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Duration;
    paramChange.Id1 = 0; // ZERO means "attack duration"
    paramChange.DoubleValue = attackDuration;
    
    // Enqueuing
    SendParamChange(paramChange);
}


// =================== (possibly) Synchronous callbacks from Model ===================
void Presenter::OnNewUdpPort(int udpPort, bool isConnected)
{
    oscMatrixComponent->SetUdpPortAndMessage(udpPort, isConnected, oscAddressCopy);
}




