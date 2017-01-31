/*
  ==============================================================================

    Model.cpp
    Created: 26 Nov 2016 2:19:48pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/




#include "Model.h"

#include "Presenter.h"


#include "SpatSender.h"


using namespace Miam;





// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
Model::Model(Presenter* presenter_)
:
    SpatModel(presenter_),
presenter(presenter_)
{
    spatInterpolator = new SpatInterpolator<double>(this);
    
    // OCTOPHONIE POUR L'INSTANT
    for (size_t i = 0; i<8 ; i++)
    {
        AddSpeaker();
        speakers.back()->SetName("Octophonie " + std::to_string(i+1));
    }
    
    spatInterpolator->__AddDefaultStates();
    // PAS LE CHOIX POUR L'INSTANT
    spatType = SpatType::Volumes1d;
    
    // Auto-referenciation to other modules
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


    delete spatInterpolator;
}



// - - - - - Periodic updates - - - - -

void Model::update()
{
    while(continueUpdate)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
		DBG("update Modèle !");
    }
}



