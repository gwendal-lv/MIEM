/*
  ==============================================================================

    IPresenter.cpp
    Created: 21 Dec 2016 2:29:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <iostream>

#include "IPresenter.h"


using namespace Miam;

IPresenter::IPresenter()
:
constructionTimePt( std::chrono::steady_clock::now() )
{
    std::cout << "AsyncParamChange size = " << sizeof(AsyncParamChange) << "bytes" << std::endl;
}

