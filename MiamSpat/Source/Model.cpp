/*
  ==============================================================================

    Model.cpp
    Created: 24 Jan 2017 9:09:12am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Model.h"

#include "Presenter.h"


using namespace Miam;


Model::Model(Presenter* presenter_)
:
SpatModel(presenter_, 500.0),
presenter(presenter_) // own private downcasted pointer
{
    // Fin d'Initialisation des autres modules
    presenter->CompleteInitialisation(this);
}

Model::~Model()
{
}


void Model::update()
{
    while(continueUpdate)
    {
        updateThreadMeasurer.OnNewFrame();
        
        if (updateThreadMeasurer.IsFreshAverageAvailable())
            DBG(updateThreadMeasurer.GetInfo());
        
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        
        /*
        AsyncParamChange lastParamChange;
        while (presenter->TryGetAsyncParamChange(lastParamChange))
        {
            
        }
        */
    }
}
