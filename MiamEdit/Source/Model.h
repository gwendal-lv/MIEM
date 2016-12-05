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

#include "IModel.h"



namespace Miam
{
    // pre-declarations for pointers
    class Presenter;
    class SpatSender;
    
    
    
    /// \brief
    class Model : public IModel
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :

        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction / destruction - - - - -
        Model(Presenter* _presenter);
        ~Model();
        
        
        
        
    };
    
}





#endif  // MODEL_H_INCLUDED