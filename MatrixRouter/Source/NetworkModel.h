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
        /// only... We'll see if we observe strange MT conflicts. -1 means 'disconnect'
        ///
        /// \return Wether the connection (or disconnection) could be performed or not
        bool SetUdpPort(int _udpPort, bool notifyModel=false);
        // IMPLÉMENTATION DU -1 POUR SE DÉCONNECTER : À FAIRE
        // IMPLÉMENTATION DU -1 POUR SE DÉCONNECTER : À FAIRE
        // IMPLÉMENTATION DU -1 POUR SE DÉCONNECTER : À FAIRE
        // IMPLÉMENTATION DU -1 POUR SE DÉCONNECTER : À FAIRE
        // IMPLÉMENTATION DU -1 POUR SE DÉCONNECTER : À FAIRE
        // IMPLÉMENTATION DU -1 POUR SE DÉCONNECTER : À FAIRE
        // IMPLÉMENTATION DU -1 POUR SE DÉCONNECTER : À FAIRE
        
        
        
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

        
