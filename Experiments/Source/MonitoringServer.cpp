/*
  ==============================================================================

    MonitoringServer.cpp
    Created: 16 Mar 2019 3:38:41pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
    std::cout << "Monitoring server ready for connection on TCP ports " << serversBeginPort
    << " to " << (serversBeginPort+serversCount) << std::endl;
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

