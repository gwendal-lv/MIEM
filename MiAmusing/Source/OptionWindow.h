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
#include "AudioDeviceAndMidiOutputSelectorComponent.h"
class MainContentComponent;

namespace Amusing
{
	class OptionWindow : public Component,
		public Button::Listener, public ChangeListener
	{
	public:
		OptionWindow(MainContentComponent* _mainComponent);
		~OptionWindow();
		void CompleteInitialization(std::shared_ptr<AudioDeviceManager> deviceManager);
		void removeDeviceManager();

		void paint(Graphics& g) override;
		void resized() override;

		void buttonClicked(Button * button) override;

		void changeListenerCallback(ChangeBroadcaster*) override;

		void saveAudioDeviceCurrentState();

	private:
		TextButton* OKbutton;
		TextButton* cancelButton;
		ScopedPointer<AudioDeviceAndMidiOutputSelectorComponent> audioSetupComp;
		MainContentComponent* mainComponent;
		std::shared_ptr<AudioDeviceManager> deviceManager;

		/*static String getListOfActiveBits(const BitArray& b)
		{
			StringArray bits;

			for (int i = 0; i <= b.getHighestBit(); ++i)
				if (b[i])
					bits.add(String(i));

			return bits.joinIntoString(", ");
		}*/

	};
}
