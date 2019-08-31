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

#include "LockFreeParamChangeSender.h"

using namespace Miam;


void LockFreeParamChangeSender::SendParamChange(AsyncParamChange& paramChange)
{
    if (!paramChangesToSend.push(paramChange))
        throw std::runtime_error("Lock-Free Queue is full : cannot push an element...");
}


bool LockFreeParamChangeSender::TrySendParamChange(AsyncParamChange& paramChange)
{
    return paramChangesToSend.push(paramChange);
}



bool LockFreeParamChangeSender::TryGetAsyncParamChange(AsyncParamChange& param_)
{
    return paramChangesToSend.pop(param_);
}
