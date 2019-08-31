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

#include "Model.h"

#include "Presenter.h"
#include "AudioMatrix.hpp"

using namespace Miam;

Model::Model(Presenter* presenter_)
:
PlayerModel(presenter_), presenter(presenter_)
{
    presenter->CompleteInitialisation(this);
    
    continuousBackgroundBlobMatrixRefresh = true;
    continuousBackgroundSingleMatrixCoeffRefresh = false;
    
    // at the very end of the construction
    launchUpdateThread();
}



void Model::onUpdateStarts()
{
    
}
void Model::onUpdateFinished()
{
    durationSinceLastInfoToPresenter_ms += this->updateThreadMeasurer.GetLastDuration_ms();
    
    // Here, we are going to compute the volume of the
    // current interpolated matrix
    if (wasSomethingUpdated)
    {
        // (but at 10Hz max.)
        if (durationSinceLastInfoToPresenter_ms > presenterRefreshPeriodMin_ms)
        {
            durationSinceLastInfoToPresenter_ms = 0.0;
            
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
                    double lowCorrVolume = matrixState->ComputeMatrixTotalVolume(CorrelationLevel::Low, spatEngineCorrelationLevel);
                    double highCorrVolume = matrixState->ComputeMatrixTotalVolume(CorrelationLevel::High, spatEngineCorrelationLevel);
                    
                    paramChange.DoubleValue = lowCorrVolume;
                    paramChange.Type = AsyncParamChange::ParamType::Volume_DecorrelatedInputs;
                    SendParamChange(paramChange);
                    paramChange.DoubleValue = highCorrVolume;
                    paramChange.Type = AsyncParamChange::ParamType::Volume_CorrelatedInputs;
                    SendParamChange(paramChange);
                }
                else {
                    throw std::logic_error("Cannot send the volume of a state that is not a matrix state.");
                }
            }
        }
    }
}
