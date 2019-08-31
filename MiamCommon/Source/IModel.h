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

#ifndef IMODEL_H_INCLUDED
#define IMODEL_H_INCLUDED


#include "LockFreeParamChangeSender.h"
#include <iostream>

#include "InterpolationTypes.h"


namespace Miam
{
    class IModel : public LockFreeParamChangeSender
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IModel();
        virtual ~IModel() {}
        
        /// \brief Renvoie le type d'interpolation actuellement calculée, ce qui est une
        /// donnée partagée par le Presenter et le Modèle ;
        /// doit donc rester thread-safe y compris pour les override potentiels
        virtual InterpolationType GetInterpolatorType_Atomic ()
        { return InterpolationType::Matrix_Linear; } // par défaut
        
    };
}


#endif  // IMODEL_H_INCLUDED
