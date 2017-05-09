/*
  ==============================================================================

    NetworkModel.h
    Created: 9 May 2017 4:50:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "LockFreeParamChangeSender.h"

namespace Miam {
    
    
    // Forward declarations
    class MatrixRouterAudioProcessor;
    
    
    /// \brief Module of the Model module
    class NetworkModel : public LockFreeParamChangeSender,
    private OSCReceiver,
    
    // !!!!!!!!!!!!!!
    /// Juce 5.0.1 debug macOS :
    /// Realtime callback does not work with ListenerWithOSCAddress
    // !!!!!!!!!!!!!!
    private OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
    
    {
        
        
        
        // ================== ATTRIBUTES ===================
        
        // - - - - - links to other modules - - - - -
        private :
        MatrixRouterAudioProcessor& model;
        
        
        // - - - - - Network settings - - - - -
        // May suffer from concurrent accesses
        // .... sauf si Juce gère bien les appels du DAW
        //int udpPort = -1;
        
        // ================== METHODS ===================
        
        // - - - - - Construction & destruction - - - - -
        public :
        NetworkModel(MatrixRouterAudioProcessor& _model);
        ~NetworkModel();
        
        
        // - - - - - Setters and Getters - - - - -
        
        /// \return Wether the connection could be established or not
        bool SetUdpPort(int _udpPort);
        
        // - - - - - Asynchronous OSC processing - - - - -
        private :
        virtual void oscMessageReceived(const OSCMessage& message) override;
        
        
    };
    
    
} // namespace Miam

        
