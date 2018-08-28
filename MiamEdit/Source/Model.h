/*
  ==============================================================================

    Model.h
    Created: 26 Nov 2016 2:19:48pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <memory>
#include <vector>

#include "ControlModel.h"

#include "AppPurpose.h"


namespace Miam
{
    // pre-declarations for pointers
    class Presenter;
    
    
    
    /// \brief
    class Model : public ControlModel
    {
        friend class Presenter;
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        Presenter* presenter;
        
        // va pouvoir varier selon la session actuellement chargée
        AppPurpose sessionPurpose;
        
        protected :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        virtual AppPurpose GetSessionPurpose() const override {return sessionPurpose; }
        
        private :
        // Privé, accessible via le presenter ami
        void setSessionPurpose(AppPurpose sessionPurpose_)
        { sessionPurpose = sessionPurpose_; }
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction / destruction - - - - -
        Model(Presenter* presenter_);
        virtual ~Model();
        
        
        
        // - - - - - Periodic updates - - - - -
        protected :
        void update() override;
        
        
        
        // - - - - - Simple OSC sender (for devices OSC learn) - - - - -
        // Works in a purely synchronous way... Which is not good for the responsiveness of the GUI
        // (but this is a very small and non-blocking functionnality)
        void TryConnectAndSendOSCMessage(const std::string& oscAddress, double argumentValue);
        
    };
    
}





#endif  // MODEL_H_INCLUDED
