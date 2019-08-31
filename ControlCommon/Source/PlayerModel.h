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
        void launchUpdateThread();
        virtual void update() override;
        
        
        
        // - - - - - Events from Presenter - - - - -
        public :
        // see ControlModel.h
        virtual bool ResetOscConfigurationFromTree(bptree::ptree& tree) override;
        
        
        
        
        // - - - - - Property tree (for XML) import/export - - - - -
        public :
        virtual void SetConfigurationFromTree(bptree::ptree& tree) override;
        
        
    };
}


#endif  // MODEL_H_INCLUDED
