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
        
        void SendParamChange(AsyncParamChange paramChange, DataOrigin origin)
        {
            std::string oscAddress = "/";
            switch (origin)
            {
                case DataOrigin::Daw :
                    oscAddress += "DAWenvoie";
                    break;
                case DataOrigin::Presenter :
                    oscAddress += "PRESENTERenvoie";
                    break;
                case DataOrigin::NetworkModel :
                    oscAddress += "NETWORKenvoie";
                    break;
                    
                default :
                    oscAddress += "origineAUTRE";
            }
            
            send(oscAddress.c_str(), (int32_t)paramChange.Id1, (int32_t)paramChange.Id2, (Float32)paramChange.FloatValue);
        }
    };
}
