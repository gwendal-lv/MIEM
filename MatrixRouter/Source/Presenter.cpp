/*
  ==============================================================================

    Presenter.cpp
    Created: 3 May 2017 3:16:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "Presenter.h"

#include "PluginProcessor.h" // class MatrixRouterAudioProcessor
#include "OscMatrixComponent.h"

#include "MatrixSlider.h" // min and max volumes

using namespace Miam;




// =================== Construction & destruction ===================
Presenter::Presenter(MatrixRouterAudioProcessor& _model)
    :
    model(_model)
{
    oscMatrixComponent = new OscMatrixComponent(this);
}

Presenter::~Presenter()
{
}





// =================== Periodic Update ===================

void Presenter::UpdateFromView(MatrixRouterAudioProcessorEditor* view)
{
    // Have new messages arrived ?
    AsyncParamChange newParamChange;
    while( model.TryGetAsyncParamChange(newParamChange) )
    {
        switch (newParamChange.Type)
        {
            case AsyncParamChange::InputsAndOutputsCount :
                std::cout << "=====================" << std::endl;
                std::cout << newParamChange.Id1 << "in " << newParamChange.Id2 << "out" << std::endl;
                std::cout << "=====================" << std::endl;
                break;
                
            default :
                break;
        }
    }
}


void Presenter::Update()
{
    throw std::runtime_error("Classic Update() from Presenter should not be called without specifying a View module (that may not exist when working with audio plug-ins");
}


// =================== Callbacks from View ===================

void Presenter::OnSliderValueChanged(int row, int col, double value)
{
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Volume;
    paramChange.Id1 = row;
    paramChange.Id2 = col;
    
    // We keep values over min+0.5dB only
    if (value < (MatrixSlider::GetMinVolume() + 0.5))
        paramChange.DoubleValue = 0.0;
    else
        paramChange.DoubleValue = std::pow(10.0, value/20.0);
        
    // Enqueuing
    SendParamChange(paramChange);
}


