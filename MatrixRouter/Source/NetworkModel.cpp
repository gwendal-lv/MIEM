/*
  ==============================================================================

    NetworkModel.cpp
    Created: 9 May 2017 4:50:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <string>

#include "NetworkModel.h"

#include "PluginProcessor.h" // main model

using namespace Miam;




// =================== Construction & destruction ===================
NetworkModel::NetworkModel(MatrixRouterAudioProcessor& _model)
:
model(_model)
{
}

NetworkModel::~NetworkModel()
{
    
}

// =================== Setters and Getters ===================
bool NetworkModel::SetUdpPort(int _udpPort)
{
    removeListener(this);
    
    disconnect();
    if (! connect (_udpPort))
    {
        std::cout << "Error: could not connect to UDP port " << _udpPort << std::endl;
        return false;
    }
    else
    {
        addListener(this);
        return true;
    }
}


// =================== Asynchronous OSC processing ===================
void NetworkModel::oscMessageReceived(const OSCMessage& message)
{
    if (message.getAddressPattern().matches("/miam/matrix"))
    {
        if (message.size() == 3
            && message[0].isInt32() && message[1].isInt32()
            && message[2].isFloat32())
        {
            AsyncParamChange paramChange;
            paramChange.Type = AsyncParamChange::Volume; // parce qu'on a que ça....
            
            paramChange.Id1 = message[0].getInt32();
            paramChange.Id2 = message[1].getInt32();
            paramChange.DoubleValue = (double) message[2].getFloat32();
            
            SendParamChange(paramChange);
        }
    }
}
