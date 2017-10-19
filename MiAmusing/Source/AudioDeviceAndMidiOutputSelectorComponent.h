/*
  ==============================================================================

    AudioDeviceAndMidiOutputSelectorComponent.h
    Created: 16 Oct 2017 11:02:01am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class AudioDeviceAndMidiOutputSelectorComponent : public Component, public ButtonListener
{
public:
	/* This component is an an AudioDeviceSelectorComponent with the possibility to 
	select the "internal Midi output". It contains some checkbox to show or not 
	some audio parameters
	
	Without the MidiInput ComboBox, the index of the components are:
	0 : audio device type ComboBox
	1 : audio device type Label
	2 : midi output ComboBox
	3 : midi output Lable
	4 : Output, Input, active output, active input, sampleRate, audio buffer size

	*/

	AudioDeviceAndMidiOutputSelectorComponent(AudioDeviceManager& dm,
		const int minInputChannels_,
		const int maxInputChannels_,
		const int minOutputChannels_,
		const int maxOutputChannels_,
		const bool showMidiOutputSelector,
		const bool showChannelsAsStereoPairs_,
		const bool hideAdvancedOptionsWithButton_);
	~AudioDeviceAndMidiOutputSelectorComponent();

	void setMidiOutputSelectorIsVisible(bool isVisible);
	void resized();
	void setItemHeight(int newItemHeight);

	void removeAllChangeListener();

private:
	int itemHeight;
	int midiOutputSelectorId;
	ToggleButton* internalSynthButton;
	bool internalSynthButtonState;
	//ScopedPointer<ComboBox> deviceTypeDropDown;
	//ScopedPointer<Label> deviceTypeDropDownLabel;
	//ScopedPointer<Component> audioDeviceSettingsComp;
	AudioDeviceSelectorComponent* audioDeviceSelectorComponent;
	//ScopedPointer<MidiInputSelectorComponentListBox> midiInputsList;
	//ScopedPointer<ComboBox> midiOutputSelector;
	//ScopedPointer<Label> midiInputsLabel, midiOutputLabel;
	//ScopedPointer<TextButton> bluetoothButton;
	

	void buttonStateChanged(Button*) override;
	void buttonClicked(Button*) override;
};
