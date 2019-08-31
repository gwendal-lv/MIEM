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

#include <memory>

#include "IGraphicSessionManager.h"
#include "ControlArea.h"

#include "StatesInterpolator.hpp"

#include "AsyncParamChange.h"


namespace Miam
{
    // Forward declarations
    class ControlPresenter;
    
    class GraphicControlSessionManager : public IGraphicSessionManager
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        ControlPresenter* presenter;
        
        protected :
        
        // For communication with Model
        std::shared_ptr<StatesInterpolator<double>> spatInterpolator;
        
        
        
        
        
        // = = = = = = = = = = Getters and Setters = = = = = = = = = =
        public :
        
        
        
        // = = = = = = = = = = = = = = METHODS = = = = = = = = = = = = = =
        
        public :
        
        // - - - - - Construction/destruction, and helpers - - - - -
        
        /// \brief Construction (the whole Presenter module is built after the View).
        GraphicControlSessionManager(ControlPresenter* _presenter);
        
        /// \Brief to be called after Model construction (which occurs
        /// after Presenter construction)
        void CompleteInitialisation(std::shared_ptr<StatesInterpolator<double>> _spatInterpolator);
        
        /// \brief Destruction and the editor and the canvases
        virtual ~GraphicControlSessionManager() {}
        
        protected :
        AsyncParamChange buildExcitementParamChange(std::shared_ptr<ControlArea> area);

        
        // - - - - - XML import/export - - - - -
        public :
        virtual void SetFromTree(bptree::ptree& graphicSessionTree) override;
        /// \Brief Finishes to XML-load spat attributes of an area, from the inner
        /// content of the <spatstate> tag.
        ///
        /// \param spatStateTree may be a std::nullptr
        virtual void LoadSpatAreaLinks(std::shared_ptr<ControlArea> area, std::shared_ptr<bptree::ptree> spatStateTree);
        
    };
}
