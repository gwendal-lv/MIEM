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

#include "RemoteControlServer.h"

#include "PlayerPresenter.h"

using namespace Miam;

RemoteControlServer::RemoteControlServer(PlayerPresenter* _presenter)
: presenter(_presenter)
{
}

InterprocessConnection* RemoteControlServer::createConnectionObject()
{
    // We need to create a custom, derived InterprocessConnection object.
    // Many of its methods will be overriden ! Quite easy to use then
    auto newConnection = std::make_unique<RemoteControlConnection>(presenter);
    
    remoteConnections.push_back( std::move(newConnection) );
    
    return remoteConnections.back().get();
}


void RemoteControlServer::DisplayStatusInConsole()
{
    
}
