/*
  ==============================================================================

    Model.cpp
    Created: 26 Nov 2016 2:19:48pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/




#include "Model.h"

#include "Presenter.h"




using namespace Miam;





// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction / destruction - - - - -
Model::Model(Presenter* presenter_, std::string commandLine)
:
    SpatModel(presenter_),
presenter(presenter_)
{
    // Choice of interpolation type
    spatInterpolator = std::make_shared<SpatInterpolator<double>>(SpatType::RoutingMatrix);
    
    // OCTOPHONIE POUR L'INSTANT
    for (size_t i = 0; i<8 ; i++)
    {
        AddSpeaker();
        speakers.back()->SetName("Octophonie " + std::to_string(i+1));
    }
    
    
    // Auto-referenciation to other modules
    presenter->CompleteInitialisation(this, commandLine);


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



