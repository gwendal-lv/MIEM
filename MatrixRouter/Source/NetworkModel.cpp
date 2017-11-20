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
NetworkModel::NetworkModel(MatrixRouterAudioProcessor& _model, std::string _oscAddress, std::string _oscZeroMatrixSuffix)
:
model(_model),
oscAddress(_oscAddress),
oscZeroMatrixSuffix(_oscZeroMatrixSuffix)
{
}

NetworkModel::~NetworkModel()
{
    
}

// =================== Setters and Getters ===================
bool NetworkModel::SetUdpPort(int _udpPort, bool notifyModel)
{
    if (_udpPort < 0 || _udpPort > 65535)
    {
        std::cout << "Port number " << _udpPort << "is not valid." << std::endl;
        return false;
    }
    
    removeListener(this);
    
    udpPort = _udpPort;
    
    disconnect();
    if (! connect (udpPort))
    {
        std::cout << "Error: could not connect to UDP port " << udpPort << std::endl;
        return false;
    }
    else
    {
        // Listener NOT AS A SHARED_PTR.... Hope this is OK.....
        // Juce documentation and forum do not give enough info...
        addListener(this);
        
        if (notifyModel)
        {
            AsyncParamChange udpChangeNotification;
            udpChangeNotification.Type = AsyncParamChange::UdpPort;
            udpChangeNotification.IntegerValue = udpPort;
            SendParamChange(udpChangeNotification);
        }
        
        return true;
    }
}



// =================== Asynchronous OSC processing ===================
void NetworkModel::oscMessageReceived(const OSCMessage& message)
{
    AsyncParamChange paramChange;
    
    // Unique coefficient (identified by row and col)
    if (message.getAddressPattern().matches(oscAddress.c_str()))
    {
        if (message.size() == 3
            && message[0].isInt32() && message[1].isInt32()
            && message[2].isFloat32())
        {
            paramChange.Type = AsyncParamChange::Volume; // parce qu'on a que ça....
            
            paramChange.Id1 = message[0].getInt32();
            paramChange.Id2 = message[1].getInt32();
            paramChange.FloatValue = message[2].getFloat32();
            
            SendParamChange(paramChange);
        }
    }
    // Zeroing of the whole matrix
    else if (message.getAddressPattern().matches(GetOscZeroMatrixAddress().c_str()))
    {
        paramChange.Type = AsyncParamChange::Reinitialize;
        SendParamChange(paramChange);
    }
}
