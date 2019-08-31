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

#include "MiamExceptions.h"

#include "boost/lexical_cast.hpp"

#ifndef JUCE_WINDOWS
#include <pthread.h>
#endif


using namespace Miam;





// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
Model::Model(Presenter* presenter_)
:
    ControlModel(presenter_, false), // don't start update thread in parent ctor

presenter(presenter_),
sessionPurpose(AppPurpose::None)
{
    // Après construction des classes parentes : référencement aux autres modules
    presenter->CompleteInitialisation(this);
    
	// Launch of thread, at the specified frequency
	continueUpdate = true;
	// Using a c++11 lambda function for class member calling
	updateThread = std::thread( [this] {this->update();} );
}


Model::~Model()
{
}



// - - - - - Periodic updates - - - - -

void Model::update()
{
#ifndef JUCE_WINDOWS // toutes plateformes POSIX 
    pthread_setname_np(/*pthread_self(), */"MIEM Model::update Thread"); // pas de TID pour FreeBSD 2003... (doc macOS)
#endif
    
    ControlModel::update();
}

// - - - - - internal events - - - - -
void Model::onPlay()
{
    // parent class work
    ControlModel::onPlay();
}

// - - - - - Simple OSC sender (for devices OSC learn) - - - - -
void Model::ConnectAndSendOSCMessage(const std::string& oscAddressPattern, double argumentValue)
{
    tryConnectAndGetOscSender();
    
    // Finally : OSC Message sending
    miamOscSender->SendToAddressAsFloat(oscAddressPattern, argumentValue);
}

/// - - - - - simple OSC sender for a full state - - - - -
void Model::ConnectAndSendState(std::shared_ptr<ControlState<double>> stateToSend, int rowToSend)
{
    tryConnectAndGetOscSender();
    
    // Tentative de conversion de l'état : doit TOUJOURS être un matrix pour l'instant
    auto castedMatrixState = std::dynamic_pointer_cast<MatrixState<double>>(stateToSend);
    if (!castedMatrixState)
        throw std::logic_error("Not implemented : Control State has to be actually a Matrix State....");
    
    // Comportement différent selon GEN CON
    if (sessionPurpose == AppPurpose::GenericController)
    {
        // d'abord, on force la ré-activation des adresses OSC et du mode OSC à 1 colonne
        miamOscSender->EnableSendFirstColOnly(true,
                                              interpolator->GetInOutChannelsName().Inputs);
        
        // création de la liste de tous les indices à envoyer
        std::vector<size_t> allIndexes;
        if (rowToSend == -1)
        {
            for (size_t i=0 ; i<castedMatrixState->GetInputsCount() ; i++)
                allIndexes.push_back( castedMatrixState->GetMatrix()
                                     ->GetIndexFromIndex2d(Index2d(i, 0)) );
        }
        else if (rowToSend >= 0)
        {
            allIndexes.push_back( castedMatrixState->GetMatrix()
                                 ->GetIndexFromIndex2d(Index2d(rowToSend, 0)) );
        }
        // commande d'envoi
        //assert(false); // désactivé pour l'instant !!!!! à remettre en marche
        // --> car l'état doit maintenant absolument être un état de backup (qui
        // connaît toutes les courbes d'interpolation et a initialisé tous les états internes)
        auto artificialMatrixBackupState = std::make_unique<MatrixBackupState<double>>
        (*castedMatrixState, interpolator->GetInterpolationCurves());
#ifdef __MIAM_DEBUG
        artificialMatrixBackupState->DisplayMatrixInStdCout();
#endif
        miamOscSender->SendMatrixParamChanges(artificialMatrixBackupState.get(), allIndexes);
    }
    // ou SPAT
    else if (sessionPurpose == AppPurpose::Spatialisation)
    {
        // on désactive d'abord le mode à adresses OSC paramétrables
        miamOscSender->EnableSendFirstColOnly(false);
        
        // création de la liste de tous les indices à envoyer
        std::vector<size_t> allIndexes;
        allIndexes.reserve(castedMatrixState->GetInputsCount() * castedMatrixState->GetOutputsCount());
        for (size_t i=0 ; i<castedMatrixState->GetInputsCount() ; i++)
            for (size_t j=0 ; j<castedMatrixState->GetOutputsCount() ; j++)
                allIndexes.push_back(castedMatrixState->GetMatrix()->GetIndexFromIndex2d(Index2d(i,j)));
        // commande d'envoi (à tester avec Reaper)
        miamOscSender->SendMatrixCoeffChanges(castedMatrixState.get(), allIndexes);
    }
}

std::shared_ptr<MiamOscSender<double>> Model::tryConnectAndGetOscSender()
{
    // At first, we ask the interpolator for the current MiamOSCSender status and data
    miamOscSender = std::dynamic_pointer_cast<MiamOscSender<double>>(GetStateSender(0));
    if (! miamOscSender )
        throw std::logic_error("State sender has to be an OSC one for now.");
    
    // Then we open a connection if necessary (or we keep it alive if it was opened)
    if (! miamOscSender->TryConnect() )
        throw Miam::OscException(TRANS("Cannot connect to the device. Please check OSC settings.").toStdString());
    
    
    // Configuration depending on the session
    // SHOULD BE MOVED ELSEWHERE
    switch (sessionPurpose) {
    case AppPurpose::Spatialisation:
        miamOscSender->EnableSendFirstColOnly(false);
        continuousBackgroundSingleMatrixCoeffRefresh = false;
        continuousBackgroundBlobMatrixRefresh = true;
        break;
        
    case AppPurpose::GenericController:
        miamOscSender->EnableSendFirstColOnly(true,
                                              interpolator->GetInOutChannelsName().Inputs);
        continuousBackgroundSingleMatrixCoeffRefresh = true;
        continuousBackgroundBlobMatrixRefresh = false;
        break;
        
    default:
        break;
    }
    
    return miamOscSender;
}


