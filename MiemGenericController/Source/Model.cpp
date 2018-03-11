/*
  ==============================================================================

    Model.cpp
    Created: 10 Mar 2018 12:28:48am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Model.h"

#include "Presenter.h"

using namespace Miam;

Model::Model(Presenter* presenter_)
:
PlayerModel(presenter_),
presenter(presenter_)
{
    continuousBackgroundBlobMatrixRefresh = false;
    
    std::vector<std::string> emptyVector;
    miamOscSender->EnableSendFirstColOnly(true, emptyVector);
}

void Model::SetConfigurationFromTree(bptree::ptree& tree)
{
    PlayerModel::SetConfigurationFromTree(tree);
    
    auto inOutNames = interpolator->GetInOutChannelsName();
    miamOscSender->EnableSendFirstColOnly(true, inOutNames.Inputs);
}
