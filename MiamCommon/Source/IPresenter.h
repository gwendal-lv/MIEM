/*
  ==============================================================================

    IPresenter.h
    Created: 21 Dec 2016 2:29:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IPRESENTER_H_INCLUDED
#define IPRESENTER_H_INCLUDED


#include "LockFreeParamChangeSender.h"



namespace Miam
{
    class IPresenter : public LockFreeParamChangeSender
    {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :

        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IPresenter();
        virtual ~IPresenter() {}

        /// \brief Called within the MessageThread by a pseudo-periodic object,
        /// and enslaved to its frequency (for example an OpenGL renderer at ~60Hz).
        ///
        virtual void Update() = 0;
        
        
        
        
    };
}


#endif  // IPRESENTER_H_INCLUDED
