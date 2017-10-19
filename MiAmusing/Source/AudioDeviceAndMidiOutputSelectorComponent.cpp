/*
  ==============================================================================

    AudioDeviceAndMidiOutputSelectorComponent.cpp
    Created: 16 Oct 2017 11:02:01am
    Author:  ayup1

  ==============================================================================
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
		}
		else
		{
			audioDeviceSelectorComponent->getChildComponent(midiOutputSelectorId)->setVisible(true);
		}
	}
}

void AudioDeviceAndMidiOutputSelectorComponent::buttonClicked(Button* clickedButton)
{

}

void AudioDeviceAndMidiOutputSelectorComponent::resized()
{
	audioDeviceSelectorComponent->setBounds(0,0,getWidth(),getHeight());
	Rectangle<int> r(proportionOfWidth(0.35f), 15, proportionOfWidth(0.6f), 3000);
	const int space = itemHeight / 4;

	
	audioDeviceSelectorComponent->getChildComponent(0)->setBounds(r.removeFromTop(itemHeight));
	r.removeFromTop(space * 3);
	

	audioDeviceSelectorComponent->getChildComponent(4)->resized();
	audioDeviceSelectorComponent->getChildComponent(4)->setBounds(r.removeFromTop(audioDeviceSelectorComponent->getChildComponent(4)->getHeight())
		.withX(0).withWidth(getWidth()));
	r.removeFromTop(space);
	

	internalSynthButton->setBounds(r.removeFromTop(itemHeight));
	r.removeFromTop(space);

	audioDeviceSelectorComponent->getChildComponent(midiOutputSelectorId)->setBounds(r.removeFromTop(itemHeight));

	r.removeFromTop(itemHeight);
	
	//AudioDeviceSelectorComponent::resized();
	//if (internalSynthButtonState)
	//{
	//	Rectangle<int> r = getChildComponent(midiOutputSelectorId)->getBoundsInParent();
	//	internalSynthButton->setBounds(r);
	//}
	//else
	//{
	//	Rectangle<int> r = getChildComponent(midiOutputSelectorId)->getBoundsInParent();
	//	internalSynthButton->setBounds(r);
	//	r.setBounds(r.getX(), r.getY() + itemHeight + itemHeight / 3, r.getWidth(), r.getHeight());
	//	getChildComponent(midiOutputSelectorId)->setBounds(r);
	//	//setSize(getWidth(), r.getY() + itemHeight + itemHeight / 3 + r.getHeight());
	//}
}

void AudioDeviceAndMidiOutputSelectorComponent::setItemHeight(int newItemHeight)
{
	itemHeight = newItemHeight;
	audioDeviceSelectorComponent->setItemHeight(itemHeight);
}

void AudioDeviceAndMidiOutputSelectorComponent::removeAllChangeListener()
{
	audioDeviceSelectorComponent->deviceManager.removeAllChangeListeners();
}
