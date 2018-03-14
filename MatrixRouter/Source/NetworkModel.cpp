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

#include "boost/endian/conversion.hpp"

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
    
    // Coefficients (identified by row and col)
    if (message.getAddressPattern().matches(oscAddress.c_str()))
    {
        // Unique coefficient
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
        // 2 coeffs or more
        else if (message.size() == 1 && message[0].isBlob())
        {
            paramChange.Type = AsyncParamChange::Volume; // parce qu'on a que ça....
            
            const MemoryBlock& oscMemoryBlock = message[0].getBlob();
            int32_t bigEndianInt;
            // Lecture du nombre de coeffs contenus
            // Check de la taille du bloc ? -> pas nécessaire.... Code interne pour l'instant seulement !
            oscMemoryBlock.copyTo(&bigEndianInt, 0, sizeof(int32_t));
            size_t totalCoeffsCount = (size_t) boost::endian::big_to_native<int32_t>(bigEndianInt);
            // Lecture des triplets int/int/float et transmission lock-free directe + envoi
            for (size_t i=0 ; i<totalCoeffsCount ; i++)
            {
                size_t currentCoeffOffset = sizeof(int32_t) * (1 + 2*i) + sizeof(Float32)*i;
                
                oscMemoryBlock.copyTo(&bigEndianInt, (int) currentCoeffOffset, sizeof(int32_t));
                currentCoeffOffset += sizeof(int32_t);
                paramChange.Id1 = (int) boost::endian::big_to_native<int32_t>(bigEndianInt);
                oscMemoryBlock.copyTo(&bigEndianInt, (int) currentCoeffOffset, sizeof(int32_t));
                currentCoeffOffset += sizeof(int32_t);
                paramChange.Id2 = (int) boost::endian::big_to_native<int32_t>(bigEndianInt);
                
                Float32 floatValue;
                oscMemoryBlock.copyTo(&floatValue, (int) currentCoeffOffset, sizeof(Float32));
                currentCoeffOffset += sizeof(Float32);
                paramChange.FloatValue = (float) floatValue;
                
                SendParamChange(paramChange);
            }
        }
    }
    // Zeroing of the whole matrix
    else if (message.getAddressPattern().matches(GetOscZeroMatrixAddress().c_str()))
    {
        paramChange.Type = AsyncParamChange::Reinitialize;
        SendParamChange(paramChange);
    }
}




