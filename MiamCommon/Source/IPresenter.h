/*
  ==============================================================================

    IPresenter.h
    Created: 21 Dec 2016 2:29:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IPRESENTER_H_INCLUDED
#define IPRESENTER_H_INCLUDED


#include "AsyncParamChange.h"
#include "boost/lockfree/spsc_queue.hpp"



namespace Miam
{
    class IPresenter {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        /// \brief
        ///
        /// 100-by-100 matrix can be entirely stored within the queue without it being
        /// full.
        boost::lockfree::spsc_queue<AsyncParamChange, boost::lockfree::capacity<(1<<17)>> paramChangesToModel;
        
        
        // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
        public :
        
        /// \brief Tries a get a parameter change, that happened on the Presenter side,
        /// from the internal lock-free queue
        ///
        /// Must always be called from the same main Miam::Model thread
        ///
        /// \returns Wether a parameter change has occured since the last "gets" sequence
        bool TryGetAsyncParamChange(AsyncParamChange& param_);

		/// \brief Sends a parameter change to the Model
		///
		/// The presenter may delay the actual send, but at the moment :
        /// Exception thrown if lock-free queue is full (no dynamic memory allocation)
		void SendParamChange(AsyncParamChange& paramChange);
        
        
        /// \brief Called within the MessageThread by a pseudo-periodic object,
        /// and enslaved to its frequency (for example an OpenGL renderer at ~60Hz).
        ///
        /// Objects are for now only some SceneCanvasComponent ; all of them may call
        /// this asynchronously, so this may be called more often than the display
        /// refresh rate
        virtual void Update() = 0;
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IPresenter();
        virtual ~IPresenter() {}
        
        
        
    };
}


#endif  // IPRESENTER_H_INCLUDED
