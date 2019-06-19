/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

#include "Model.h"
#include "View.h"

#include "JuceHeader.h"


#include "AppPurpose.h"
#include "MiemDefaultSessions.h"

#include "AudioUtils.hpp"

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -
AppPurpose App::appPurpose = AppPurpose::Spatialisation;

Presenter::Presenter(View* _view) :
    PlayerPresenter(_view),

    view(_view),

    graphicSessionManager(this, _view)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    //view->GetMainContentComponent()->resized();
    view->OnNewVolumes(lastLowCorrelationVolume, lastHighCorrelationVolume);
    
    appModeChangeRequest(PlayerAppMode::Loading);
}
void Presenter::CompleteInitialisation(Model* _model)
{
    // - - - Init des attributs privés puis des parents - - -
    model = _model;
    
    PlayerPresenter::CompleteInitialisation(model);
}

void Presenter::OnLoadDefaultSession()
{
    LoadSession(DefaultSessions::GetDefaultSessionCode_mspat());
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =





// = = = = = = = = = = SPAT-specific events from View = = = = = = = = = =
void Presenter::OnMainSliderValueChanged_dB(double newValue_dB)
{
    AsyncParamChange gainChange;
    auto newValue = AudioUtils<double>::Amplitude_dB_to_Linear(newValue_dB);
    gainChange.DoubleValue = newValue;
    gainChange.Type = AsyncParamChange::ParamType::MasterVolume;
    SendParamChange(gainChange);
}




// = = = = = = = = = = PERIODIC UPDATES = = = = = = = = = =
void Presenter::processParamChangeFromModel(AsyncParamChange const & paramChange)
{
    bool wasVolumeUpdated = false;
    bool sendToParentClass = false; // most events should be treated here,
    // if the method if overriden...
    switch(paramChange.Type)
    {
        case AsyncParamChange::ParamType::Volume_CorrelatedInputs :
            lastHighCorrelationVolume = paramChange.DoubleValue;
            wasVolumeUpdated = true;
            break;
        case AsyncParamChange::ParamType::Volume_DecorrelatedInputs :
            lastLowCorrelationVolume = paramChange.DoubleValue;
            wasVolumeUpdated = true;
            break;
            
        // Ack : model is actually stopped
        case AsyncParamChange::ParamType::Stopped :
            lastLowCorrelationVolume = 0.0;
            lastHighCorrelationVolume = 0.0;
            wasVolumeUpdated = true;
            sendToParentClass = true;
            break;
        
        default :
            sendToParentClass = true;
            break;
    }

    if (wasVolumeUpdated)
        view->OnNewVolumes(lastLowCorrelationVolume, lastHighCorrelationVolume);
    
    if (sendToParentClass)
        PlayerPresenter::processParamChangeFromModel(paramChange);
}




// = = = = = = = = = = XML loading/Save = = = = = = = = = =
void Presenter::LoadSession(std::string filename, URL fileURL)
{
	// spat-only stops
	view->GetBackgroundComponent()->SetMainSliderVolume_dB(Miam_MinVolume_dB);

	// base-class loading
	PlayerPresenter::LoadSession(filename, fileURL);

	// spat-only restarts
	// WARNING : default volume is 0dB....
	view->GetBackgroundComponent()->SetMainSliderVolume_dB(0.0);
}

