/*
  ==============================================================================

    SettingsManager.cpp
    Created: 25 May 2017 11:09:10am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <iostream>

#include "SettingsManager.h"
#include "MiamOscSender.hpp"

#include "View.h"
#include "Model.h"


using namespace Miam;



SettingsManager::SettingsManager(View* _view, Presenter* _presenter)
:
view(_view),
configurationComponent(view->GetMainContentComponent()->GetConfigurationComponent()),
presenter(_presenter)
{
}
void SettingsManager::CompleteInitialisation(Model* _model)
{
    model = _model;
    
    // Links on the view side
    configurationComponent->settingsManager = this;
    
    // Update from model's contructed data
    
    // Ready to actually process inputs
    linksInitialized = true;
}

void SettingsManager::ConfigureGuiForSessionPurpose()
{
    switch (GetSessionPurpose())
    {
        case AppPurpose::Spatialisation :
            
            configurationComponent->inputNamesToggleButton->setToggleState(false, NotificationType::dontSendNotification);
            configurationComponent->outputNamesToggleButton->setToggleState(false, NotificationType::dontSendNotification);
            // on ne peut pas (ne doit pas...) faire confiance à la vue pour re-déclencher des callbacks !
            OnInOutNamesDisplayedChanged(false, false);
            // UDP / IP
            configurationComponent->ipAddressLabel->setText(TRANS("Plug-in host IP address:"), NotificationType::sendNotification);
            configurationComponent->udpPortLabel->setText(TRANS("Plug-in listening on UDP port:"), NotificationType::sendNotification);
            // Seuls certains types d'interpolation sont dispo
            configurationComponent->SetAvailableInterpolations({
                InterpolationType::Matrix_ConstantPower,
                InterpolationType::Matrix_ConstantAmplitude
            });
            configurationComponent->SetInterpolationChoiceVisible(true);
            // On active bien la possibilité de choisir keyboard edition, ou non
            configurationComponent->keyboardToggleButton->setVisible(true);
            // Master gain : possible de choisir aussi
            configurationComponent->enableMasterVolumeToggleButton->setVisible(true);
            break;
            
            
        case AppPurpose::GenericController :
            configurationComponent->inputNamesToggleButton->setToggleState(true, NotificationType::dontSendNotification);
            // on ne peut pas (ne doit pas...) faire confiance à la vue pour re-déclencher des callbacks !
            OnInOutNamesDisplayedChanged(true, false);
            // UDP / IP
            configurationComponent->ipAddressLabel->setText(TRANS("Target device IP address:"), NotificationType::sendNotification);
            configurationComponent->udpPortLabel->setText(TRANS("Target device listening on UDP port:"), NotificationType::sendNotification);
            // Seuls certains types d'interpolation sont dispo
            // Avec les interps. par paramètre, on n'aura que l'interpolation linéaire dispo en
            // interne
            configurationComponent->SetAvailableInterpolations({
                InterpolationType::Matrix_Linear
            });
            // et du coup, puisque pas le choix, on cache
            configurationComponent->SetInterpolationChoiceVisible(false);
            // On désactive bien la possibilité de choisir keyboard edition !
            // Car on peut toujours le faire avec les sliders... Donc ça n'a pas de sens...
            configurationComponent->keyboardToggleButton->setVisible(false);
            // MASTER GAIN DESACTIVE POUR l'INSTANT POUR GENERIC CONTROLLER
            configurationComponent->enableMasterVolumeToggleButton->setToggleState(false,
                                                                                   NotificationType::dontSendNotification);
            configurationComponent->enableMasterVolumeToggleButton->setVisible(false);
            break;
        default :
            break;
    }
}



// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =


AppPurpose SettingsManager::GetSessionPurpose()
{
    return model->GetSessionPurpose();
}



// - - - - - Callbacks from the Presenter - - - - -

void SettingsManager::OnEnterSettingsEdition()
{
    // On force l'actualisation graphique des choses qui changent d'apparence selon la
	// validité des données
	configurationComponent->TryParseIpAddress();
	configurationComponent->TryParseUdpPort();
}
void SettingsManager::OnLeaveSettingsEdition()
{
    // Mise à jour des paramètres nécessaires (pas ceux qui réagissent direct à l'utilisateur)
    
    int udpPort = configurationComponent->TryParseUdpPort();
    if (udpPort != -1)
        OnUdpPortChanged(udpPort);
    else
        configurationComponent->udpPortTextEditor->setText("");
    
    std::string ipAddress = configurationComponent->TryParseIpAddress();
    if (!ipAddress.empty())
        OnIpAddressChanged(ipAddress);
    else
        configurationComponent->ipAddressTextEditor->setText("");
}



// - - - - - Callbacks from View to Model, retransmitted - - - - -

void SettingsManager::OnInOutChannelsCountChanged(int inputsCount, int outputsCount)
{
    if (!linksInitialized)
        return;
    
    // model updates
    
    // Might not validate the change... We don't know !
    model->GetInterpolator()->SetInputOutputChannelsCount(inputsCount, outputsCount);
    
    // Direct dispatching of the event (not telling other presenter modules)
    view->GetMainContentComponent()->GetSpatStatesEditionComponent()->SetInsOutsCount(
        model->GetInterpolator()->GetInputsCount(),
        model->GetInterpolator()->GetOutputsCount());
}
void SettingsManager::OnInOutNamesDisplayedChanged(bool areInputNamesVisible, bool areOutputNamesVisible)
{
    if (!linksInitialized)
        return;
    
    // View updates only
    presenter->GetSpatStatesManager()->OnInOutNamesDisplayedChanged(areInputNamesVisible,
                                                                    areOutputNamesVisible);
}
void SettingsManager::OnInterpolationTypeChanged(InterpolationType type)
{
    model->GetInterpolator()->ReinitInterpolation(type);
}
void SettingsManager::OnMasterGainEnabledChanged(bool isEnabled)
{
    model->SetIsMasterGainEnabled(isEnabled);
}
void SettingsManager::OnAllowKeyboardEdition(bool allow)
{
    presenter->GetSpatStatesManager()->AllowKeyboardEdition(allow);
}

void SettingsManager::OnUdpPortChanged(int udpPort)
{
    if (!linksInitialized)
        return;
    
    auto miamOscSender = std::dynamic_pointer_cast<MiamOscSender<double>>(model->GetStateSender(0));
    if (miamOscSender)
        miamOscSender->SetUdpPort(udpPort);
    else
        throw std::logic_error("StateSender[0] is not a MiamOscSender: cannot set UDP port");
}
void SettingsManager::OnIpAddressChanged(std::string ipAddress)
{
    if (!linksInitialized)
        return;
    
    auto miamOscSender = std::dynamic_pointer_cast<MiamOscSender<double>>( model->GetStateSender(0) );
    if (miamOscSender)
        miamOscSender->SetIpv4(ipAddress);
    else
        throw std::logic_error("StateSender[0] is not a MiamOscSender: cannot set IPv4 address");
}




// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> SettingsManager::GetTree()
{
    // This will be the internal settings tree for the whole Presenter (which does not
    // manage its config itself)
    auto settingsTree = std::make_shared<bptree::ptree>();
    settingsTree->add( "keyboardedition", (bool) configurationComponent->keyboardToggleButton->getToggleState() );
    settingsTree->add( "exciters.<xmlattr>.constraint_positions", (bool) configurationComponent->constraintExcitersToggleButton->getToggleState() );
    return settingsTree;
}
void SettingsManager::SetFromTree(bptree::ptree& tree)
{
    // Presenter (self)-related
    // may be absent
    try {
        // Actual Editor Settings
        bool allow = tree.get<bool>("presenter.keyboardedition");
        configurationComponent->keyboardToggleButton->setToggleState(allow,
                                        NotificationType::dontSendNotification);
        presenter->GetSpatStatesManager()->AllowKeyboardEdition(allow);
        
        // Presenter touch behavior related -> also simple data trans. to view
        bool shouldConstraintPositions = tree.get<bool>("presenter.exciters.<xmlattr>.constraint_positions", false); // default = false
        configurationComponent->constraintExcitersToggleButton->setToggleState(shouldConstraintPositions,
                                           NotificationType::dontSendNotification);
    
        // Interpolator-related : simple transmission of data to View,
        // with some delayed notifications
        std::string typeString = tree.get<std::string>("control.interpolation.<xmlattr>.type");
        InterpolationType interpolationType = InterpolationTypes::ParseName(typeString);
        configurationComponent->interpolationTypeComboBox->setSelectedId((int)interpolationType,
                                                        NotificationType::dontSendNotification);
        // Here, no notification would be sent if there is only 1 in or out (slider would not move...)
        // -> forced fake move of slider to trigger callbacks
        int inCount = tree.get<int>("control.inputs.<xmlattr>.activeCount");
        int outCount = tree.get<int>("control.outputs.<xmlattr>.activeCount");
        configurationComponent->inputsCountSlider->setValue( (inCount >= 2) ? 1.0 : 2.0,
                                                            NotificationType::dontSendNotification);
        configurationComponent->inputsCountSlider->setValue( (double) inCount,
                                                            NotificationType::sendNotificationAsync);
        configurationComponent->outputsCountSlider->setValue( (outCount >= 2) ? 1.0 : 2.0,
                                                            NotificationType::dontSendNotification);
        configurationComponent->outputsCountSlider->setValue( (double) outCount,
                                                            NotificationType::sendNotificationAsync);
        // Spat sender-related
        configurationComponent->ipAddressTextEditor->
        setText( tree.get<std::string>("control.senders.sender.ip") );
        configurationComponent->udpPortTextEditor->
        setText( boost::lexical_cast<std::string>(tree.get<int>("control.senders.sender.udp.port")) );
        
        // At this point : the MODEL (and its config) are fully set from their trees
        // So we can use their value instead of reading them again...
        configurationComponent->enableMasterVolumeToggleButton->setToggleState(model->GetIsMasterGainEnabled(),
                                                                               NotificationType::dontSendNotification);
    }
    catch (bptree::ptree_error& e) {
        throw XmlReadException(std::string("Cannot read a necessary settings tag: ") + e.what());
    }
    
    // à la fin du chargement : on met les données par défaut pour certains paramètres (channels names
    // cachés ou pas, ...) qui ne sont pas précisés dans le XML
    ConfigureGuiForSessionPurpose();
}


