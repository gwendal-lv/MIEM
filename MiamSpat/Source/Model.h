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
        
        // Modules
        Presenter* presenter;
        std::shared_ptr<MiamOscSender<double>> miamOscSender;
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        protected :
        std::shared_ptr<MiamOscSender<double>>& getMainSpatSender()
        { return miamOscSender; };
        public :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        Model(Presenter* presenter_);
        virtual ~Model();
        
        
        
        // - - - - - Periodic updates - - - - -
        protected :
        void update() override;
        
        
        // - - - - - Property tree (for XML) import/export - - - - -
        public :
        virtual void SetConfigurationFromTree(bptree::ptree& tree) override;
        
        
    };
}


#endif  // MODEL_H_INCLUDED
