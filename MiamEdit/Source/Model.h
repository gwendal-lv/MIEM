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
        AppPurpose GetSessionPurpose() {return sessionPurpose; }
        
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
        
        
    };
    
}





#endif  // MODEL_H_INCLUDED
