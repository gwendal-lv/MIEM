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

#ifndef IPRESENTER_H_INCLUDED
#define IPRESENTER_H_INCLUDED


#include <chrono>

#include "LockFreeParamChangeSender.h"

#include "AppPurpose.h"
#include "InterpolationTypes.h"


namespace Miam
{
    class IModel;
    
    
    class IPresenter : public LockFreeParamChangeSender
    {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        IModel* model = 0;
        
        // - - - - - Time management - - - - -
        protected :
        /// \brief Common reference shared with the Miam::IModel (model must
        /// retrieve it later)
        std::chrono::time_point<std::chrono::steady_clock> constructionTimePt;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        std::chrono::time_point<std::chrono::steady_clock> GetCommonTimePoint() const
        { return constructionTimePt; }
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IPresenter();
        virtual ~IPresenter() {}

        void CompleteInitialization(IModel*);
        
        /// \brief Called within the MessageThread by a pseudo-periodic object,
        /// and enslaved to its frequency (for example an OpenGL renderer at ~60Hz).
        ///
        virtual void Update() = 0;
        
        virtual AppPurpose GetSessionPurpose() const = 0;
        
        InterpolationType GetInterpolatorType() const;
        
    };
}


#endif  // IPRESENTER_H_INCLUDED
