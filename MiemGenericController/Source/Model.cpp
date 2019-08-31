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

using namespace Miam;

Model::Model(Presenter* presenter_)
:
PlayerModel(presenter_),
presenter(presenter_)
{
    std::vector<std::string> emptyVector;
    miamOscSender->EnableSendFirstColOnly(true, emptyVector);
    
    presenter->CompleteInitialisation(this);
    
    continuousBackgroundBlobMatrixRefresh = false;
    // va fonctionner automatiquement avec les bonnes adresses, puisqu'on a configuré
    // le OSC sender comme il faut
    continuousBackgroundSingleMatrixCoeffRefresh = true;
    
    // at the very end of the construction
    launchUpdateThread();
}

void Model::SetConfigurationFromTree(bptree::ptree& tree)
{
    PlayerModel::SetConfigurationFromTree(tree);
    
    auto inOutNames = interpolator->GetInOutChannelsName();
    miamOscSender->EnableSendFirstColOnly(true, inOutNames.Inputs);
}
