/*
  ==============================================================================

    IModel.h
    Created: 31 Dec 2016 9:25:00am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IMODEL_H_INCLUDED
#define IMODEL_H_INCLUDED


#include <atomic>
#include <thread>

#include "LockFreeQueue.h"
#include "FrequencyMeasurer.h"


namespace Miam
{
    class IModel
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        LockFreeQueue paramChangesToPresenter;
        
        // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
        public :
        
        /// \brief Tries a get a parameter change, that happened on the Model side,
        /// from the internal Miam::LockFreeQueue.
        ///
        /// Must be always called from the same unique Miam::Presenter thread
        ///
        bool TryGetAsyncParamChange(AsyncParamChange& param_)
        { return paramChangesToPresenter.TryDequeue(param_); }
        
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IModel();
        virtual ~IModel() {}
        
        
    };
}


#endif  // IMODEL_H_INCLUDED
