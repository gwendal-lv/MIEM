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
        std::shared_ptr<MiamOscSender<double>> miamOscSender;
        
        AsyncParamChange::ParamType playState;
        
        // Rafraîchissements forcés
        int refreshFramesCounter = 0;
        const int refreshPeriod_frames = 500; // unité = frames
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        protected :
        std::shared_ptr<MiamOscSender<double>>& getMainSpatSender()
        { return miamOscSender; };
        public :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        PlayerModel(PlayerPresenter* presenter_);
        virtual ~PlayerModel();
        
        
        
        // - - - - - Periodic updates - - - - -
        protected :
        void setHighThreadPriority();
        void update() override;
        
        
        // - - - - - Property tree (for XML) import/export - - - - -
        public :
        virtual void SetConfigurationFromTree(bptree::ptree& tree) override;
        
        
    };
}


#endif  // MODEL_H_INCLUDED
