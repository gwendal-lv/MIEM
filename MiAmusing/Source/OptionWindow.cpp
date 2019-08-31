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

#include "OptionWindow.h"
#include "MainComponent.h"
#include "AudioDeviceAndMidiOutputSelectorComponent.h"

using namespace Amusing;
OptionWindow::OptionWindow(MainContentComponent* _mainComponent)
{
	OKbutton = new TextButton();
	OKbutton->setButtonText("OK");
	OKbutton->addListener(this);
	addAndMakeVisible(OKbutton);

	cancelButton = new TextButton();
	cancelButton->setButtonText("Cancel");
	cancelButton->addListener(this);
	addAndMakeVisible(cancelButton);

	mainComponent = _mainComponent;
}

OptionWindow::~OptionWindow()
{
	delete OKbutton;
	delete cancelButton;
}

void OptionWindow::CompleteInitialization(std::shared_ptr<AudioDeviceManager> m_deviceManager)
{
	if (m_deviceManager == nullptr)
		DBG("nullptr");
	deviceManager = m_deviceManager;
	audioSetupComp = new AudioDeviceAndMidiOutputSelectorComponent(*m_deviceManager,
		0, 256, 0, 256, true, true, false);
	m_deviceManager->addChangeListener(this);
	addAndMakeVisible(audioSetupComp);
}

void OptionWindow::removeDeviceManager()
{
	audioSetupComp->removeAllChangeListener();
}

void OptionWindow::paint(Graphics& /*g*/)
{
	
}

void OptionWindow::resized()
{
	if (audioSetupComp)
	{
		audioSetupComp->setItemHeight(20);
		audioSetupComp->setBounds(0, 0, getWidth(), getHeight() - 30);
		audioSetupComp->setItemHeight(getHeight() / 20);
	}
	OKbutton->setBounds(getWidth()/4 - 25, getHeight()-20,50,20);
	cancelButton->setBounds(3 * getWidth() / 4 - 25, getHeight() - 20, 50, 20);
}

void OptionWindow::buttonClicked(Button * button)
{
	
	if (button == OKbutton)
		mainComponent->CloseOptionWindow();//DBG("OK clicked"); // dire au mainContentComponent de revenir aux scenes
	else if (button == cancelButton)
	{
		audioSetupComp->reloadPreviousState();
		mainComponent->CloseOptionWindow();
	}
}


void OptionWindow::changeListenerCallback(ChangeBroadcaster*)
{
	/*AudioIODevice* device = audioSetupComp->deviceManager.getCurrentAudioDevice();
	DBG("Current audio device: " + device->getName().quoted());
	DBG("Sample rate: " + String(device->getCurrentSampleRate()) + " Hz");
	DBG("Block size: " + String(device->getCurrentBufferSizeSamples()) + " samples");
	DBG("Output Latency: " + String(device->getOutputLatencyInSamples()) + " samples");
	DBG("Input Latency: " + String(device->getInputLatencyInSamples()) + " samples");
	DBG("Bit depth: " + String(device->getCurrentBitDepth()));
	DBG("Input channel names: " + device->getInputChannelNames().joinIntoString(", "));
	DBG("Active input channels: " + getListOfActiveBits(device->getActiveInputChannels()));
	DBG("Output channel names: " + device->getOutputChannelNames().joinIntoString(", "));
	DBG("Active output channels: " + getListOfActiveBits(device->getActiveOutputChannels()));*/
}

void OptionWindow::saveAudioDeviceCurrentState()
{
	audioSetupComp->saveAudioDeviceCurrentState();
}
