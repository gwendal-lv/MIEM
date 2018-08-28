/*
  ==============================================================================

    Model.cpp
    Created: 26 Nov 2016 2:19:48pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/




#include "Model.h"

#include "Presenter.h"

#include "MiamExceptions.h"

#include "boost/lexical_cast.hpp"



using namespace Miam;





// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
Model::Model(Presenter* presenter_)
:
    ControlModel(presenter_),

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
	// Joining of threads
	continueUpdate = false;
	updateThread.join();
    
    // Disconnection / Destruction of the OSC sender
}



// - - - - - Periodic updates - - - - -

void Model::update()
{
    while(continueUpdate)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
		//DBG("update Modèle !");
    }
}


// - - - - - Simple OSC sender (for devices OSC learn) - - - - -
void Model::ConnectAndSendOSCMessage(const std::string& oscAddressPattern, double argumentValue)
{
    // At first, we ask the interpolator for the current MiamOSCSender status and data
    std::shared_ptr<MiamOscSender<double>> miamOscSender = std::dynamic_pointer_cast<MiamOscSender<double>>(GetStateSender(0));
    if (! miamOscSender )
        throw std::logic_error("State sender has to be an OSC one for now.");
    
    // Then we open a connection if necessary (or we keep it alive if it was opened)
    if (! miamOscSender->TryConnect() )
        throw Miam::OscException(TRANS("Cannot connect to the device. Please check OSC settings.").toStdString());
    
    // Finally : OSC Message sending
    miamOscSender->SendToAddressAsFloat(oscAddressPattern, argumentValue);
}


