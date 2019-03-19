/*
  ==============================================================================

    OSCListenerForwarder.h
    Created: 12 Feb 2019 10:57:17pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include <memory>
typedef std::chrono::steady_clock MiemClock;

#include "boost/lockfree/spsc_queue.hpp"

#include "JuceHeader.h"

#include "MiemExpePreset.h" // also: struct MiemSample

class OSCRecorder;


/// \brief Classe qui crée le récepteur OSC, mais qui re-transmet aussi en MIDI
/// les infos après les avoir quantifiées correctement.
/// Classe qui va elle-même faire le
/// transport lock-free des paramètres entre le message thread de Juce et
/// le thread réseau temps-réel (qui servira à mesurer le temps le +
/// précisément possible, depuis cette machine)
///
/// \remark C'est cette classe qui fait la discrétisation/quantification des valeurs
/// OSC, puisque c'est elle qui transforme et transfère les messages MIDI sur 7 bits.
///
class OSCListenerForwarder : OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{
    // ========================= ATTRIBUTES =============================
    private :
    OSCReceiver oscReceiver;
    
    
    //  - - - - - to be used by the network thread - - - - -
    const std::chrono::time_point<MiemClock> experimentStartTimePoint;
    std::chrono::time_point<MiemClock> restartTimePoint;
    std::atomic<bool> isRecording;
    
    
    //  - - - - - to be used by the Juce thread - - - - -
    std::vector<MiemSample<float>> bufferedSamples;
    /// \brief Lock-free queue to be read by the main Juce thread (the main recorder class)
    ///
    /// 100-by-100 matrix can be entirely stored within the queue without it being
    /// full.
    boost::lockfree::spsc_queue<MiemSample<float>, boost::lockfree::capacity<(1<<17)>> lastMiemSamples;
    
    
    // - - - - - to be used by the MIDI thread - - - - -
    /// \brief Lock-free queue to be read by the MIDI sending thread
    const int midiResolution = 128; // pas de MIDI sur 14 bits avec reaper learn...
    boost::lockfree::spsc_queue<MiemSample<int>, boost::lockfree::capacity<(1<<17)>> midiSamplesQueue;
    /// \brief Virtual midi device, for non-Windows only
    std::unique_ptr<MidiOutput> virtualMidiOutput;
    /// \brief Thread that is going to poll for midi message ; it will wait at a condition
    /// variable after finishing polling all messages.
    ///
    /// Its controlling thread must notify the cond var after each message pushed to
    /// the lock-free queue
    std::thread midiForwardThread;
    std::atomic<bool> continueForwardingMidi;
    std::condition_variable midiPoolConditionVariable;
    std::mutex midiConditionVariableMutex;
    std::vector<int> lastParametersMidiValues; ///< For storing the last forwarded values
    std::vector<int> lastParametersForwardingTime_ms; ///< The last time that the last values were send
    /// \brief Toutes les 200ms, on autorise l'envoi en MIDI d'une valeur qui avait déjà
    /// été envoyée sur le port virtuel
    const int allowedUpdatePeriodForConstantMidiValue = 200;
    
    
    // ========================= METHODS =============================
    public :
    
    // ctor et dtor
    OSCListenerForwarder(int udpPort, std::chrono::time_point<MiemClock> _startTimePoint);
    ~OSCListenerForwarder() {}
    
    /// \brief OSC listening (on the NETWORK THREAD)
    virtual void oscMessageReceived (const OSCMessage &message) override;
    
    /// \brief Main function-loop of MIDI forwarding thread
    void RunMidiSamplesForwarding();
    
    /// \brief last Messages reading and writing for sending to the manager
    // On the JUCE MESSAGE thread
    void StartRecording();
    void StopRecording();
    const std::vector<MiemSample<float>>& GetBufferedSamples();
};
