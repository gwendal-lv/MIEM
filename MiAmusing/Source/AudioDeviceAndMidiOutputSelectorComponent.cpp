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

#include "AudioDeviceAndMidiOutputSelectorComponent.h"

AudioDeviceAndMidiOutputSelectorComponent::AudioDeviceAndMidiOutputSelectorComponent(AudioDeviceManager& dm,
	const int minInputChannels_,
	const int maxInputChannels_,
	const int minOutputChannels_,
	const int maxOutputChannels_,
	const bool showMidiOutputSelector,
	const bool showChannelsAsStereoPairs_,
	const bool hideAdvancedOptionsWithButton_)
{
	audioDeviceSelectorComponent = new AudioDeviceSelectorComponent(dm, minInputChannels_, maxInputChannels_, minOutputChannels_, maxOutputChannels_,
		false, showMidiOutputSelector, showChannelsAsStereoPairs_, hideAdvancedOptionsWithButton_);
	itemHeight = 24;
	midiOutputSelectorId = 2;
	
	audioDeviceSelectorComponent->getChildComponent(midiOutputSelectorId)->setVisible(false);
	
	internalSynthButton = new ToggleButton();
	internalSynthButton->setButtonText("use Internal Synth");
	internalSynthButton->addListener(this);
	internalSynthButtonState = true;
	internalSynthButton->setToggleState(internalSynthButtonState, false);
	
	addAndMakeVisible(audioDeviceSelectorComponent);
	addAndMakeVisible(internalSynthButton);
}

AudioDeviceAndMidiOutputSelectorComponent::~AudioDeviceAndMidiOutputSelectorComponent()
{
	delete audioDeviceSelectorComponent;
	delete internalSynthButton;
}

void AudioDeviceAndMidiOutputSelectorComponent::setMidiOutputSelectorIsVisible(bool isVisible)
{
	audioDeviceSelectorComponent->getChildComponent(midiOutputSelectorId)->setVisible(isVisible);
}

void AudioDeviceAndMidiOutputSelectorComponent::buttonStateChanged(Button *buttonStateChanged)
{
	if (buttonStateChanged == internalSynthButton && internalSynthButton->getToggleState() != internalSynthButtonState)
	{
		//DBG("toggle button state " + (String)int(internalSynthButton->getToggleState()));
		internalSynthButtonState = internalSynthButton->getToggleState();
		if (internalSynthButtonState)
		{
			// bouton On : on utilise le synthé interne -> pas besoin de montrer le midiOutputSelector
			audioDeviceSelectorComponent->getChildComponent(midiOutputSelectorId)->setVisible(false);
			audioDeviceSelectorComponent->deviceManager.setDefaultMidiOutput(String());
		}
		else
		{
			const int space = itemHeight / 4;
			audioDeviceSelectorComponent->getChildComponent(midiOutputSelectorId)->setVisible(true);
		}
	}
}

void AudioDeviceAndMidiOutputSelectorComponent::buttonClicked(Button* /*clickedButton*/)
{

}

void AudioDeviceAndMidiOutputSelectorComponent::resized()
{
	Rectangle<int> r(proportionOfWidth(0.35f), 15, proportionOfWidth(0.6f), 3000);
	const int space = itemHeight / 4;
	internalSynthButton->setBounds(proportionOfWidth(0.35f), 3*space, proportionOfWidth(0.6f), itemHeight);
	audioDeviceSelectorComponent->setBounds(0, 3 * space + itemHeight + space, getWidth(), getHeight() - (itemHeight + space));
	
	
}

void AudioDeviceAndMidiOutputSelectorComponent::setItemHeight(int newItemHeight)
{
	itemHeight = newItemHeight;
	audioDeviceSelectorComponent->setItemHeight(itemHeight);
}

void AudioDeviceAndMidiOutputSelectorComponent::saveAudioDeviceCurrentState()
{
	audioDeviceSelectorComponent->deviceManager.getAudioDeviceSetup(savedState);
	internalSynthButtonStateSaved = internalSynthButton;
	savedAudioDeviceType = audioDeviceSelectorComponent->deviceManager.getCurrentAudioDeviceType();
}

void AudioDeviceAndMidiOutputSelectorComponent::reloadPreviousState()
{
	audioDeviceSelectorComponent->deviceManager.setAudioDeviceSetup(savedState,false);
	audioDeviceSelectorComponent->deviceManager.setCurrentAudioDeviceType(savedAudioDeviceType, false);
	internalSynthButton->setToggleState(internalSynthButtonStateSaved,false);

	if (internalSynthButtonStateSaved)
	{
		// bouton On : on utilise le synthé interne -> pas besoin de montrer le midiOutputSelector
		audioDeviceSelectorComponent->getChildComponent(midiOutputSelectorId)->setVisible(false);
		audioDeviceSelectorComponent->deviceManager.setDefaultMidiOutput(String());
	}
	else
	{
		const int space = itemHeight / 4;
		audioDeviceSelectorComponent->getChildComponent(midiOutputSelectorId)->setVisible(true);
	}
}

void AudioDeviceAndMidiOutputSelectorComponent::removeAllChangeListener()
{
	audioDeviceSelectorComponent->deviceManager.removeAllChangeListeners();
}
