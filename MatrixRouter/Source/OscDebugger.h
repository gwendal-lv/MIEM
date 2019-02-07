/*
  ==============================================================================

    OscDebugger.h
    Created: 3 Jun 2017 6:17:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "AsyncParamChange.h"
#include "ModelDefines.h"


namespace Miam
{
    class OscDebugger : public OSCSender
    {
        public :
        
        OscDebugger()
        {
            if (! connect ("127.0.0.1", 9001))
                throw std::runtime_error ("Error: could not connect to UDP port 9001.");
        }
        
        void SendOscDebugPoint(int ptNum)
        {
            send("/miamDebugPoint", (int32_t)ptNum);
        }
        
        void SendParamChange(AsyncParamChange paramChange, DataOrigin origin, bool sendIds = true)
        {
            std::string oscAddress = "/";
            switch (origin)
            {
                case DataOrigin::Daw :
                    oscAddress += "DAW_envoie";
                    break;
                case DataOrigin::Presenter :
                    oscAddress += "PRESENTER_envoie";
                    break;
                case DataOrigin::NetworkModel :
                    oscAddress += "NETWORK_envoie";
                    break;
                case DataOrigin::PluginProcessorModel :
                    oscAddress += "PROCESSOR_envoie";
                    break;
                    
                default :
                    oscAddress += "origine_AUTRE";
            }
            
            if (sendIds)
                send(oscAddress.c_str(), (int32_t)paramChange.Id1, (int32_t)paramChange.Id2, (float)paramChange.FloatValue);
            else
                send(oscAddress.c_str(), (float)paramChange.FloatValue);
        }
    };
}
