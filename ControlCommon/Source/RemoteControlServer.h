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
