/*
  ==============================================================================

    IPresenter.cpp
    Created: 21 Dec 2016 2:29:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <iostream>

#include "IPresenter.h"
#include "IModel.h"


using namespace Miam;

IPresenter::IPresenter()
:
constructionTimePt( std::chrono::steady_clock::now() )
{
    std::cout << "AsyncParamChange size = " << sizeof(AsyncParamChange) << "bytes" << std::endl;
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
