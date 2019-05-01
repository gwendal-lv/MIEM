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

#include "MiemSamples.h"

namespace Miam
{

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
    
    // - - - - - common inter-thread shared values - - - - -
    std::mutex mainMutex;
    std::atomic<bool> isRecording;
    
    //  - - - - - to be used by the network thread - - - - -
    const std::chrono::time_point<MiemClock> startTimePt_NetworkThreadCopy;
    std::chrono::time_point<MiemClock> restartTimePoint;
    bool wasOscThreadRecording; ///< for thread-safe detection of the "recording started" event
    // - - - ........MIDI management processed inside Network/OSC thread........ - - -
    std::vector<int> lastMidiValues; ///< For storing the last forwarded values
    /// \brief The last time (since beginning of the experiment) that the last values were send
    std::vector<int64_t> lastMidiForwardingTimes_ms; // largement assez pour des années de millisecondes...
    /// \brief Toutes les 200ms, on autorise l'envoi en MIDI d'une valeur qui avait déjà
    /// été envoyée sur le port virtuel.
    ///
    /// \remark int64_t pour éviter des problèmes de cast dans
    const int64_t allowedUpdatePeriodForConstantMidiValue_ms = 200;
    
    
    //  - - - - - to be used by the Juce thread - - - - -
    const std::chrono::time_point<MiemClock> experimentStartTimePoint;
    std::vector<MiemExpeSample> bufferedSamples;
    /// \brief Lock-free queue to be read by the main Juce thread (the main recorder class)
    ///
    /// 100-by-100 matrix can be entirely stored within the queue without it being
    /// full.
    boost::lockfree::spsc_queue<MiemExpeSample, boost::lockfree::capacity<(1<<17)>> lastMiemSamples;
    
    
    // - - - - - to be used by the MIDI thread - - - - -
    const std::chrono::time_point<MiemClock> startTimePt_MidiThreadCopy;
    const int midiResolution = 128; // pas de MIDI CC sur 14 bits avec reaper learn...
    const int maxMidiParametersCount = 128; ///< 128 different control changes (no RPM or NRPM)
    /// \brief Lock-free queue to be read by the MIDI sending thread
    boost::lockfree::spsc_queue<MiemMidiSample, boost::lockfree::capacity<(1<<17)>> midiSamplesQueue;
    /// \brief Virtual midi device, for non-Windows only
    std::unique_ptr<MidiOutput> virtualMidiOutput;
    /// \brief Thread that is going to poll for midi message ; it will wait at a condition
    /// variable after finishing polling all messages.
    ///
    /// Its controlling thread must notify the cond var after each message pushed to
    /// the lock-free queue
    std::thread midiForwardThread;
    std::atomic<bool> continueForwardingMidi;
    std::condition_variable midiPoolConditionVariable; ///< Main cond var for re-activation forwarding
    std::mutex midiConditionVariableMutex;
    
    
    // ========================= METHODS =============================
    public :
    
    // ctor et dtor
    OSCListenerForwarder(int udpPort, std::chrono::time_point<MiemClock> _startTimePoint, std::string midiDeviceName);
    ~OSCListenerForwarder();
    
    /// \brief OSC listening (on the NETWORK THREAD)
    virtual void oscMessageReceived (const OSCMessage &message) override;
    
    /// \brief Main function-loop of MIDI forwarding thread
    void RunMidiSamplesForwarding();
    private :
    /// \brief To be called by the network/OSC thread
    void reinitMidiDuplicatesDetection();
    public :
    
    /// \brief last Messages reading and writing for sending to the manager
    // On the JUCE MESSAGE thread
    void StartRecording();
    void StopRecording();
    const std::vector<MiemExpeSample>& GetBufferedSamples();
};
    
}
