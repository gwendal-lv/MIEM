/*
  ==============================================================================

    OSCRealtimeListener.cpp
    Created: 12 Feb 2019 10:57:17pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <iostream>
#include <thread>

#include "OSCRealtimeListener.h"

#include "OSCRecorder.h"


OSCRealtimeListener::OSCRealtimeListener(int udpPort)
:
oscReceiver("MIEM real-time OSC receiver thread")
{
    // added as real-time listener
    oscReceiver.addListener(this);
    
    if (! oscReceiver.connect(udpPort))
    {
        String errorStr = "Cannot open UDP socket for OSC receiving on port " + String(udpPort) + ". Please check parameters and restart experiment.";
        DBG("errorStr"); // won't be graphically displayed... console only
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
        throw std::runtime_error(errorStr.toStdString());
    }
    else
    {
        std::cout << "Listening for OSC message on UDP port " << udpPort << std::endl;
    }
}



void OSCRealtimeListener::oscMessageReceived (const OSCMessage &message)
{
    std::cout << "OSC MESSAGE SNIFFED à mettre en lock-free queue" << std::endl;
    
    MiemSample sample;
}
