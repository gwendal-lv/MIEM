/*
  ==============================================================================

    Model.h
    Created: 24 Jan 2017 9:09:12am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


#include "SpatModel.h"


namespace Miam {
    
    // Pre-declarations for pointer members
    class Presenter;
    
    /// \brief
    class Model : public SpatModel
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        Presenter* presenter;
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        Model(Presenter* presenter_);
        virtual ~Model();
        
        
        
        // - - - - - Periodic updates - - - - -
        protected :
        void update() override;
        
        
        
        
    };
}


#endif  // MODEL_H_INCLUDED
