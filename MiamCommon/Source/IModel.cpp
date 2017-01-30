/*
  ==============================================================================

    IModel.cpp
    Created: 31 Dec 2016 9:25:00am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "IModel.h"

#include "JuceHeader.h"

using namespace Miam;


IModel::IModel()
{
}



void IModel::SendParamChange(AsyncParamChange& paramChange)
{
    if (!paramChangesToPresenter.push(paramChange))
        throw std::runtime_error("Lock-Free Queue Model-Presenter : cannot push an element...");
}





bool IModel::TryGetAsyncParamChange(AsyncParamChange& param_)
{
    return paramChangesToPresenter.pop(param_);
}
