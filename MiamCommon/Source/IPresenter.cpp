/*
  ==============================================================================

    IPresenter.cpp
    Created: 21 Dec 2016 2:29:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "IPresenter.h"

#include <iostream>
using namespace Miam;

IPresenter::IPresenter() : paramChangesToModel()
{
    std::cout << "async param change size : " << sizeof(AsyncParamChange) << std::endl;
}

void IPresenter::SendParamChange(AsyncParamChange& paramChange)
{
	if (!paramChangesToModel.TryEnqueue(paramChange))
		throw std::runtime_error("Lock-Free Queue Presenter->Model is full...");
    
    AsyncParamChange dummyParam;
    TryGetAsyncParamChange(dummyParam);
}

