/*
  ==============================================================================

    OptionWindow.cpp
    Created: 5 Jul 2017 11:00:48am
    Author:  ayup1

  ==============================================================================
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

	mainComponent = _mainComponent;
}

OptionWindow::~OptionWindow()
{
	delete OKbutton;
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
	audioSetupComp->deviceManager.removeAllChangeListeners();
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
	}
	OKbutton->setBounds(getWidth()/2 - 25, getHeight()-20,50,20);
}

void OptionWindow::buttonClicked(Button * button)
{
	if (button == OKbutton)
		mainComponent->CloseOptionWindow();//DBG("OK clicked"); // dire au mainContentComponent de revenir aux scenes
}


void OptionWindow::changeListenerCallback(ChangeBroadcaster*)
{
	AudioIODevice* device = audioSetupComp->deviceManager.getCurrentAudioDevice();
	DBG("Current audio device: " + device->getName().quoted());
	DBG("Sample rate: " + String(device->getCurrentSampleRate()) + " Hz");
	DBG("Block size: " + String(device->getCurrentBufferSizeSamples()) + " samples");
	DBG("Output Latency: " + String(device->getOutputLatencyInSamples()) + " samples");
	DBG("Input Latency: " + String(device->getInputLatencyInSamples()) + " samples");
	DBG("Bit depth: " + String(device->getCurrentBitDepth()));
	DBG("Input channel names: " + device->getInputChannelNames().joinIntoString(", "));
	DBG("Active input channels: " + getListOfActiveBits(device->getActiveInputChannels()));
	DBG("Output channel names: " + device->getOutputChannelNames().joinIntoString(", "));
	DBG("Active output channels: " + getListOfActiveBits(device->getActiveOutputChannels()));
}