/*
  ==============================================================================

    RemoteControlConnection.cpp
    Created: 9 Feb 2019 2:14:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "RemoteControlConnection.h"

using namespace Miam;


RemoteControlConnection::RemoteControlConnection(PlayerPresenter* _presenter) :
presenter(_presenter)
{
    
}



void RemoteControlConnection::connectionMade()
{
    std::cout << "[Remote connection made]" << std::endl;
}
void RemoteControlConnection::connectionLost()
{
    std::cout << "[Remote connection lost]" << std::endl;
}
void RemoteControlConnection::messageReceived(const MemoryBlock& message)
{
    std::cout << "[Remote connection: message received]" << std::endl;
}


