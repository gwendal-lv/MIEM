/*
  ==============================================================================

    NetworkModel.h
    Created: 9 May 2017 4:50:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <vector>
#include <string>

#include "JuceHeader.h"

#include "LockFreeParamChangeSender.h"

#include "OscDefines.h"

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
        int udpPort = -1;
        std::string oscAddress;
        std::string oscZeroMatrixSuffix;
        
        // ================== METHODS ===================
        
        // - - - - - Construction & destruction - - - - -
        public :
        NetworkModel(MatrixRouterAudioProcessor& _model, std::string _oscAddress = Miam_OSC_Matrix_Address, std::string _oscZeroMatrixSuffix = Miam_OSC_Set_to_Zero_Address);
        ~NetworkModel();
        
        
        // - - - - - Setters and Getters - - - - -
        
        /// Called from ??? At the moment, should be called from the Juce UI thread
        /// only... We'll see if we observe strange MT conflicts
        ///
        /// \return Wether the connection could be established or not
        bool SetUdpPort(int _udpPort, bool notifyModel=false);
        
        int GetUdpPort() {return udpPort;}
        
        /// \Brief Not thread-safe at the moment !
        std::string GetOscAddress() {return oscAddress;}
        std::string GetOscZeroMatrixAddress() {return oscAddress + oscZeroMatrixSuffix;}
        
        // - - - - - Asynchronous OSC processing - - - - -
        private :
        // Called from the network thread
        virtual void oscMessageReceived(const OSCMessage& message) override;
        
        
    };
    
    
} // namespace Miam

        
