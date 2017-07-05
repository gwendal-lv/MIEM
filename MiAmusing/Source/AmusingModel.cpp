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
	
	audioManager = new AudioManager(this);
	presenter->CompleteInitialisation(this);
	//presenter->setAudioDeviceManager(audioManager);
}

AmusingModel::~AmusingModel()
{
	
	//delete audioManager;
	audioManager = nullptr;
}

bool AmusingModel::lookForParameter(Miam::AsyncParamChange &param)
{
	return presenter->TryGetAsyncParamChange(param);
}

AudioDeviceManager& AmusingModel::getAudioDeviceManager()
{
	if (audioManager == nullptr)
		DBG("No audioManager");
	return audioManager->getAudioDeviceManager();
}

void AmusingModel::removeDeviceManagerFromOptionWindow()
{
	presenter->removeDeviceManagerFromOptionWindow();
}