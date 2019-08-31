/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
