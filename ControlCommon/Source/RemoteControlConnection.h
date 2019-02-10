/*
  ==============================================================================

    RemoteControlConnection.h
    Created: 9 Feb 2019 2:14:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "OscDefines.h" // also contains defines for non-OSC connections...


namespace Miam
{
    class PlayerPresenter;
    
    class RemoteControlConnection : public juce::InterprocessConnection
    {
        // = = = = = = = = ATTRIBUTES = = = = = =
        private :
        PlayerPresenter* const presenter; // const pointer to Presenter
        
        
        // = = = = = = = = METHODS = = = = = =
        public :
        // default ctor
        RemoteControlConnection(PlayerPresenter* _presenter);
        
        
        
        // - - - - - connection callback methods - - - - -
        public :
        virtual void connectionMade() override;
        virtual void connectionLost() override;
        virtual void messageReceived(const MemoryBlock& message) override;
        
    };
}
