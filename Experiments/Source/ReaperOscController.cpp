/*
  ==============================================================================

    ReaperOscController.cpp
    Created: 13 Feb 2019 5:35:34pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "ReaperOscController.h"

#include <thread>


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


void ReaperOscController::SetTrackSolo_usingMutes(int trackNumber)
{
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
    
    if ( ! sender.send(oscMsg) )
        displayErrorAndThrowException(TRANS("Impossible to send on OSC message to Reaper"));
}

void ReaperOscController::waitForMissingReaperResponses()
{
    // en fait... on dirait que Reaper ne fait que transmettre les modifs utilisateur
    // dans le doute : on va renvoyer tous les paquets OSC nécessaires à chaque changement !
}



void ReaperOscController::displayErrorAndThrowException(String errorStr)
{
    DBG(errorStr);
    std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
    throw std::runtime_error(errorStr.toStdString());
}
