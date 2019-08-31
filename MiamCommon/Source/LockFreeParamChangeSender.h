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

#include "AsyncParamChange.h"
#include "../../boost/lockfree/spsc_queue.hpp"


namespace Miam
{
    class LockFreeParamChangeSender
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        /// \brief
        ///
        /// 100-by-100 matrix can be entirely stored within the queue without it being
        /// full.
        boost::lockfree::spsc_queue<AsyncParamChange, boost::lockfree::capacity<(1<<17)>> paramChangesToSend;
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        virtual ~LockFreeParamChangeSender() {}
        
        /// \brief Tries a get a parameter change, that happened on this side
        ///
        /// Must be always called from the same unique thread
        ///
        /// \return Wether there was a param change to pop or not
        bool TryGetAsyncParamChange(AsyncParamChange& param_);
        
        
        
        
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// LA SUITE DEVRAIT DEVENIR PROTECTED
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        
        
        /// \brief Enqueues a parameter change
        ///
        /// The unique thread that is supposed to retrieve the data will actually get
        /// this data when it will ask for it (through the TryGetAsyncParamChange
        /// function)
        ///
        /// Exception thrown if lock-free queue is full (no dynamic memory allocation)
        virtual void SendParamChange(AsyncParamChange& paramChange);
        
        
        /// \brief Tries to enqueue a parameter change
        ///
        /// The unique thread that is supposed to retrieve the data will actually get
        /// this data when it will ask for it (through the TryGetAsyncParamChange
        /// function)
        ///
        /// \return Wether the queue was full or not (parameter change enqueued or not)
        virtual bool TrySendParamChange(AsyncParamChange& paramChange);
        
        
        
    };
}
