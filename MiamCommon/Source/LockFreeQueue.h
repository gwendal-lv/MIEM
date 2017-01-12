/*
  ==============================================================================

    LockFreeQueue.h
    Created: 23 Dec 2016 4:30:40pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef LOCKFREEQUEUE_H_INCLUDED
#define LOCKFREEQUEUE_H_INCLUDED

#include "readerwriterqueue/readerwriterqueue.h"
#include "AsyncParamChange.h"

namespace Miam
{
    
    /// \brief A lock-free queue that mostly relies on the
    /// moodycamel::ReaderWriterQueue
    ///
    class LockFreeQueue {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        /// \brief Able to enqueue all coefficients of a 100-by-100 matrix
        moodycamel::ReaderWriterQueue<AsyncParamChange, (1<<17)> changesQueue;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        AE_FORCEINLINE bool TryEnqueue(AsyncParamChange& param_)
        {
            return changesQueue.try_enqueue(param_);
        }
        AE_FORCEINLINE bool TryDequeue(AsyncParamChange& param_)
        {
            return changesQueue.try_dequeue(param_);
        }
        
    };
    
}


#endif  // LOCKFREEQUEUE_H_INCLUDED
