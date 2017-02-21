/*
  ==============================================================================

    Model.cpp
    Created: 9 Dec 2016 9:24:21am
    Author:  ayup1

  ==============================================================================
*/


#include "AmusingModel.h"
#include "Presenter.h"


using namespace Amusing;

AmusingModel::AmusingModel(Presenter* m_presenter) : presenter(m_presenter)
{
	DBG("AmusingModel::AmusingModel");
	//audioPlayer = new AudioPlayer();
	presenter->CompleteInitialisation(this);
	audioManager = new AudioManager(this);
}

AmusingModel::~AmusingModel()
{
	//DBG("Model destructor");
	//delete audioPlayer;
	delete audioManager;
}

bool AmusingModel::lookForParameter(Miam::AsyncParamChange &param)
{
	return presenter->TryGetAsyncParamChange(param);
}

AudioDeviceManager& AmusingModel::getAudioDeviceManager()
{
	if (audioManager == nullptr)
	{
		DBG("pas d'audioManager");
	}
	return audioManager->getAudioDeviceManager();
}