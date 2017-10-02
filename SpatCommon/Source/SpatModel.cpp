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
#include "MiamOscSender.hpp"

#include "IPresenter.h"


using namespace Miam;

// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
SpatModel::SpatModel(IPresenter* presenter_, double updateFrequency_Hz)
:
PeriodicUpdateThread("SpatModel updater"), // base class

presenter(presenter_)
{
    updateThreadF_Hz = updateFrequency_Hz;
    updateThreadT_us = (int)std::round(1000000.0/updateFrequency_Hz);
    continueUpdate = false;
    
    // Communication initialization
    spatSenders.push_back( std::make_shared<MiamOscSender<double>>() );
}


SpatModel::~SpatModel()
{
}



// - - - - - Speakers management - - - - -
void SpatModel::AddSpeaker()
{
    std::shared_ptr<Speaker<double>> newSpeaker(new Speaker<double>());
    speakers.push_back(newSpeaker);
    std::logic_error("not fully implemented");
    //spatInterpolator->AddSpeaker();
}
void SpatModel::RemoveSpeaker(size_t id_)
{
    std::logic_error("not fully implemented");
    //spatInterpolator->RemoveSpeaker(id_);
    speakers.erase(speakers.begin()+id_);
}



// = = = = = = = = Property tree (for XML) import/export = = = = = = = =
std::shared_ptr<bptree::ptree> SpatModel::GetConfigurationTree()
{
    // Interpolator general config
    auto configurationTree = spatInterpolator->GetConfigurationTree();
    // Individual spat senders
    bptree::ptree senderChildren;
    for (size_t i=0 ; i<spatSenders.size() ; i++)
    {
        auto child = senderChildren.add_child( "sender", *(spatSenders[i]->GetConfigurationTree()) );
        child.put("<xmattr>.type", spatSenders[i]->GetTypeAsString());
    }
    configurationTree->add_child("senders", senderChildren);
    // Final return
    return configurationTree;
}
void SpatModel::SetConfigurationFromTree(bptree::ptree& tree)
{
    try {
        // Interpolator general config
        spatInterpolator->SetConfigurationFromTree(tree);
        // Individual spat senders
        int spatSendersCount = 0; // exactly 1 spat sender allowed at the moment
        for (auto& sender : tree.get_child("senders"))
        {
            if (spatSendersCount >=1)
                throw XmlReadException("No more than 1 <sender> is allowed");
            
            spatSenders[spatSendersCount]->SetConfigurationFromTree(sender.second);
        }
    }
    catch (XmlReadException &e) {
        throw XmlReadException("Spat settings are not correct: ", e);
    }
}


