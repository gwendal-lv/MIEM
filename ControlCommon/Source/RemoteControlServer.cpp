/*
  ==============================================================================

    RemoteControlServer.cpp
    Created: 9 Feb 2019 1:47:03pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
