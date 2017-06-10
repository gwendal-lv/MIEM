/*
  ==============================================================================

    SpatPresenter.h
    Created: 6 Jun 2017 10:49:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <string>

#include "IPresenter.h"


namespace Miam {
    
    class SpatModel;
    
    /// \brief
    class SpatPresenter : public IPresenter
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        // Pointer to the unique Model module
        SpatModel* model = 0;
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        // - - - - - Construction and Destruction (and init) - - - - -
        void CompleteInitialisation(SpatModel* _model);
        
        // - - - - - XML loading - - - - -
        virtual void LoadSession(std::string filename);
        
    };
    
    
    
}
