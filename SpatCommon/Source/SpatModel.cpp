/*
  ==============================================================================

    SpatModel.cpp
    Created: 28 Nov 2016 11:02:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatModel.h"

#include "SpatInterpolator.h"


using namespace Miam;

// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
SpatModel::SpatModel(Presenter* _presenter) :
    spatType(SpatType::None)
{
    presenter = _presenter;
    
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
void SpatModel::RemoveSpeaker(size_t _id)
{
    spatInterpolator->RemoveSpeaker(_id);
    speakers.erase(speakers.begin()+_id);
}
