/*
  ==============================================================================

    Model.cpp
    Created: 9 Dec 2016 9:24:21am
    Author:  ayup1

  ==============================================================================
*/

#include "Model.h"
#include "Presenter.h"
#include "AudioPlayer.h"

using namespace Miam;

Model::Model(Presenter* _presenter) : IModel(_presenter)
{
	audioPlayer = new AudioPlayer();
	presenter->CompleteInitialisation(this);
}

Model::~Model()
{
	DBG("Model destructor");
	delete audioPlayer;
}