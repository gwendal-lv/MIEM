/*
  ==============================================================================

    Model.h
    Created: 24 Jan 2017 9:09:12am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


#include "ControlModel.h"


namespace Miam {
    
    // Pre-declarations for pointer members
    class PlayerPresenter;
    
    /// \brief
    class PlayerModel : public ControlModel
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        // Modules
        PlayerPresenter* presenter;
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        PlayerModel(PlayerPresenter* presenter_);
        virtual ~PlayerModel();
        
        
        
        // - - - - - Periodic updates - - - - -
        protected :
        void setHighThreadPriority();
        virtual void update() override;
        
        
        // - - - - - Property tree (for XML) import/export - - - - -
        public :
        virtual void SetConfigurationFromTree(bptree::ptree& tree) override;
        
        
    };
}


#endif  // MODEL_H_INCLUDED
