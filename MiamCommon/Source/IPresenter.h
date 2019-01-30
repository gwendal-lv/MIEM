/*
  ==============================================================================

    IPresenter.h
    Created: 21 Dec 2016 2:29:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IPRESENTER_H_INCLUDED
#define IPRESENTER_H_INCLUDED


#include <chrono>

#include "LockFreeParamChangeSender.h"

#include "AppPurpose.h"
#include "InterpolationTypes.h"


namespace Miam
{
    class IModel;
    
    
    class IPresenter : public LockFreeParamChangeSender
    {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        IModel* model = 0;
        
        // - - - - - Time management - - - - -
        protected :
        /// \brief Common reference shared with the Miam::IModel (model must
        /// retrieve it later)
        std::chrono::time_point<std::chrono::steady_clock> constructionTimePt;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        std::chrono::time_point<std::chrono::steady_clock> GetCommonTimePoint() const
        { return constructionTimePt; }
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IPresenter();
        virtual ~IPresenter() {}

        void CompleteInitialization(IModel*);
        
        /// \brief Called within the MessageThread by a pseudo-periodic object,
        /// and enslaved to its frequency (for example an OpenGL renderer at ~60Hz).
        ///
        virtual void Update() = 0;
        
        virtual AppPurpose GetSessionPurpose() const = 0;
        
        InterpolationType GetInterpolatorType() const;
        
    };
}


#endif  // IPRESENTER_H_INCLUDED
