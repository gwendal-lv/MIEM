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
Model::Model(Presenter* _presenter) :
    SpatModel(_presenter)
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
}


Model::~Model()
{
    delete spatInterpolator;
}
