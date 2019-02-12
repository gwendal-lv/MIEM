/*
  ==============================================================================

    Presenter.cpp
    Created: 3 May 2017 3:16:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "Presenter.h"
#include "AppPurpose.h"

#include "PluginProcessor.h" // class MatrixRouterAudioProcessor
#include "NetworkModel.h"

#include "OscMatrixComponent.h"

#include "AudioUtils.hpp"

#include "MatrixSlider.h" // min and max volumes

#include "ModelDefines.h" // Enums for lock-free communication

#include "HelpContents.h"

using namespace Miam;



AppPurpose App::appPurpose = AppPurpose::Spatialisation;

// =================== Construction & destruction ===================
Presenter::Presenter(MatrixRouterAudioProcessor& _model, std::shared_ptr<NetworkModel> _networkModel)
	:
	model(_model), networkModel(_networkModel),
	oscAddressCopy(networkModel->GetOscAddress()) // thread-safe at this point
#ifdef __MIAM_DEBUG
	,
	oscLocalhostDebugger(9030)
#endif // MIAM_DEBUG
{
    oscMatrixComponent = new OscMatrixComponent(this);
    oscMatrixComponent->SetNetworkHelpContent(HelpContents::GetOscCommandsHelp());

	continueWakeUpPooling = true;
	wakeUpPoolingThread = std::thread([this] {this->poolNotifications(); });
}

Presenter::~Presenter()
{
	continueWakeUpPooling = false;
	if (wakeUpPoolingThread.joinable())
		wakeUpPoolingThread.join();
	else // we hope it is enough for the thread to end itself...
		std::this_thread::sleep_for(std::chrono::milliseconds(10 * poolPeriod_ms));
}


void Presenter::OnPluginEditorCreated(MatrixRouterAudioProcessorEditor* /*view*/)
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



// =================== Lock-free communication ===================
void Presenter::SendParamChange(AsyncParamChange& paramChange)
{
    //if (isModelReadyToReceive)
        LockFreeParamChangeSender::SendParamChange(paramChange);
    //else
        //throw std::runtime_error("Cannot send the parameter (model is not ready)");
}
bool Presenter::TrySendParamChange(AsyncParamChange& paramChange)
{
    if (isModelReadyToReceive)
        return LockFreeParamChangeSender::TrySendParamChange(paramChange);
    else
        return false;
}


// =================== Periodic Update ===================

void Presenter::UpdateFromView(MatrixRouterAudioProcessorEditor* /*view*/)
{
/*#ifdef __MIAM_DEBUG
	AsyncParamChange debugMsg;
	debugMsg.FloatValue = 222;
	oscLocalhostDebugger.SendParamChange(debugMsg, DataOrigin::Presenter);
#endif*/

    // Have new messages arrived ?
    AsyncParamChange newParamChange;
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
                //oscMatrixComponent->SetActiveSliders(JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels);
                break;
                
            case AsyncParamChange::Duration :
                if (newParamChange.Id1 == DurationId::AttackTime) // Attack time
                    oscMatrixComponent->SetAttackSliderValue( (double) newParamChange.IntegerValue);
                break;
                
            case AsyncParamChange::Volume :
                oscMatrixComponent->SetSliderValue(newParamChange.Id1, newParamChange.Id2, newParamChange.FloatValue);
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

void Presenter::poolNotifications()
{
	size_t poolsCounter = 0;
	while (continueWakeUpPooling)
	{
		if (mustWakeUp)
		{
			mustWakeUp = false;
			// Update from View, even if view does not exist !!
			MessageManager::callAsync( [this] { UpdateFromView(nullptr); } );
		}
		// auto-pools sometimes...
		else
		{
			poolsCounter++;
			if (poolsCounter > 50)
				mustWakeUp = true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(poolPeriod_ms));
	}
}
void Presenter::WakeUp()
{
	mustWakeUp = true;
}


// =================== Callbacks from View ===================

void Presenter::OnSliderValueChanged(int row, int col, double value)
{
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Volume;
    paramChange.Id1 = row;
    paramChange.Id2 = col;
    
    // We keep values over min+0.5dB only
	if (value < (MatrixSlider::GetMinVolume_dB() + 0.5))
		paramChange.FloatValue = 0.0f;
	else
		paramChange.FloatValue = (float) AudioUtils<double>::Amplitude_dB_to_Linear(value);
    

#ifdef __MIAM_DEBUG
	paramChange.FloatValue = (float)value;
	oscLocalhostDebugger.SendParamChange(paramChange, DataOrigin::Presenter);
	/*AsyncParamChange debugMsg;
	debugMsg.FloatValue = 11101;
	oscLocalhostDebugger.SendParamChange(debugMsg, DataOrigin::Presenter);*/
#endif

    // Enqueuing
    TrySendParamChange(paramChange);
}
void Presenter::OnMatrixZeroed()
{
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Reinitialize;
    // Enqueuing
    TrySendParamChange(paramChange);
}
void Presenter::OnUdpPortChanged(int udpPort)
{
    // At this point : called from the Juce UI thread (so : OK), notifyModel=false
    bool isUdpConnected = networkModel->SetUdpPort(udpPort, false);
    // Self-update
    this->OnNewUdpPort(udpPort, isUdpConnected);
}
void Presenter::OnAttackDurationChanged(double attackDuration)
{
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Duration;
    paramChange.Id1 = DurationId::AttackTime;
    paramChange.IntegerValue = (int) std::round(attackDuration);
    
    // Enqueuing
    TrySendParamChange(paramChange);
}


// =================== (possibly) Synchronous callbacks from Model ===================
void Presenter::OnNewUdpPort(int udpPort, bool isConnected)
{
    oscMatrixComponent->SetUdpPortAndMessage(udpPort, isConnected, oscAddressCopy);
}




