/*
  ==============================================================================

    ReaperOscController.cpp
    Created: 13 Feb 2019 5:35:34pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <thread>
#include <cmath>

#include "boost/lexical_cast.hpp"

#include "ReaperOscController.h"

#include "OSCRecorder.h"

#include "MonitorCommunication.h"


ReaperOscController::ReaperOscController(int _tracksCount)
:
tracksCount(_tracksCount)
{
    // resize of vectors
    tracksMuteState.resize(tracksCount);
    tracksMuteState_waitingForReaperResponse.resize(tracksCount);
    for (int i=0 ; i<tracksCount ; i++)
    {
        tracksMuteState[i] = false; // to be re-set properly
        tracksMuteState_waitingForReaperResponse[i] = false; // nothing asked yet
    }
    
    // Sender setup
    if (! sender.connect(hostIp, reaperUdpPort))
        displayErrorAndThrowException(TRANS("Cannot connect OSC sender to Reaper at ") + String(hostIp) + ":" + String(reaperUdpPort) + TRANS(". Please check parameters."));
    
    // receiver : does not need to be real-time. The UI can wait a bit for REAPER's responses
    if (! receiver.connect(listeningUdpPort))
        displayErrorAndThrowException(TRANS("Cannot connect OSC receiver from Reaper on port ") +  String(listeningUdpPort) + TRANS(". Please check parameters."));
    
    receiver.addListener(this);
}



void ReaperOscController::oscMessageReceived (const OSCMessage &message)
{
    //std::cout << "NORMAL OSC msg received from REAPER" << std::endl;
    //assert(false); // no treated... REAPER seems to send bundles only
}
void ReaperOscController::oscBundleReceived (const OSCBundle & oscBundle)
{
    //std::cout << "Reaper BUNDLE received" << std::endl;
}



void ReaperOscController::RestartAndPlay(float tempo)
{
    MonitorCommunication::SendLog("[OSC -> REAPER]: REPLAY, TEMPO = "
                                  + boost::lexical_cast<std::string>(tempo));
    
    // TEMPO set before playing
    String oscAddress = "/tempo/raw";
    OSCMessage oscMsgTempo = OSCMessage(OSCAddressPattern(oscAddress));
    oscMsgTempo.addFloat32(tempo);
    sendMessageOrThrowException(oscMsgTempo);
    
    // Délimitation de la boucle (à recalculer pour 16*4 temps, selon tempo)
    oscAddress = "/loop/start/time";
    OSCMessage oscMsgLoop1 = OSCMessage(OSCAddressPattern(oscAddress));
    oscMsgLoop1.addFloat32(0.0);
    sendMessageOrThrowException(oscMsgLoop1);
    float loopLength_s = (float)(16 * 4) / (tempo / 60.0f) ;
    oscAddress = "/loop/end/time";
    OSCMessage oscMsgLoop2 = OSCMessage(OSCAddressPattern(oscAddress));
    oscMsgLoop2.addFloat32(loopLength_s);
    sendMessageOrThrowException(oscMsgLoop2);
    
    // Then : RESTART
    oscAddress = "/time";
    OSCMessage oscMsgTime = OSCMessage(OSCAddressPattern(oscAddress));
    oscMsgTime.addFloat32(0.0f); // works ?
    sendMessageOrThrowException(oscMsgTime);
    
    // DE-MUTE après avoir placé la tête de lecture
    oscAddress = "/master/volume";
    OSCMessage oscVolumeMsg = OSCMessage(OSCAddressPattern(oscAddress));
    oscVolumeMsg.addFloat32(0.715f);
    sendMessageOrThrowException(oscVolumeMsg);
    
    // At last : PLAY
    oscAddress = "/play";
    OSCMessage oscTriggerMsg = OSCMessage(OSCAddressPattern(oscAddress));
    oscTriggerMsg.addInt32(1); // trigger forced to 1 (not a real trigger then...)
    sendMessageOrThrowException(oscTriggerMsg);
}
void ReaperOscController::Stop()
{
    MonitorCommunication::SendLog("[OSC -> REAPER]: STOP");
    
    // STOP
    String oscAddress = "/stop";
    OSCMessage oscTriggerMsg = OSCMessage(OSCAddressPattern(oscAddress));
    oscTriggerMsg.addInt32(1); // trigger forced to 1 (not a real trigger then...)
    sendMessageOrThrowException(oscTriggerMsg);
    
    // RE-MUTE après un petit délai,
    // pour éviter les échos plus tard
    Timer::callAfterDelay(/*(int) std::round((double)(OSCRecorder::delayAfterFinished_ms) * 0.9)*/ 200,
                          [this]
                          {
                              String oscAddress = "/master/volume";
                              OSCMessage oscVolumeMsg = OSCMessage(OSCAddressPattern(oscAddress));
                              oscVolumeMsg.addFloat32(0.0f);
                              sendMessageOrThrowException(oscVolumeMsg);
                          });
}


void ReaperOscController::SetTrackSolo_usingMutes(int trackNumber)
{
    if (trackNumber <= 0)
        MonitorCommunication::SendLog("[OSC -> REAPER]: mute pour TOUTES les tracks");
    else
        MonitorCommunication::SendLog("[OSC -> REAPER]: track "
                                      + boost::lexical_cast<std::string>(trackNumber)
                                      + " Solo.");
    
    for (int i=1 ; i<=tracksCount ; i++)
    {
        if (i != trackNumber)
            setTrackMuteState(i, true);
        else
            setTrackMuteState(i, false);
    }
}



void ReaperOscController::setTrackMuteState(int trackNumber, bool shouldBeMuted)
{
    String oscAddress = "/track/";
    oscAddress += String(trackNumber);
    oscAddress += "/mute";
    OSCMessage oscMsg = OSCMessage(OSCAddressPattern(oscAddress));
    if (shouldBeMuted)
        oscMsg.addInt32(1);
    else
        oscMsg.addInt32(0);
    
    sendMessageOrThrowException(oscMsg);
}

void ReaperOscController::waitForMissingReaperResponses()
{
    // en fait... on dirait que Reaper ne fait que transmettre les modifs utilisateur
    // dans le doute : on va renvoyer tous les paquets OSC nécessaires à chaque changement !
}


void ReaperOscController::sendMessageOrThrowException(OSCMessage& oscMessage)
{
    if ( ! sender.send(oscMessage) )
        displayErrorAndThrowException(TRANS("Impossible to send an OSC message to Reaper. Check call stack to find the type of message being sent."));
}

void ReaperOscController::displayErrorAndThrowException(String errorStr)
{
    MonitorCommunication::SendLog(errorStr.toStdString());
    std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
    throw std::runtime_error(errorStr.toStdString());
}
