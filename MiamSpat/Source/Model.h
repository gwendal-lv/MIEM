/*
  ==============================================================================

    Model.h
    Created: 13 Jan 2019 4:28:05pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "PlayerModel.h"

namespace Miam
{
    class Presenter;
    
    
    /// \brief Adds a few missing features that are related to Spatialiation only.
    class Model : public PlayerModel {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        // Modules
        Presenter* presenter;
        
        /// \brief For recording how much time has passed since
        /// the last info was sent to the Presenter
        double durationSinceLastInfoToPresenter_ms = 0.0;
        const double presenterRefreshPeriodMin_ms = 100.0;
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        Model(Presenter* presenter_);
        virtual ~Model() {}
        
        
        protected :
        virtual void onUpdateStarts() override;
        virtual void onUpdateFinished() override;
        
        
    };
    
} // namespace Miam
