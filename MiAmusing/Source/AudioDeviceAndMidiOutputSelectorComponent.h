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

#pragma once

#include "JuceHeader.h"

class AudioDeviceAndMidiOutputSelectorComponent : public Component, public Button::Listener
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
	void resized() override;
	void setItemHeight(int newItemHeight);

	void saveAudioDeviceCurrentState();
	void reloadPreviousState();

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
	
	String savedAudioDeviceType;
	AudioDeviceManager::AudioDeviceSetup savedState;
	bool internalSynthButtonStateSaved;

	void buttonStateChanged(Button*) override;
	void buttonClicked(Button*) override;
};
