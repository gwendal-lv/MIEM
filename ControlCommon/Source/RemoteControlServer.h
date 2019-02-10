/*
  ==============================================================================

    RemoteControlServer.h
    Created: 9 Feb 2019 1:47:03pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <vector>

#include "JuceHeader.h"

#include "RemoteControlConnection.h"


namespace Miam
{
    class PlayerPresenter;
    
    class RemoteControlServer : public juce::InterprocessConnectionServer
    {
        // = = = = = = = = ATTRIBUTES = = = = = =
        private :
        PlayerPresenter* const presenter; // const pointer to Presenter
        std::vector<std::unique_ptr<RemoteControlConnection>> remoteConnections;
        
        
        // = = = = = = = = METHODS = = = = = =
        public :
        // default ctor
        RemoteControlServer(PlayerPresenter* _presenter);
        virtual ~RemoteControlServer() {}
        
        
        // - - - - - server methods - - - - -
        protected :
        virtual InterprocessConnection* createConnectionObject() override;
        
        
        public :
        // Display clients status
        void DisplayStatusInConsole();
        
    };
}
