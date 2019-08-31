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

#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <memory>
#include <vector>

#include "ControlModel.h"

#include "AppPurpose.h"


namespace Miam
{
    // pre-declarations for pointers
    class Presenter;
    
    
    
    /// \brief
    class Model : public ControlModel
    {
        friend class Presenter;
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        Presenter* presenter;
        
        // va pouvoir varier selon la session actuellement chargée
        AppPurpose sessionPurpose;
        
        protected :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        virtual AppPurpose GetSessionPurpose() const override {return sessionPurpose; }
        
        private :
        // Privé, accessible via le presenter ami
        void setSessionPurpose(AppPurpose sessionPurpose_)
        { sessionPurpose = sessionPurpose_; }
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction / destruction - - - - -
        Model(Presenter* presenter_);
        virtual ~Model();
        
        
        
        // - - - - - Periodic updates - - - - -
        protected :
        void update() override;
        
        
        // - - - - - internal events - - - - -
        protected :
        virtual void onPlay() override;
        
        
        // - - - - - Simple OSC sender (for devices OSC learn) - - - - -
        public :
        /// \brief Synchronously sends a unique osc message via the first sender.
        ///
        /// Works in a purely synchronous way... Which is not good for the responsiveness of the GUI
        /// (but this should a very small and non-blocking functionnality)
        ///
        /// Throws exceptions if the OSC sender or OSC message are not configured properly
        void ConnectAndSendOSCMessage(const std::string& oscAddress, double argumentValue);
        
        /// \brief Sends a full state (after trying to connect),
        /// and using the current interpolation curves from the
        /// interpolator.
        ///
        /// Sends only a particular row if a valid index is given.
        void ConnectAndSendState(std::shared_ptr<ControlState<double>> stateToSend,
                                 int rowToSend = -1);
        
        private :
        std::shared_ptr<MiamOscSender<double>> tryConnectAndGetOscSender();
        
    };
    
}





#endif  // MODEL_H_INCLUDED
