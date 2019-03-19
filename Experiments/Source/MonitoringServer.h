/*
  ==============================================================================

    MonitoringServer.h
    Created: 16 Mar 2019 3:38:41pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
    const int serversCount = 10;
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
