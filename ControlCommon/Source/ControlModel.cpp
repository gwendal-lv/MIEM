/*
  ==============================================================================

    ControlModel.cpp
    Created: 28 Nov 2016 11:02:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "JuceHeader.h"

#include "ControlModel.h"
#include "MiamOscSender.hpp"

#include "ControlPresenter.h"


using namespace Miam;



// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
std::shared_ptr<ControlStateSender<double>> ControlModel::GetStateSender(size_t index)
{
    if ( index == 0 )
        return stateSenders[0];
    else
        throw std::logic_error("State sender #0 is the only one available at the moment.");
}

size_t ControlModel::GetOutputsCount()
{
    return interpolator->GetOutputsCount();
    
}




// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
ControlModel::ControlModel(ControlPresenter* presenter_, double updateFrequency_Hz)
:
PeriodicUpdateThread("Control Model updater"), // base class

presenter(presenter_),
commonStartTimePt( presenter->GetCommonTimePoint() )
{
    // Choice of interpolation type
    interpolator = std::make_shared<StatesInterpolator<double>>(InterpolationType::Matrix_ConstantVolumeInterpolation);
    
    // Définition de la fréquence de fonctionnement
    updateThreadF_Hz = updateFrequency_Hz;
    updateThreadT_us = (int)std::round(1000000.0/updateFrequency_Hz);
    continueUpdate = false;
    
    // Communication initialization
    stateSenders.push_back( std::make_shared<MiamOscSender<double>>() );
}


ControlModel::~ControlModel()
{
}



// = = = = = = = = Property tree (for XML) import/export = = = = = = = =
std::shared_ptr<bptree::ptree> ControlModel::GetConfigurationTree()
{
    // Interpolator general config
    auto configurationTree = interpolator->GetConfigurationTree();
    // Individual senders
    bptree::ptree senderChildren;
    for (size_t i=0 ; i<stateSenders.size() ; i++)
    {
        auto child = senderChildren.add_child( "sender", *(stateSenders[i]->GetConfigurationTree()) );
        child.put("<xmattr>.type", stateSenders[i]->GetTypeAsString());
    }
    configurationTree->add_child("senders", senderChildren);
    // Final return
    return configurationTree;
}
void ControlModel::SetConfigurationFromTree(bptree::ptree& tree)
{
    try {
        // Interpolator general config
        interpolator->SetConfigurationFromTree(tree);
        // Individual senders
        int sendersCount = 0; // exactly 1 sender allowed at the moment
        for (auto& sender : tree.get_child("senders"))
        {
            if (sendersCount >=1)
                throw XmlReadException("No more than 1 <sender> is allowed");
            
            stateSenders[sendersCount]->SetConfigurationFromTree(sender.second);
        }
    }
    catch (XmlReadException &e) {
        throw XmlReadException("Control settings are not correct: ", e);
    }
}


