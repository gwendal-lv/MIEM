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
    std::vector<std::string> emptyVector;
    miamOscSender->EnableSendFirstColOnly(true, emptyVector);
    
    continuousBackgroundBlobMatrixRefresh = false;
    // va fonctionner automatiquement avec les bonnes adresses, puisqu'on a configuré
    // le OSC sender comme il faut
    continuousBackgroundSingleMatrixCoeffRefresh = true;
}

void Model::SetConfigurationFromTree(bptree::ptree& tree)
{
    PlayerModel::SetConfigurationFromTree(tree);
    
    auto inOutNames = interpolator->GetInOutChannelsName();
    miamOscSender->EnableSendFirstColOnly(true, inOutNames.Inputs);
}
