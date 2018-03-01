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



namespace Miam
{
    // pre-declarations for pointers
    class Presenter;
    
    
    
    /// \brief
    class Model : public ControlModel
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        Presenter* presenter;
        protected :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :

        
        
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
