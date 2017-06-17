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





void SettingsManager::OnInOutChannelsCountChanged(int inputsCount, int outputsCount)
{
    if (!linksInitialized)
        return;
    
    // model updates
    
    // Might not validate the change... We don't know !
    model->GetSpatInterpolator()->SetInputOuputChannelsCount(inputsCount, outputsCount);
    
    // Direct dispatching of the event (not telling other presenter modules)
    view->GetMainContentComponent()->GetSpatStatesEditionComponent()->SetInsOutsCount(model->GetSpatInterpolator()->GetInputsCount(),
         model->GetSpatInterpolator()->GetOutputsCount());
}
void SettingsManager::OnAllowKeyboardEdition(bool allow)
{
    presenter->GetSpatStatesManager()->AllowKeyboardEdition(allow);
}
void SettingsManager::OnUdpPortChanged(int udpPort)
{
    if (!linksInitialized)
        return;
    
    auto miamOscSender = std::dynamic_pointer_cast<MiamOscSender<double>>(model->GetSpatSender(0));
    if (miamOscSender)
        miamOscSender->SetUdpPort(udpPort);
    else
        throw std::logic_error("SpatSender[0] is not a MiamOscSender: cannot set UDP port");
}
void SettingsManager::OnIpAddressChanged(std::string ipAddress)
{
    if (!linksInitialized)
        return;
    
    auto miamOscSender = std::dynamic_pointer_cast<MiamOscSender<double>>( model->GetSpatSender(0) );
    if (miamOscSender)
        miamOscSender->SetIpv4(ipAddress);
    else
        throw std::logic_error("SpatSender[0] is not a MiamOscSender: cannot set IPv4 address");
}




// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> SettingsManager::GetTree()
{
    auto settingsTree = std::make_shared<bptree::ptree>();
    settingsTree->add( "keyboardedition", (bool) configurationComponent->keyboardToggleButton->getToggleState() );
    return settingsTree;
}
void SettingsManager::SetFromTree(bptree::ptree& tree)
{
    // Presenter (self)-related
    // may be absent
    try {
        bool allow = tree.get<bool>("presenter.keyboardedition");
        configurationComponent->keyboardToggleButton->setToggleState(allow, NotificationType::dontSendNotification);
        presenter->GetSpatStatesManager()->AllowKeyboardEdition(allow);
    } catch (std::exception) {}
    
    // Interpolator-related
    configurationComponent->inputsCountSlider->
    setValue((double) tree.get<int>("model.inputs.<xmlattr>.count") );
    configurationComponent->outputsCountSlider->
    setValue((double) tree.get<int>("model.outputs.<xmlattr>.count") );
    // Spat sender-related
    configurationComponent->ipAddressTextEditor->
    setText( tree.get<std::string>("model.senders.sender.ip") );
    configurationComponent->udpPortTextEditor->
    setText( std::to_string(tree.get<int>("model.senders.sender.udp.port")) );
}


