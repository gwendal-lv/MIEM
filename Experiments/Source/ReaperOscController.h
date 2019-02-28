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
    
    // ===================== ATTRIBUTES ========================
    private :
    
    OSCSender sender;
    OSCReceiver receiver;
    
    const int reaperUdpPort = 7071;
    const int listeningUdpPort = 7072;
    
    const char hostIp[100] = "127.0.0.1";
    
    const int tracksCount;
    
    std::vector<bool> tracksMuteState;
    // actually seems useless
    std::vector<bool> tracksMuteState_waitingForReaperResponse;
    
    
    // ===================== METHODS ========================
    public :
    ReaperOscController(int _tracksCount = 24);
    virtual ~ReaperOscController(){}
    
    virtual void oscMessageReceived (const OSCMessage &message) override;
    virtual void oscBundleReceived (const OSCBundle &) override;
    
    
    void RestartAndPlay(float tempo);
    void Stop();
    
    
    /// \brief Sets a track as "solo" by muting all other tracks.
    ///
    /// Set track -1 as solo to mute all tracks
    void SetTrackSolo_usingMutes(int trackNumber);
    
    
    protected :
    
    /// \brief Track number starting from 1 in Reaper
    void setTrackMuteState(int trackNumber, bool shouldBeMuted);
    
    private :
    void waitForMissingReaperResponses();

    
    void displayErrorAndThrowException(String errorStr);
    void sendMessageOrThrowException(OSCMessage& oscMessage);
    
    };
