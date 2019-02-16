/*
  ==============================================================================

    OSCRealtimeListener.h
    Created: 12 Feb 2019 10:57:17pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <atomic>
#include <chrono>
typedef std::chrono::steady_clock MiemClock;

#include "boost/lockfree/spsc_queue.hpp"

#include "JuceHeader.h"

#include "MiemExpePreset.h" // also: struct MiemSample

class OSCRecorder;


/// \brief Classe qui crée le récepteur OSC, et qui va elle-même fait le
/// transport lock-free des paramètres en le message thread de Juce et
/// le thread réseau temps-réel (qui servira à mesurer le temps le +
/// précisément possible, depuis cette machine)
class OSCRealtimeListener : OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{
    private :
    OSCReceiver oscReceiver;
    /// \brief
    ///
    /// 100-by-100 matrix can be entirely stored within the queue without it being
    /// full.
    boost::lockfree::spsc_queue<MiemSample, boost::lockfree::capacity<(1<<17)>> lastMiemSamples;
    
    // to be used by the network thread
    const std::chrono::time_point<MiemClock> experimentStartTimePoint;
    std::chrono::time_point<MiemClock> restartTimePoint;
    std::atomic<bool> isRecording;
    
    // to be used by the Juce thread
    std::vector<MiemSample> bufferedSamples;
    
    
    public :
    
    // ctor et dtor
    OSCRealtimeListener(int udpPort, std::chrono::time_point<MiemClock> _startTimePoint);
    ~OSCRealtimeListener() {}
    
    /// \brief OSC listening (on the NETWORK THREAD)
    virtual void oscMessageReceived (const OSCMessage &message) override;
    
    
    /// \brief last Messages reading and writing for sending to the manager
    // On the JUCE MESSAGE thread
    void StartRecording();
    void StopRecording();
    const std::vector<MiemSample>& GetBufferedSamples();
};
