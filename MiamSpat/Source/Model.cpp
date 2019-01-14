/*
  ==============================================================================

    Model.cpp
    Created: 13 Jan 2019 4:28:05pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Model.h"

#include "Presenter.h"
#include "AudioMatrix.hpp"

using namespace Miam;

Model::Model(Presenter* presenter_)
:
PlayerModel(presenter_), presenter(presenter_)
{
    
    presenter->CompleteInitialisation(this);
}



void Model::onUpdateStarts()
{
    
}
void Model::onUpdateFinished()
{
    // Here, we are going to compute the volume of the
    // current interpolated matrix (but at 10Hz max.)
    if (wasSomethingUpdated)
    {
        AsyncParamChange paramChange;
        
        CorrelationLevel spatEngineCorrelationLevel = CorrelationLevel::Undefined;
        if (interpolator->GetType() == InterpolationType::Matrix_ConstantPower)
            spatEngineCorrelationLevel = CorrelationLevel::Low;
        else if (interpolator->GetType() == InterpolationType::Matrix_ConstantAmplitude)
            spatEngineCorrelationLevel = CorrelationLevel::High;
        
        if (spatEngineCorrelationLevel != CorrelationLevel::Undefined)
        {
            // 1st volume is correlated, 2nd is decorrelated
            if ( MatrixState<double>* matrixState
                = dynamic_cast<MatrixState<double>*>(&(interpolator->GetCurrentInterpolatedState())) )
            {
                //matrixState->GetMatrix().
                /*
                 auto linearVolume1 = matrixState->ComputeTotalVolume(CorrelationLevel::High, spatEngineCorrelationLevel);
                 auto linearVolume2 = matrixState->ComputeTotalVolume(CorrelationLevel::Low, spatEngineCorrelationLevel);
                 */
            }
            else {
                throw std::logic_error("Cannot send the volume of a state that is not a matrix state.");
            }
        }
    }
}
