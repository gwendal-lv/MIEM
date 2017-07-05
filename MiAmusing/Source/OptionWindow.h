/*
  ==============================================================================

    OptionWindow.h
    Created: 5 Jul 2017 11:00:48am
    Author:  ayup1

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
class MainContentComponent;

namespace Amusing
{
	class OptionWindow : public Component,
		public ButtonListener
	{
	public:
		OptionWindow(MainContentComponent* _mainComponent);
		~OptionWindow();
		void CompleteInitialization(AudioDeviceManager& deviceManager);
		void removeDeviceManager();

		void paint(Graphics& g);
		void resized();

		void buttonClicked(Button * button) override;

	private:
		TextButton* OKbutton;
		SafePointer<AudioDeviceSelectorComponent> audioSetupComp;
		MainContentComponent* mainComponent;
	};
}
