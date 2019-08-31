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

#include <iostream>

#include "IPresenter.h"
#include "IModel.h"


using namespace Miam;

IPresenter::IPresenter()
:
constructionTimePt( std::chrono::steady_clock::now() )
{
    //std::cout << "AsyncParamChange size = " << sizeof(AsyncParamChange) << "bytes" << std::endl;
}


void IPresenter::CompleteInitialization(IModel* _model)
{
    model = _model;
}


InterpolationType IPresenter::GetInterpolatorType() const
{
    // On n'est pas obligé d'utiliser le type d'interpolation, mais si on l'utilise, il fallait
    // absoluement que le modèle soit initialité...
    assert(model != 0);
    
    return model->GetInterpolatorType_Atomic();
}

