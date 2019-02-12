/*
  ==============================================================================

    OSCRealtimeListener.h
    Created: 12 Feb 2019 10:57:17pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "MiemExpePreset.h"

class OSCRecorder;


/// \brief Classe qui crée le récepteur OSC, et qui va elle-même fait le
/// transport lock-free des paramètres en le message thread de Juce et
/// le thread réseau temps-réel (qui servira à mesurer le temps le +
/// précisément possible, depuis cette machine)
class OSCRealtimeListener : OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{
    private :
    OSCReceiver oscReceiver;
    
    
    public :
    
    // ctor et dtor
    OSCRealtimeListener(int udpPort);
    ~OSCRealtimeListener() {}
    
    // OSC listening
    virtual void oscMessageReceived (const OSCMessage &message) override;
    
};
