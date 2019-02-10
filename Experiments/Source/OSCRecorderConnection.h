/*
  ==============================================================================

    OSCRecorderConnection.h
    Created: 9 Feb 2019 4:16:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class OSCRecorder;

class OSCRecorderConnection : public InterprocessConnection
{
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    private :
    OSCRecorder& recorderManager; // parent manager class
    
    
    
    // = = = = = = = = = = METHODS = = = = = = = = = =
    public :
    OSCRecorderConnection(OSCRecorder& _recorderManager);
    virtual ~OSCRecorderConnection() {}
    
    
    // - - - - - connection callback methods - - - - -
    public :
    virtual void connectionMade() override;
    virtual void connectionLost() override;
    virtual void messageReceived(const MemoryBlock& message) override;
    
    
};
