/*
  ==============================================================================

    IPresenter.h
    Created: 21 Dec 2016 2:29:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IPRESENTER_H_INCLUDED
#define IPRESENTER_H_INCLUDED


#include "LockFreeQueue.h"


namespace Miam
{
    class IPresenter {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        LockFreeQueue paramChangesToModel;
        
        
        
        // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
        public :
        
        /// \brief Tries a get a parameter change, that happened on the Presenter side,
        /// from the internal Miam::LockFreeQueue
        ///
        /// \returns Wether a parameter change has occured since the last "gets" sequence
        bool TryGetAsyncParamChange(AsyncParamChange& param_)
        { return paramChangesToModel.TryDequeue(param_); }

		/// \brief Sends a parameter change to the Model
		///
		/// The presenter may delay the actual send
		virtual void SendParamChange(AsyncParamChange& paramChange);
        
        
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
