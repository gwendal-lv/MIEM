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

		/// \brief
		///
		/// The presenter may delay the actual send
		virtual void SendParamChange(AsyncParamChange& paramChange);
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IPresenter();
        virtual ~IPresenter() {}
        
        
        
    };
}


#endif  // IPRESENTER_H_INCLUDED
