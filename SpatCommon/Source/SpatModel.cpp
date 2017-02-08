/*
  ==============================================================================

    SpatModel.cpp
    Created: 28 Nov 2016 11:02:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "JuceHeader.h"

#include "SpatModel.h"

#include "SpatInterpolator.h"

#include "IPresenter.h"


using namespace Miam;

// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
SpatModel::SpatModel(IPresenter* presenter_, double updateFrequency_Hz)
:
presenter(presenter_),
updateThreadF_Hz(updateFrequency_Hz),
updateThreadT_us((int)std::round(1000000.0/updateFrequency_Hz)),
continueUpdate(false),
updateThreadMeasurer("Model updater"),
spatType(SpatType::None)
{
    // Test of lockfreeness
    if (continueUpdate.is_lock_free())
        DBG("Atomic bool is lock free on this platform");
    else
        throw std::runtime_error("Atomic bool is not lock free on this platform");
    


    // Launch of thread, at the specified frequency
    //continueUpdate = true;
    // Using a c++11 lambda function for class member calling
	// Gros problème de polymorphisme non-fonctionnel sous VS2015 (voir header pour détails)
    //updateThread = std::thread( [this] {this->update();} );
}


SpatModel::~SpatModel()
{
}



// - - - - - Speakers management - - - - -
void SpatModel::AddSpeaker()
{
    std::shared_ptr<Speaker<double>> newSpeaker(new Speaker<double>());
    speakers.push_back(newSpeaker);
    spatInterpolator->AddSpeaker();
}
void SpatModel::RemoveSpeaker(size_t id_)
{
    spatInterpolator->RemoveSpeaker(id_);
    speakers.erase(speakers.begin()+id_);
}
