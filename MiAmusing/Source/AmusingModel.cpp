/*
  ==============================================================================

    Model.cpp
    Created: 9 Dec 2016 9:24:21am
    Author:  ayup1

  ==============================================================================
*/


#include "AmusingModel.h"
#include "Presenter.h"

#include <string>
#include <map>


using namespace Amusing;

AmusingModel::AmusingModel(Presenter* m_presenter) : presenter(m_presenter)
{
	DBG("AmusingModel::AmusingModel");
	//audioPlayer = new AudioPlayer();
	
	sharedAudioDeviceManager = std::shared_ptr<AudioDeviceManager>(new AudioDeviceManager());
	audioManager = new AudioManager(this);

	/*for (int i = 0; i < sharedAudioDeviceManager->getAvailableDeviceTypes().size(); ++i)
	{
		DBG("Type : " + sharedAudioDeviceManager->getAvailableDeviceTypes()[i]->getTypeName());
		StringArray outs = sharedAudioDeviceManager->getAvailableDeviceTypes()[i]->getDeviceNames(false);
		for (int j = 0; j < outs.size(); ++j)
		{
			DBG(outs[j]);
		}
	}*/


	sharedAudioDeviceManager->initialise(2, 2, nullptr, true);

	
	AudioDeviceManager::AudioDeviceSetup audioSetup;
	sharedAudioDeviceManager->getAudioDeviceSetup(audioSetup);
	
	if (!sharedAudioDeviceManager->getCurrentAudioDevice()->isOpen())
	{
		for (int i = 0; i < sharedAudioDeviceManager->getAvailableDeviceTypes().size(); ++i)
		{
			StringArray outs = sharedAudioDeviceManager->getAvailableDeviceTypes()[i]->getDeviceNames(false);
			if (outs.size() != 0)
			{
				audioSetup.outputDeviceName = outs[0];
				sharedAudioDeviceManager->setAudioDeviceSetup(audioSetup,false);
				break;
			}
		}
	}
	
	sharedAudioDeviceManager->addAudioCallback(audioManager);
	presenter->CompleteInitialisation(this);
	//presenter->setAudioDeviceManager(audioManager);
}

AmusingModel::~AmusingModel()
{
	sharedAudioDeviceManager->removeAudioCallback(audioManager);
	sharedAudioDeviceManager->closeAudioDevice();
	//delete audioManager;
	
	audioManager = nullptr;
}

bool AmusingModel::lookForParameter(Miam::AsyncParamChange &param)
{
	return presenter->TryGetAsyncParamChange(param);
}

//AudioDeviceManager& AmusingModel::getAudioDeviceManager()
//{
//	if (audioManager == nullptr)
//		DBG("No audioManager");
//	return audioManager->getAudioDeviceManager();
//}

void AmusingModel::removeDeviceManagerFromOptionWindow()
{
	presenter->removeDeviceManagerFromOptionWindow();
}

void Amusing::AmusingModel::addNewSoundPath(int idx, std::string newPath)
{
	//idxToPath.at(idx) = newPath;
	idxToPath[idx] = newPath;

}

String Amusing::AmusingModel::getSoundPath(int idx)
{
	return String(idxToPath[idx]);
}

MidiOutput* AmusingModel::getMidiOutput()
{
	return sharedAudioDeviceManager->getDefaultMidiOutput();
}