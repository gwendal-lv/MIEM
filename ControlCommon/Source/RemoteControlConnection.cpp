/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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


