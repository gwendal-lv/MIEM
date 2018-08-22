/*
  ==============================================================================

    LockFreeParamChangeSender.h
    Created: 8 May 2017 11:06:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
