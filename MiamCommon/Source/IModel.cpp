/*
  ==============================================================================

    IModel.cpp
    Created: 28 Nov 2016 11:02:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "IModel.h"



using namespace Miam;

// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
IModel::IModel(Presenter* _presenter) :
    spatType(SpatType::None)
{
    presenter = _presenter;
    
}


IModel::~IModel()
{
}



// - - - - - Speakers management - - - - -
void IModel::AddSpeaker()
{
    std::shared_ptr<Speaker<double>> newSpeaker(new Speaker<double>());
    speakers.push_back(newSpeaker);
    spatInterpolator->AddSpeaker();
}
void IModel::RemoveSpeaker(size_t _id)
{
    spatInterpolator->RemoveSpeaker(_id);
    speakers.erase(speakers.begin()+_id);
}
