/*
  ==============================================================================

    OptionWindow.h
    Created: 5 Jul 2017 11:00:48am
    Author:  ayup1

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AudioDeviceAndMidiOutputSelectorComponent.h"
class MainContentComponent;

namespace Amusing
{
	class OptionWindow : public Component,
		public ButtonListener, public ChangeListener
	{
	public:
		OptionWindow(MainContentComponent* _mainComponent);
		~OptionWindow();
		void CompleteInitialization(std::shared_ptr<AudioDeviceManager> deviceManager);
		void removeDeviceManager();

		void paint(Graphics& g);
		void resized();

		void buttonClicked(Button * button) override;

		void changeListenerCallback(ChangeBroadcaster*) override;

	private:
		TextButton* OKbutton;
		ScopedPointer<AudioDeviceAndMidiOutputSelectorComponent> audioSetupComp;
		MainContentComponent* mainComponent;
		std::shared_ptr<AudioDeviceManager> deviceManager;

		static String getListOfActiveBits(const BitArray& b)
		{
			StringArray bits;

			for (int i = 0; i <= b.getHighestBit(); ++i)
				if (b[i])
					bits.add(String(i));

			return bits.joinIntoString(", ");
		}

	};
}
