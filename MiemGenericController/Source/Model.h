/*
  ==============================================================================

    Model.h
    Created: 10 Mar 2018 12:28:48am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "PlayerModel.h"



namespace Miam
{
    class Presenter;
    
    class Model : public PlayerModel
    {
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        Presenter* presenter;
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        Model(Presenter*);
        virtual ~Model() {}
        
        
        // - - - - - Property tree (for XML) import/export - - - - -
        public :
        virtual void SetConfigurationFromTree(bptree::ptree& tree) override;
    };
    
}


