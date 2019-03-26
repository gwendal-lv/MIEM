/*
  ==============================================================================

    ReaperOscController.h
    Created: 13 Feb 2019 5:35:34pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <vector>

#include "JuceHeader.h"

class ReaperOscController : public OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
    enum class TrackMuteState {
        Undefined = -1,
        
        Muted = 0,
        Unmuted,
    };
    
    // ===================== ATTRIBUTES ========================
    private :
    
    OSCSender sender;
    OSCReceiver receiver;
    
    const int reaperUdpPort = 7071;
    const int listeningUdpPort = 7072;
    
    const char hostIp[100] = "127.0.0.1";
    
    const int tracksCount;
    
    const int ForcedRefreshInterMessageDelay_ms = 100;
    std::vector<TrackMuteState> tracksMuteStates;
    // actually seems useless
    std::vector<bool> tracksMuteStates_waitingForReaperResponse;
    
    
    // ===================== METHODS ========================
    public :
    ReaperOscController(int _tracksCount);
    virtual ~ReaperOscController(){}
    
    virtual void oscMessageReceived (const OSCMessage &message) override;
    virtual void oscBundleReceived (const OSCBundle &) override;
    
    
    void RestartAndPlay(float tempo);
    void Stop();
    
    
    /// \brief Sets a track as "solo" by muting all other tracks.
    ///
    /// Set track -1 as solo to mute all tracks
    ///
    /// \remark Now uses a memory system to reduce the number of OSC messages
    /// sent to reaper. If the system is not used (forced resend), delays
    /// will be applied to sent message, because Reaper is easily drown
    /// in OSC messages....
    void SetTrackSolo_usingMutes(int trackNumber, bool forceResendAllMutes = false);
    
    
    protected :
    
    /// \brief Track number starting from 1 in Reaper
    void setTrackMuteState(int trackNumber, bool shouldBeMuted);
    
    private :
    void waitForMissingReaperResponses();

    
    void displayErrorAndThrowException(String errorStr);
    void sendMessageOrThrowException(OSCMessage& oscMessage);
    
    };
