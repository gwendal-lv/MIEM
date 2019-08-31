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

#ifndef PRESENTER_H_INCLUDED
#define PRESENTER_H_INCLUDED

#include <iostream>

#include "PlayerPresenter.h"

#include "SpatType.h"

#include "GraphicSessionPlayer.h"

#include "MiemFileChoosers.h"


namespace Miam {
    // Simple declaration : we don't need the entire description
    class Model;
    class View;
    

    
    /// \brief One of the three main modules of the MiamSpat program, that defines the behavior
	/// between the user and displayed GUI elements. Miam-specific graphical objects belong to it.
	///
	/// Read <a href="http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter">MPV scheme</a>
	/// for a better understanding of MiamEdit functionnalities between the Model, the Presenter 
	/// and the View : "The presenter acts upon the model and the view. It retrieves data from 
	/// repositories (the model), and formats it for display in the view".
	///
	/// \remark Usual GUI controls (created from the Projucer) however belong to the View.
    class Presenter : public PlayerPresenter
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        // Pointer to the unique View module
        View* view = 0;
        Model* model;
        
        
        // Sub-modules
        GraphicSessionPlayer graphicSessionManager;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        
        public :
        virtual GraphicSessionPlayer* GetGraphicSessionPlayer() override
        { return &graphicSessionManager; }
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // - - - - - Contruction and Destruction - - - - -
        // Only constructor
        Presenter(View* _view);
        void CompleteInitialisation(Model* _model);
        virtual ~Presenter() {}
        
        virtual void OnLoadDefaultSession() override;
        virtual AppPurpose GetSessionPurpose() const override {return AppPurpose::GenericController;}
        
        
        protected :
        /// \brief Called by a parent class when emptying the lock-free queue
        virtual void processParamChangeFromModel(AsyncParamChange const & paramChange) override;
        
        
    };
    
    
    
}



#endif  // PRESENTER_H_INCLUDED
