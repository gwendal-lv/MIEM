/*
  ==============================================================================

    RemoteControlConnection.cpp
    Created: 9 Feb 2019 2:14:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "RemoteControlConnection.h"

#include "InterprocessControlBlock.h"

#include "PlayerPresenter.h"
#include "GraphicSessionPlayer.h"

using namespace Miam;


RemoteControlConnection::RemoteControlConnection(PlayerPresenter* _presenter)
:
// magic number "signs" the messages only. The connection will always be allowed
// even for a client with a wrong magic number
InterprocessConnection(true, MIEM_MAGIC_MESSAGE_HEADER_NUMBER), // callback on message thread
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
    
    AsyncParamChange receivedParam = InterprocessControlBlock::DecodeMemoryBlock(message);
    if (receivedParam.Type != AsyncParamChange::None) // if not the "unvalid" case
    {
        switch (receivedParam.Type) {
            case AsyncParamChange::Scene:
                if (receivedParam.Id1 >= 0)
                    presenter->GetGraphicSessionPlayer()->SelectSceneOfSelectedCanvas((int)receivedParam.Id1);
                else
                {
                    DBG("Unrecognized SCENE ID from TCP packet received.");
                    assert(false);
                }
                break;
                
            default:
                break;
        }
    }
    else
    {
        DBG("Unrecognized or unvalid TCP packet received.");
        assert(false);
    }
}


