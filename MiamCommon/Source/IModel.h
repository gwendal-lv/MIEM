/*
  ==============================================================================

    IModel.h
    Created: 31 Dec 2016 9:25:00am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IMODEL_H_INCLUDED
#define IMODEL_H_INCLUDED



#include "AsyncParamChange.h"
#include "boost/lockfree/spsc_queue.hpp"


#include "FrequencyMeasurer.h"


namespace Miam
{
    class IModel
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        /// \brief 1 Mo lock-free queue
        boost::lockfree::spsc_queue<AsyncParamChange, boost::lockfree::capacity<(1<<20)>> paramChangesToPresenter;
        
        // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
        public :
        
        /// \brief Tries a get a parameter change, that happened on the Model side,
        /// from the internal lock-free queue.
        ///
        /// Must be always called from the same unique Miam::Presenter thread (called
        /// back for the MessageManager Juce thread).
        ///
        /// \return Wether there was a param change to pop or not
        bool TryGetAsyncParamChange(AsyncParamChange& param_);
        
        
        /// \brief Sends a parameter change to the Presenter
        ///
        /// Exception thrown if lock-free queue is full (no dynamic memory allocation)
        void SendParamChange(AsyncParamChange& paramChange);
        
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IModel();
        virtual ~IModel() {}
        
        
    };
}


#endif  // IMODEL_H_INCLUDED
