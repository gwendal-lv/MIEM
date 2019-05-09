/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"



namespace Miam {

	class MyPacaranaManager;
	class OscConnector;

	//[/Headers]



	//==============================================================================
	/**
																		//[Comments]
								An auto-generated component, created by the Projucer.

								Describe your class and how it works here!
																		//[/Comments]
	*/
	class MyOscConnector : public Component,
		public Button::Listener
	{
	public:
		//==============================================================================
		MyOscConnector();
		~MyOscConnector();

		//==============================================================================
		//[UserMethods]     -- You can add your own custom methods in this section.

		void init(OscConnector* refConnect);

		// Visual improvements
		void updateButtonsAndText();
		void addToLog(TextEditor* tE, String str);

		//[/UserMethods]

		void paint(Graphics& g) override;
		void resized() override;
		void buttonClicked(Button* buttonThatWasClicked) override;



	private:
		//[UserVariables]   -- You can add your own custom variables in this section.
		OscConnector* connector;
		//[/UserVariables]

		//==============================================================================
		std::unique_ptr<Label> title;
		std::unique_ptr<Label> label_portOut;
		std::unique_ptr<Label> label_statusIn;
		std::unique_ptr<Label> label_portIn;
		std::unique_ptr<Label> label_statusOut;
		std::unique_ptr<GroupComponent> group_sent;
		std::unique_ptr<GroupComponent> group_received;
		std::unique_ptr<TextEditor> messageSent;
		std::unique_ptr<TextEditor> messageReceived;
		std::unique_ptr<TextEditor> portIn_line;
		std::unique_ptr<TextEditor> portOut_line;
		std::unique_ptr<Label> infoLabel;
		std::unique_ptr<TextEditor> box_ip;
		std::unique_ptr<TextButton> comboButton;
		std::unique_ptr<TextButton> StopButton;


		//==============================================================================
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyOscConnector)
	};

	//[EndFile] You can add extra defines here...


}
//[/EndFile]

