/*
  ==============================================================================

    LockFreeParamChangeSender.cpp
    Created: 8 May 2017 11:06:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
