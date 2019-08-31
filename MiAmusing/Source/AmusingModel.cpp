/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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


	sharedAudioDeviceManager->initialise(0, 2, nullptr, true);

	
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
	if (String(idxToPath[idx]).isEmpty())
		DBG("deja vide ici");
	return String(idxToPath[idx]);
}

MidiOutput* AmusingModel::getMidiOutput()
{
	return sharedAudioDeviceManager->getDefaultMidiOutput();
}