/*
  ==============================================================================

    OSCRecorderConnection.cpp
    Created: 9 Feb 2019 4:16:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "OSCRecorderConnection.h"

#include "OSCRecorder.h"


OSCRecorderConnection::OSCRecorderConnection(OSCRecorder& _recorderManager, uint32 magicHeaderNumber)
:
// magic number "signs" the messages only. The connection will always be allowed
// even for a client with a wrong magic number
InterprocessConnection(true, magicHeaderNumber), // callbacks on message thread
recorderManager(_recorderManager)
{
    
}



void OSCRecorderConnection::connectionMade()
{
    // nothing to do.... the manager already waited for
    // the connection to be made !
}
void OSCRecorderConnection::connectionLost()
{
    recorderManager.OnConnectionLost();
}
void OSCRecorderConnection::messageReceived(const MemoryBlock& message)
{
    std::cout << "[Remote connection: message received]" << std::endl;
}

