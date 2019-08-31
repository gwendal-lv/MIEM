/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
