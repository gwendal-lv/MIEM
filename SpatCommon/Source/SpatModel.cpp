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
PeriodicUpdateThread("SpatModel updater"), // base class

presenter(presenter_),
spatType(SpatType::None)
{
    updateThreadF_Hz = updateFrequency_Hz;
    updateThreadT_us = (int)std::round(1000000.0/updateFrequency_Hz);
    continueUpdate = false;
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
