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

#pragma once

#include <memory>
#include <mutex>
#include <queue>

#include "JuceHeader.h"


/// \brief Singleton class for sending monitoring data to devices
/// connected to this server.
class MonitoringServer : public DeletedAtShutdown
{
    
    class MiemLogServer : public InterprocessConnectionServer
    {
        private :
        MonitoringServer* parent;
        public :
        MiemLogServer(MonitoringServer* _parent)
        : parent(_parent) {}
        ~MiemLogServer() {}
        virtual InterprocessConnection * createConnectionObject () override
       { return parent->CreateConnectionObject(this); }
    };
    
    class MiemLogConnection : public InterprocessConnection
    {
        private :
        MonitoringServer* parent;
        public :
        MiemLogConnection(MonitoringServer* _parent)
        : parent(_parent) {}
        virtual void     connectionMade () override
        {}
        virtual void     connectionLost () override
        { parent->OnConnectionLost(this); }
        virtual void     messageReceived (const MemoryBlock &message) override
        {}
    };
    
    
    
    
    
    
    
    // ------------ Singleton interface and attributes ----------
    private :
    static MonitoringServer* mainInstance;
    public :
    
    /// \brief Sends a message to all opened connections.
    ///
    /// Main function of the singleton
    static void SendLog(std::string info);
    
    
    // ========== ATTRIBUTES ==========
    private :
    std::mutex mainMutex; // useless at the moment...
    const int serversBeginPort = 19192;
    
#ifdef __MIEM_ALLOW_MONITORING_SERVER_CONNECTIONS
    const int serversCount = 10;
#else
    const int serversCount = 0;
#endif
    
    /// \brief Gives enough time for all waiting clients to actually connect.
    /// After the delay, we consider that all client are connected and we start sending
    /// messages in the queue.
    const int initialEnqueuingDuration_ms = 6000;
    
    std::queue<std::string> infoQueue;
    bool isInitialEnqueuingFinished = false;

    std::vector<std::unique_ptr<MiemLogServer>> servers;
    std::vector<std::unique_ptr<MiemLogConnection>> connections;
    
    // ========== METHODS ==========
    private :
    /// \brief Constructeur privé car singleton
    MonitoringServer();
    public :
    /// \brief Destructeur doit par contre être public
    ~MonitoringServer();

    protected : // fonction callback pour les connections et les objets-serveurs
    InterprocessConnection * CreateConnectionObject(MiemLogServer*);
    void OnConnectionLost(MiemLogConnection*);
    private :
    void sendLog(const std::string& info);
    
    /// \brief Dans une fonction séparée, après après un délai, car on ne peut pas supprimer
    /// les connections directement dans le callback... puisque ce sont les
    /// connections qui appellent le callback
    void deleteLostConnections();
};
