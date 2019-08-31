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

#include "MonitoringServer.h"

// Problème avec le singleton... destructeur appelé après que Juce ne fasse toutes
// les vérifications de leaks....
MonitoringServer* MonitoringServer::mainInstance = nullptr;


void MonitoringServer::SendLog(std::string info)
{
    if (mainInstance == nullptr)
        mainInstance = new MonitoringServer();
    
    mainInstance->sendLog(info);
}


MonitoringServer::MonitoringServer()
{
    for (int i=0 ; i<serversCount ; i++)
    {
        servers.push_back(std::make_unique<MiemLogServer>(this));
        servers[i]->beginWaitingForSocket(serversBeginPort + i);
    }
    
    if (serversCount >= 1)
        std::cout << "Monitoring server ready for connections on TCP ports " << serversBeginPort << " to " << (serversBeginPort + serversCount-1) << std::endl;
    else
        std::cout << "Monitoring server disabled (displays info in console but does not accept incoming connections)" << std::endl;
    
    
    Timer::callAfterDelay(initialEnqueuingDuration_ms,
                          [this] { isInitialEnqueuingFinished = true; } );
}
MonitoringServer::~MonitoringServer()
{
    // dans le doute : on supprime d'abord les connections, puis les serveurs
    // Car Juce envoie toujours des assertions pour leak de connection,
    // même si la classe hérite de Juce::DeletedAtShutdown.......
    connections.clear();
    servers.clear();
    
    // fin
    //std::cout << "Destruction of MonitorCommunication servers." << std::endl;
}


InterprocessConnection * MonitoringServer::CreateConnectionObject(MiemLogServer* requestingServer)
{
    connections.push_back(std::make_unique<MiemLogConnection>(this));
    return connections[connections.size() - 1].get();
}
void MonitoringServer::OnConnectionLost(MiemLogConnection* /*miemConnection*/)
{
    // connections destructor call will be delayed on juce message thread
    Timer::callAfterDelay(10, [this] { deleteLostConnections(); });
}

void MonitoringServer::sendLog(const std::string& info)
{
    // at beginning, data is put in a queue, while we wait for all waiting
    // clients to actually establish a TCP connection
    if (! isInitialEnqueuingFinished)
    {
        if (! info.empty() )
        {
            DBG(info);
            infoQueue.push(info);
            Timer::callAfterDelay(initialEnqueuingDuration_ms,
                                  [this] { sendLog(std::string("")); } );
        }
    }
    else
    {
        // recherche de connection perdues.... en synchrone, pas de soucis ici
        deleteLostConnections();
        
        // On envoie d'abord les anciennes infos en attente (sans
        // sortie console, car déjà faite)
        while( infoQueue.size() > 0)
        {
            MemoryBlock stringBlock = MemoryBlock(infoQueue.front().c_str(),
                                                  infoQueue.front().size());
            for (auto it = connections.begin() ; it != connections.end() ; it++)
                (*it)->sendMessage(stringBlock);
            infoQueue.pop();
        }
        // Puis la nouvelle info (si elle n'est pas vide)
        if (! info.empty())
        {
            DBG(info);
            // Copie des données (pour ne prendre aucun risque pour le programme
            // principal...)
            MemoryBlock stringBlock = MemoryBlock(info.c_str(), info.size());
            for (auto it = connections.begin() ; it != connections.end() ; it++)
                (*it)->sendMessage(stringBlock);
        }
    }
}



void MonitoringServer::deleteLostConnections()
{
    // recherche bourrine, et suppression dans le tableau
    // de toute les connections perdues...
    auto it = connections.begin(); // attention aux itérateurs dans les boucles for...
    /// après erase, l'itérateur est incrémenté avant le test -> end()++ devient n'imp !
    while( it != connections.end() )
    {
        if (! (*it)->isConnected())
            it = connections.erase(it);
        else
            it++;
    }
}

